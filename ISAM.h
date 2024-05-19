#pragma once
#include "Pokemon.h"

const string DATOS = "files/pokemon.dat";
const string FIRST_INDEX = "files/firstindex.dat";
const string SECOND_INDEX = "files/secondindex.dat";

class ISAM {
   private:
    fstream datafile;
    fstream firstindex;
    fstream secondindex;

    void create_files() {
        datafile.open(DATOS, ios::out | ios::binary);
        firstindex.open(FIRST_INDEX, ios::out | ios::binary);
        secondindex.open(SECOND_INDEX, ios::out | ios::binary);
    }

    void close_files() {
        if (datafile.is_open())
            datafile.close();

        if (firstindex.is_open())
            firstindex.close();

        if (secondindex.is_open())
            secondindex.close();
    }

   public:
    void build() {
        // La idea es recorrer el vector de pokemon una sola vez, al igual que abrir y cerrar los archivos una sola vez.
        // Crear el ISAM tiene como complejidad O(n), n siendo la cantidad de pokemon que hay en el vector.

        vector<Pokemon> pokemon = read_csv_to_pokemon();  // O(n)

        // Contador para saber la posicion de indice del segundo nivel
        int contador_pos = 0;

        // Hay 1072 registros de pokemon en total y queremos crear indices proporcionales
        create_files();  // crea los archivos y los abre (1 sola vez)

        // Construir los archivos
        for (Pokemon p : pokemon) {
            datafile.write(reinterpret_cast<char *>(&p), sizeof(Pokemon));

            // Queremos guardar la serie 1, 5, 9, 13, 17, ..., 4n + 1.
            // Hay 4 registros de pokemon por indice en el segundo nivel. (268 registros)
            if (p.get_key() % 4 == 1) {
                // Guardamos la position donde ya se escribio el Pokemon
                streampos pos = datafile.tellp() - static_cast<streampos>(sizeof(Pokemon));

                // Creamos un PokemonIndex que solo guarda la llave y la position
                PokemonIndex2 p2(p.get_key(), pos);

                // Se escribe el registro en el index
                secondindex.write(reinterpret_cast<char *>(&p2), sizeof(PokemonIndex2));

                // Queremos guardar la serie 1, 17, 33, 49, ..., 16n + 1.
                // Hay 4 registros de pokemon por indice en el primer nivel. (67 registros)

                if (p.get_key() % 16 == 1) {
                    PokemonIndex1 p1(p.get_key(), pos, contador_pos);
                    firstindex.write(reinterpret_cast<char *>(&p1), sizeof(PokemonIndex1));
                    contador_pos = contador_pos + 4;
                }
            }
        }

        close_files();  // cierra los archivos
    }

    Pokemon search(int key) {
        if (key < 1 || key > 1072) {
            throw invalid_argument("Ingrese una llave valida!");
        }

        // La idea es buscar de mas general a mas especifico, primero en el nivel uno, luego en el 2 y luego en el archivo fisico

        // Lee el primer nivel de índices
        vector<PokemonIndex1> first_level_indexes = read_all_index();

        // Busca en el primer nivel el índice más cercano a la clave
        PokemonIndex1 first_level;
        for (auto &index : first_level_indexes) {
            if (index.get_key() == key) {
                Pokemon p = physical_read(index.get_pos());
                return p;
            }

            else if (index.get_key() < key)
                first_level = index;

            else
                break;
        }

        // Lee el segundo nivel de índices a partir de la posicion mas cercana
        vector<PokemonIndex2> second_level_indexes = read_part_index(first_level.get_vector_pos());

        // Busca en el segundo nivel el índice más cercano a la clave, son solo 4 registros
        PokemonIndex2 second_level;
        for (auto &index : second_level_indexes) {
            if (index.get_key() == key) {
                Pokemon p = physical_read(index.get_pos());
                return p;
            }

            else if (index.get_key() < key)
                second_level = index;

            else
                break;
        }

        // Si no esta en los indices, abrimos el archivo de datos y buscamos ahi
        fstream file(DATOS, ios::in | ios::binary);
        if (!file)
            throw runtime_error("Error al leer datos");
        file.seekg(second_level.get_pos(), ios::beg);

        Pokemon p;

        for (int i = 0; i < 4; ++i) {  // solo puede estar dentro de estos 4 por como se definio el indice 2
            file.read(reinterpret_cast<char *>(&p), sizeof(Pokemon));

            if (p.get_key() == key)
                return p;
        }

        return p;
    }

    vector<Pokemon> range_search(int key1, int key2) {
        if (key1 < 1 || key1 > 1072 || key2 < 1 || key2 > 1072) {
            throw invalid_argument("Ingrese una llave valida!");
        }

        vector<PokemonIndex1> first_level_indexes = read_all_index();

        PokemonIndex1 first_level;
        for (auto &index : first_level_indexes) {
            if (index.get_key() == key1) {
                vector<Pokemon> p = read_while(index.get_pos(), key2);
                return p;
            }

            else if (index.get_key() < key1)
                first_level = index;

            else
                break;
        }

        // Lee el segundo nivel de índices a partir de la posicion mas cercana
        vector<PokemonIndex2> second_level_indexes = read_part_index(first_level.get_vector_pos());

        // Busca en el segundo nivel el índice más cercano a la clave, son solo 4 registros
        PokemonIndex2 second_level;
        for (auto &index : second_level_indexes) {
            if (index.get_key() == key1) {
                vector<Pokemon> p = read_while(index.get_pos(), key2);
                return p;
            }

            else if (index.get_key() < key1)
                second_level = index;

            else
                break;
        }

        // Si no esta en los indices, abrimos el archivo de datos y buscamos ahi
        fstream file(DATOS, ios::in | ios::binary);
        if (!file)
            throw runtime_error("Error al leer datos");
        file.seekg(second_level.get_pos(), ios::beg);

        Pokemon p;
        vector<Pokemon> pokemon;

        for (int i = 0; i < 4; ++i) {  // solo puede estar dentro de estos 4 por como se definio el indice 2
            file.read(reinterpret_cast<char *>(&p), sizeof(Pokemon));
            if (p.get_key() == key1) {
                pokemon.push_back(p);
                break;
            }
        }

        while (file.peek() != EOF) {
            file.read(reinterpret_cast<char *>(&p), sizeof(Pokemon));
            pokemon.push_back(p);
            if (p.get_key() == key2)
                break;
        }

        return pokemon;
    }

    // Lee la posicion fisica en el archivo de datos
    Pokemon physical_read(int pos) {
        fstream file(DATOS, ios::in | ios::binary);
        if (!file)
            throw runtime_error("Error al leer datos");
        file.seekg(pos, ios::beg);
        Pokemon p;
        file.read(reinterpret_cast<char *>(&p), sizeof(Pokemon));
        return p;
    }

    // Lee la posicion logica en el archivo de datos
    Pokemon logical_read(int pos) {
        fstream file(DATOS, ios::in | ios::binary);
        if (!file)
            throw runtime_error("Error al leer datos");
        file.seekg(pos * sizeof(Pokemon), ios::beg);
        Pokemon p;
        file.read(reinterpret_cast<char *>(&p), sizeof(Pokemon));
        return p;
    }

    // Guarda todos los registros del Indice 1 (raiz) y los almacena en un vector
    vector<PokemonIndex1> read_all_index() {
        fstream file(FIRST_INDEX, ios::in | ios::binary);
        if (!file)
            throw runtime_error("Error al leer indices");
        vector<PokemonIndex1> indexes;
        while (file.peek() != EOF) {
            PokemonIndex1 p;
            file.read(reinterpret_cast<char *>(&p), sizeof(PokemonIndex1));
            indexes.push_back(p);
        }
        file.close();
        return indexes;
    }

    // Guarda solamente 4 registros del Indice 2 y los almacena en un vector
    vector<PokemonIndex2> read_part_index(int vector_pos) {
        fstream file(SECOND_INDEX, ios::in | ios::binary);
        if (!file)
            throw runtime_error("Error al leer indices");
        vector<PokemonIndex2> indexes;
        file.seekg(vector_pos * sizeof(PokemonIndex2));

        for (int i = 0; i < 4; ++i) {
            if (file.peek() == EOF)
                break;
            PokemonIndex2 p;
            file.read(reinterpret_cast<char *>(&p), sizeof(PokemonIndex2));
            indexes.push_back(p);
        }

        file.close();
        return indexes;
    }

    vector<Pokemon> read_while(int pos, int key) {
        fstream file(DATOS, ios::in | ios::binary);
        if (!file)
            throw runtime_error("Error al leer datos");

        vector<Pokemon> pokemon;
        file.seekg(pos, ios::beg);

        while (file.peek() != EOF) {
            Pokemon p;
            file.read(reinterpret_cast<char *>(&p), sizeof(Pokemon));
            pokemon.push_back(p);
            if (p.get_key() == key)
                break;
        }
        return pokemon;
    }
};
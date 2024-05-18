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

        // Hay 1072 registros de pokemon en total y queremos crear indices proporcionales
        // Fue una coincidencia que la cantidad de registros fuera multiplo de 16 xd.

        create_files();  // crea los archivos y los abre (1 sola vez)

        for (Pokemon p : pokemon) {
            datafile.write(reinterpret_cast<char *>(&p), sizeof(Pokemon));

            // Queremos guardar la serie 1, 5, 9, 13, 17, ..., 4n + 1.
            // Hay 4 registros de pokemon por indice en el segundo nivel. (268 registros)
            if (p.get_key() % 4 == 1) {
                // Guardamos la position donde ya se escribio el Pokemon
                streampos pos = datafile.tellp() - static_cast<streampos>(sizeof(Pokemon));

                // Creamos un PokemonIndex que solo guarda la llave y la position
                PokemonIndex pi(p.get_key(), pos);

                // Se escribe el registro en el index
                secondindex.write(reinterpret_cast<char *>(&pi), sizeof(PokemonIndex));

                // Queremos guardar la serie 1, 17, 33, 49, ..., 16n + 1.
                // Hay 4 registros de pokemon por indice en el primer nivel. (67 registros)

                // Ponemos este if dentro del otro porque los valores se repetiran, mod 16 es subconjuto de mod 4
                // No hace falta volver a instanciar otro objeto PokemonIndex.

                if (p.get_key() % 16 == 1)
                    firstindex.write(reinterpret_cast<char *>(&pi), sizeof(PokemonIndex));
            }
        }

        close_files();  // cierra los archivos
    }

    // Pokemon search(int key) {}

    Pokemon read_data(int pos) {
        fstream file(DATOS, ios::in | ios::binary);
        if (!file)
            throw runtime_error("Error al leer datos");
        file.seekg(pos * sizeof(Pokemon), ios::beg);
        Pokemon p;
        file.read(reinterpret_cast<char *>(&p), sizeof(Pokemon));
        return p;
    }

    PokemonIndex read_index(int pos, string index_name) {
        fstream file(index_name, ios::in | ios::binary);
        if (!file)
            throw runtime_error("Error al leer indice");
        file.seekg(pos * sizeof(PokemonIndex), ios::beg);
        PokemonIndex pi;
        file.read(reinterpret_cast<char *>(&pi), sizeof(PokemonIndex));
        return pi;
    }

    vector<PokemonIndex> read_all_indexes(string index_name) {
        fstream file(index_name, ios::in | ios::binary);
        if (!file)
            throw runtime_error("Error al leer indices");
        vector<PokemonIndex> indexes;
        while (file.peek() != EOF) {
            PokemonIndex pi;
            file.read(reinterpret_cast<char *>(&pi), sizeof(PokemonIndex));
            indexes.push_back(pi);
        }
        file.close();
        return indexes;
    }
};
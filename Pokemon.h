#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

class Pokemon {
   private:
    int key;
    int pokedex;
    char name[50];
    char type1[20];
    char type2[20];
    int total;
    int hp;
    int attack;
    int defense;
    int sp_attack;
    int sp_defense;
    int speed;
    int generation;
    bool legendary;

   public:
    Pokemon() : key(0), pokedex(0), total(0), hp(0), attack(0), defense(0), sp_attack(0), sp_defense(0), speed(0), generation(0), legendary(false) {
        // Inicializar los arreglos con valores nulos
        memset(name, '\0', sizeof(name));
        memset(type1, '\0', sizeof(type1));
        memset(type2, '\0', sizeof(type2));
    }

    void set_values(int key, int pokedex, const char* name, const char* type1, const char* type2, int total, int hp, int attack, int defense, int sp_attack, int sp_defense, int speed, int generation, bool legendary) {
        this->key = key;
        this->pokedex = pokedex;
        strncpy(this->name, name, sizeof(this->name) - 1);  // Copiar y asegurarse de que la cadena esté terminada correctamente
        this->name[sizeof(this->name) - 1] = '\0';
        strncpy(this->type1, type1, sizeof(this->type1) - 1);
        this->type1[sizeof(this->type1) - 1] = '\0';
        strncpy(this->type2, type2, sizeof(this->type2) - 1);
        this->type2[sizeof(this->type2) - 1] = '\0';
        this->total = total;
        this->hp = hp;
        this->attack = attack;
        this->defense = defense;
        this->sp_attack = sp_attack;
        this->sp_defense = sp_defense;
        this->speed = speed;
        this->generation = generation;
        this->legendary = legendary;
    }

    void print_values() {
        cout << "Key: " << key << "\n";
        cout << "Pokedex Value: " << pokedex << "\n";
        cout << "Name: " << name << "\n";
        cout << "Type 1: " << type1 << "\n";
        cout << "Type 2: " << type2 << "\n";
        cout << "Total Stats: " << total << "\n";
        cout << "Hp Stats: " << hp << "\n";
        cout << "Attack Stats: " << attack << "\n";
        cout << "Defense Stats: " << defense << "\n";
        cout << "Special Attack Stats: " << sp_attack << "\n";
        cout << "Special Defense Stats: " << sp_defense << "\n";
        cout << "Speed Stats: " << speed << "\n";
        cout << "Generation Number: " << generation << "\n";
        cout << "Legendary? " << boolalpha << legendary << "\n";
    }

    int get_key() {
        return this->key;
    }
};

class PokemonIndex {
   private:
    int key;
    streampos pos;

   public:
    PokemonIndex() {}
    PokemonIndex(int key, streampos pos) : key(key), pos(pos) {}
    void print_values() {
        cout << "Key: " << key << "\n";
        cout << "Pos: " << pos << "\n"; 
    }
};

vector<Pokemon> read_csv_to_pokemon() {
    vector<Pokemon> pokemons;
    ifstream file("dataset/pokemon.csv");
    string line;
    bool firstLine = true;  // Flag para identificar si es la primera línea
    while (getline(file, line)) {
        if (firstLine) {
            firstLine = false;  // Marcar que ya hemos pasado la primera línea
            continue;           // Saltar al siguiente ciclo para omitir la primera línea
        }
        stringstream ss(line);
        string token;
        int key, pokedex, total, hp, attack, defense, sp_attack, sp_defense, speed, generation;
        char name[50], type1[20], type2[20];
        bool legendary;

        // Leer los datos de la línea separada por comas
        getline(ss, token, ',');
        key = stoi(token);
        getline(ss, token, ',');
        pokedex = stoi(token);
        getline(ss, token, ',');
        strncpy(name, token.c_str(), sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
        getline(ss, token, ',');
        strncpy(type1, token.c_str(), sizeof(type1) - 1);
        type1[sizeof(type1) - 1] = '\0';
        getline(ss, token, ',');
        strncpy(type2, token.c_str(), sizeof(type2) - 1);
        type2[sizeof(type2) - 1] = '\0';
        getline(ss, token, ',');
        total = stoi(token);
        getline(ss, token, ',');
        hp = stoi(token);
        getline(ss, token, ',');
        attack = stoi(token);
        getline(ss, token, ',');
        defense = stoi(token);
        getline(ss, token, ',');
        sp_attack = stoi(token);
        getline(ss, token, ',');
        sp_defense = stoi(token);
        getline(ss, token, ',');
        speed = stoi(token);
        getline(ss, token, ',');
        generation = stoi(token);
        getline(ss, token, ',');
        legendary = (token == "True");

        // Crear un nuevo objeto Pokemon y agregarlo al vector
        Pokemon pokemon;
        pokemon.set_values(key, pokedex, name, type1, type2, total, hp, attack, defense, sp_attack, sp_defense, speed, generation, legendary);
        pokemons.push_back(pokemon);
    }
    return pokemons;
}

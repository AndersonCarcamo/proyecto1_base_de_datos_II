#include <iostream>

#include "ISAM.h"
using namespace std;

int main() {
    ISAM isam;
    isam.build();

    // try {
    //     isam.search(10000).print_values();
    // } catch (const invalid_argument& e) {
    //     std::cerr << "Error: " << e.what() << std::endl;
    // }

    isam.search(100).print_values();

    // vector<PokemonIndex1> vec = isam.read_all_index();

    // for (auto& v : vec) {
    //     v.print_values();
    // }
    // cout << endl;


    return 0;
}
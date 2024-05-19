#include <iostream>

#include "ISAM.h"
using namespace std;

void test_build() {
    ISAM isam;
    isam.build();
}

void search_test() {
    ISAM isam;

    // rango de busqueda: 1 <= n <= 1072
    cout << "Search 5 -----" << "\n";
    isam.search(5).print_values();
    cout << "\n";

    cout << "Search 99 -----:" << "\n";
    isam.search(99).print_values();
    cout << "\n";

    cout << "Search 800 -----:" << "\n";
    isam.search(800).print_values();
    cout << "\n";

    cout << "Search 700 -----:" << "\n";
    isam.search(700).print_values();
}

void range_search_test() {
    ISAM isam;

    vector<Pokemon> p1 = isam.range_search(2, 4);
    vector<Pokemon> p2 = isam.range_search(50, 55);
    vector<Pokemon> p3 = isam.range_search(10, 15);

    for (auto& p : p1) {
        p.print_values();
        cout << "\n";
    }

    for (auto& p : p2) {
        p.print_values();
        cout << "\n";
    }

    for (auto& p : p3) {
        p.print_values();
        cout << "\n";
    }

    cout << endl;
}

int main() {
    // eliminar los arhivos en la carpeta de files antes de descomentar la construccion del ISAM
    test_build(); 
    search_test();
    range_search_test();
    return 0;
}
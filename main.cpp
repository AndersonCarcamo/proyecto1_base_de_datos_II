#include <iostream>

#include "ISAM.h"
using namespace std;

int main() {
    ISAM isam;
    isam.build();

    // rango de busqueda: 1 <= n <= 1072
    cout << "Search 5 -----" << "\n";
    isam.search(5).print_values();
    cout << "\n";

    cout << "Search 99 -----:" << "\n";
    isam.search(99).print_values();
    cout << "\n";

    cout << "Search 800 -----:" << "\n";
    isam.search(800).print_values();

    return 0;
}
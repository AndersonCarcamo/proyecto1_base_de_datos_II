#include <iostream>

#include "ISAM.h"
using namespace std;

int main() {
    ISAM isam;
    // isam.build();
    // try {
    //     isam.search(10000).print_values();
    // } catch (const invalid_argument& e) {
    //     std::cerr << "Error: " << e.what() << std::endl;
    // }

    isam.search(200).print_values();


    return 0;
}
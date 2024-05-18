#include <iostream>

#include "ISAM.h"
using namespace std;

int main() {
    ISAM isam;
    // isam.build();

    vector<PokemonIndex> records = isam.read_all_indexes(SECOND_INDEX);

    for (auto& record : records) {
        record.print_values();
    }
}
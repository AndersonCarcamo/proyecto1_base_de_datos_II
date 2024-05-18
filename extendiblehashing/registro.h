//
// Created by ambar on 17/05/24.
//

#ifndef PROYECTO1_BASE_DE_DATOS_II_REGISTRO_H
#define PROYECTO1_BASE_DE_DATOS_II_REGISTRO_H
#include <string>


class Registro {
public:
    int key;
    std::string data;

    Registro(int k, const std::string& d) : key(k), data(d) {}
};
#endif //PROYECTO1_BASE_DE_DATOS_II_REGISTRO_H

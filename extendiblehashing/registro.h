//
// Created by ambar on 17/05/24.
//

#ifndef PROYECTO1_BASE_DE_DATOS_II_REGISTRO_H
#define PROYECTO1_BASE_DE_DATOS_II_REGISTRO_H
#include <string>

// registro para un record simple
/*
class Registro {
public:
    int key;
    std::string data;

    Registro(int k, const std::string& d) : key(k), data(d) {}
};
*/

// sin ebmargo como voy a seterale n parametros del csv, yo no se que valores tendra como datos, por ende:

template <typename V>

//ACOMODAR

class Registro {
public:
    std::string key;
    std::unordered_map<std::string, V> data;

    Registro() = default;

    void setKey(const std::string& keyName, const std::any& keyValue) {
        key = std::any_cast<std::string>(keyValue);
        data[keyName] = keyValue;
    }

    template<typename T>
    void setField(const std::string& fieldName, const T& fieldValue) {
        data[fieldName] = fieldValue;
    }

    template<typename T>
    T getField(const std::string& fieldName) const {
        auto it = data.find(fieldName);
        if (it != data.end()) {
            return std::any_cast<T>(it->second);
        }
        return T();
    }

    std::string getKey() const {
        return key;
    }
};

#endif //PROYECTO1_BASE_DE_DATOS_II_REGISTRO_H

#ifndef PROYECTO1_BASE_DE_DATOS_II_BUCKET_EH_HH
#define PROYECTO1_BASE_DE_DATOS_II_BUCKET_EH_HH

// librerias
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <bitset>
// dependencias

#include "registro.h"

using namespace std;

template <typename T>
class Bucket_EH {
private:
    int localDepth; // depth del bucket
    int size; // cantidad de registros actuales en el bucket
    int size_max; // capacidad del bucket
    vector<Registro> records;
public:
    Bucket_EH(int localDepth, int max);
    ~Bucket_EH();

    bool isFull() const; // si el size ha llegado a la cantidad maxima de registros.
    bool add(const Registro& record); // añade un registro al bucket
    bool remove(const T& key); // quita un registro
    vector<Registro> search(const T& key) const;
    void redistribute(std::unordered_map<std::string, Bucket_EH<T>*>& directory); //reorganiza los registros y los divide en dos buckets nuevos
    int getDepth() const;
    vector<Registro> getRecords() const;
    bool isEmpty();
    void increaseDepth(); // aumenta la profundidad local
    void decreaseDepth(); // disminuye la profuncidad local
    void clear();
};

template <typename T>
Bucket_EH<T>::Bucket_EH(int localDepth, int max) {
    this->size_max = max;
    this->localDepth = localDepth;
    this->size = 0;
}

template <typename T>
Bucket_EH<T>::~Bucket_EH() {}

template <typename T>
bool Bucket_EH<T>::isFull() const {
    return records.size() >= size;
}

template <typename T>
bool Bucket_EH<T>::isEmpty() {
    return size == 0;
}

template <typename T>
bool Bucket_EH<T>::add(const Registro& record) {
    if (!isFull()) {
        records.push_back(record);
        return true;
    }
    return false;
}

template <typename T>
bool Bucket_EH<T>::remove(const T& key) {
    auto it = std::remove_if(records.begin(), records.end(), [&key](const Registro& record) {
        return record.key == key;
    });
    if (it != records.end()) {
        records.erase(it, records.end());
        return true;
    }
    return false;
}

template <typename T>
std::vector<Registro> Bucket_EH<T>::search(const T& key) const {
    std::vector<Registro> result;
    for (const auto& record : records) {
        if (record.key == key) {
            result.push_back(record);
        }
    }
    return result;
}

template <typename T>
void Bucket_EH<T>::increaseDepth() {
    localDepth++;
}

template <typename T>
void Bucket_EH<T>::decreaseDepth() {
    localDepth--;
}

template <typename T>
int Bucket_EH<T>::getDepth() const {
    return localDepth;
}

template <typename T>
std::vector<Registro> Bucket_EH<T>::getRecords() const {
    return records;
}

template <typename T>
void Bucket_EH<T>::clear() {
    records.clear();
}

template <typename T>
void Bucket_EH<T>::redistribute(std::unordered_map<std::string, Bucket_EH<T>*>& directory) {
    std::vector<Registro> oldRecords = records;
    clear();
    for (const auto& record : oldRecords) {
        std::string newBucketIndex = std::bitset<sizeof(std::size_t) * 8>(std::hash<T>{}(record.key)).to_string();
        newBucketIndex = newBucketIndex.substr(newBucketIndex.size() - localDepth);
        directory[newBucketIndex]->add(record);
    }
}

#endif //PROYECTO1_BASE_DE_DATOS_II_BUCKET_EH_HH

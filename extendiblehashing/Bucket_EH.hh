#ifndef PROYECTO1_BASE_DE_DATOS_II_BUCKET_EH_HH
#define PROYECTO1_BASE_DE_DATOS_II_BUCKET_EH_HH

// librerias
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <bitset>
// dependencias
#include <fstream>
#include "registro.h"

using namespace std;

template <typename T>
class Bucket_EH {
private:
    int localDepth; // depth del bucket
    int size; // cantidad de registros actuales en el bucket
    vector<Registro> records;
public:
    int size_max; // capacidad del bucket
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

    void save(std::ofstream& outFile) const;
    void load(std::ifstream& inFile);
};

template <typename T>
Bucket_EH<T>::Bucket_EH(int max, int localDepth) {
    this->size_max = max;
    this->localDepth = localDepth;
    this->size = 0;
}

template <typename T>
Bucket_EH<T>::~Bucket_EH() {}

template <typename T>
bool Bucket_EH<T>::isFull() const {
    return records.size() >= size_max;
}

template <typename T>
bool Bucket_EH<T>::isEmpty() {
    return records.empty();
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
        return record.getKey() == key;
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
        if (record.getKey() == key) {
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
        std::string newBucketIndex = std::bitset<sizeof(std::size_t) * 8>(std::hash<T>{}(record.getKey())).to_string();
        newBucketIndex = newBucketIndex.substr(newBucketIndex.size() - localDepth);
        directory[newBucketIndex]->add(record);
    }
}

template <typename T>
void Bucket_EH<T>::save(ofstream& outFile) const {
    outFile.write(reinterpret_cast<const char*>(&localDepth), sizeof(localDepth));
    int recordCount = records.size();
    outFile.write(reinterpret_cast<const char*>(&recordCount), sizeof(recordCount));
    for (const auto& record : records) {
        outFile.write(reinterpret_cast<const char*>(&record.ID), sizeof(record.ID));

        size_t size = record.Time_GMT.size();
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
        outFile.write(record.Time_GMT.c_str(), size);

        size = record.Phone.size();
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
        outFile.write(record.Phone.c_str(), size);

        size = record.Organization.size();
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
        outFile.write(record.Organization.c_str(), size);

        size = record.OLF.size();
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
        outFile.write(record.OLF.c_str(), size);

        outFile.write(reinterpret_cast<const char*>(&record.Rating), sizeof(record.Rating));
        outFile.write(reinterpret_cast<const char*>(&record.NumberReview), sizeof(record.NumberReview));

        size = record.Category.size();
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
        outFile.write(record.Category.c_str(), size);

        size = record.Country.size();
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
        outFile.write(record.Country.c_str(), size);

        size = record.CountryCode.size();
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
        outFile.write(record.CountryCode.c_str(), size);

        size = record.State.size();
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
        outFile.write(record.State.c_str(), size);

        size = record.City.size();
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
        outFile.write(record.City.c_str(), size);

        size = record.Street.size();
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
        outFile.write(record.Street.c_str(), size);

        size = record.Building.size();
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
        outFile.write(record.Building.c_str(), size);
    }
}

template <typename T>
void Bucket_EH<T>::load(ifstream& inFile) {
    inFile.read(reinterpret_cast<char*>(&localDepth), sizeof(localDepth));
    int recordCount;
    inFile.read(reinterpret_cast<char*>(&recordCount), sizeof(recordCount));
    records.resize(recordCount);
    for (auto& record : records) {
        inFile.read(reinterpret_cast<char*>(&record.ID), sizeof(record.ID));

        size_t size;
        inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        record.Time_GMT.resize(size);
        inFile.read(&record.Time_GMT[0], size);

        inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        record.Phone.resize(size);
        inFile.read(&record.Phone[0], size);

        inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        record.Organization.resize(size);
        inFile.read(&record.Organization[0], size);

        inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        record.OLF.resize(size);
        inFile.read(&record.OLF[0], size);

        inFile.read(reinterpret_cast<char*>(&record.Rating), sizeof(record.Rating));
        inFile.read(reinterpret_cast<char*>(&record.NumberReview), sizeof(record.NumberReview));

        inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        record.Category.resize(size);
        inFile.read(&record.Category[0], size);

        inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        record.Country.resize(size);
        inFile.read(&record.Country[0], size);

        inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        record.CountryCode.resize(size);
        inFile.read(&record.CountryCode[0], size);

        inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        record.State.resize(size);
        inFile.read(&record.State[0], size);

        inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        record.City.resize(size);
        inFile.read(&record.City[0], size);

        inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        record.Street.resize(size);
        inFile.read(&record.Street[0], size);

        inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        record.Building.resize(size);
        inFile.read(&record.Building[0], size);
    }
}

#endif //PROYECTO1_BASE_DE_DATOS_II_BUCKET_EH_HH

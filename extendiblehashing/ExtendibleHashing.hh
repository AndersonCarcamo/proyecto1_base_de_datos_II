#ifndef PROYECTO1_BASE_DE_DATOS_II_EXTENDIBLEHASHING_HH
#define PROYECTO1_BASE_DE_DATOS_II_EXTENDIBLEHASHING_HH

#include <string>
#include <functional>
#include <bitset>
/* Se manejaran dos archivos:
 * 1. index_eh.dat: guarda el key direccion de cada bucket, el directorio
 * 2. data_eh.dat : archivo de la data
 */

//dependencias
#include "Bucket_EH.hh"
#include <cmath>


// manejo del indice de direcciones a los buckets
template <typename T>
class directory_EH {
private:
    int globalDepth, size_bucket; //deph global cantidad de caracteres que tendrá la key
    unordered_map<string, Bucket_EH<T>*> directory;
public:
    directory_EH(int depth, int size_bucket);
    ~directory_EH();
    void doubleDir(); // para duplicar el directorio si es que se lleno el directorio
    void splitBucket(string bucketIndex); // para dividir el bucket

    vector<Registro> search(T key); // retorna todos los registros guardados en el mismo de bucket(mismo key)
    bool add(Registro registro);
    bool remove(T key);

    void save(); // para guardar los datos en la estructura
    void load(); // para cargar la nueva estructura en los archivos
private:
    std::string getBucketIndex(T key); // obtiene el indice del bucket para una key
};


template <typename T>
directory_EH<T>::directory_EH(int depth, int size_bucket) {
    this->globalDepth = depth;
    this->size_bucket = size_bucket;

    int size = pow(2, globalDepth);
    for (int i = 0; i < size; ++i) {
        string bucketIndex = bitset<32>(i).to_string().substr(32 - globalDepth);
        directory[bucketIndex] = new Bucket_EH<T>(size_bucket, globalDepth);
    }
}

template <typename T>
directory_EH<T>::~directory_EH() {
    save();
    for (auto& pair : directory) {
        delete pair.second;
    }
}

template <typename T>
vector<Registro> directory_EH<T>::search(T key) {
    // aplicar la funcion hash y obtener la secuencia D-bit
    string bucketIndex = getBucketIndex(key);

    auto it = directory.find(bucketIndex);
    if (it != directory.end()) {
        return  it->second->search(key);
    }
    return {};
}

template <typename T>
bool directory_EH<T>::add(Registro registro) {
    string bucketIndex = getBucketIndex(registro.getKey());
    Bucket_EH<T>* bucket = directory[bucketIndex];

    if(!bucket->isFull())
        return bucket->add(registro);
    else { // si el bucket esta lleno
        splitBucket(bucketIndex);
        // ingreso el nuevo registro
        return add(registro);
    }
}

template <typename T>
bool directory_EH<T>::remove(T key) {
    string bucketIndex = getBucketIndex(key);
    auto it = directory.find(bucketIndex);
    if (it != directory.end()){
        return it->second->remove(key);
    }
    // si no removio nada, retorna false
    return false;
}

template <typename T>
void directory_EH<T>::doubleDir() {
    globalDepth++;
    unordered_map<string, Bucket_EH<T>*> newDirectory;

    for (auto& pair : directory) {
        string oldIndex = pair.first;
        Bucket_EH<T>* bucket = pair.second;

        newDirectory[oldIndex + "O"] = bucket;
        newDirectory[oldIndex + "1"] = new Bucket_EH<T>(*bucket);
    }
    directory = newDirectory;
}

template <typename T>
void directory_EH<T>::splitBucket(string bucketIndex) {
    Bucket_EH<T>* bucket = directory[bucketIndex];
    int localDepth = bucket->getDepth();

    if (localDepth == globalDepth)
        doubleDir();

    bucket->increaseDepth();

    unordered_map<string, Bucket_EH<T>*> newDirectory;

    for (auto& pair : directory) {
        string oldIndex = pair.first;
        Bucket_EH<T>* oldBucket = pair.second;

        if (oldBucket == bucket) {
            string newPrefix = oldIndex.substr(0, localDepth);
            newDirectory[newPrefix + "0"] = bucket;
            newDirectory[newPrefix + "1"] = new Bucket_EH<T>(size_bucket, bucket->getDepth());
        } else {
            newDirectory[oldIndex] = oldBucket;
        }
    }

    directory = newDirectory;
    bucket->redistribute(directory);
}

template <typename T>
std::string directory_EH<T>::getBucketIndex(T key) {
    // funcion para el hash con el valor de tipo T
    std::hash<T> hasher;
    // genera el size_t que es hash obtenido por el key y lo pasa a binario
    std::bitset<sizeof(std::size_t)*8> binaryHash(hasher(key));
    //convierte el bitset a string
    string HashBitString = binaryHash.to_string();
    //retorna los Deph global digitos que se usaran para directorio
    return HashBitString.substr(HashBitString.size() - this->globalDepth);
}

template <typename T>
void directory_EH<T>::save() {
    std::ofstream indexFile("index_eh.dat", std::ios::binary);
    std::ofstream dataFile("data_eh.dat", std::ios::binary);

    indexFile.write(reinterpret_cast<const char*>(&globalDepth), sizeof(globalDepth));
    int dirSize = directory.size();
    indexFile.write(reinterpret_cast<const char*>(&dirSize), sizeof(dirSize));

    for (const auto& pair : directory) {
        size_t indexSize = pair.first.size();
        indexFile.write(reinterpret_cast<const char*>(&indexSize), sizeof(indexSize));
        indexFile.write(pair.first.c_str(), indexSize);

        int bucketOffset = dataFile.tellp();
        indexFile.write(reinterpret_cast<const char*>(&bucketOffset), sizeof(bucketOffset));

        pair.second->save(dataFile);
    }
}

template <typename T>
void directory_EH<T>::load() {
    std::ifstream indexFile("index_eh.dat", std::ios::binary);
    std::ifstream dataFile("data_eh.dat", std::ios::binary);

    indexFile.read(reinterpret_cast<char*>(&globalDepth), sizeof(globalDepth));
    int dirSize;
    indexFile.read(reinterpret_cast<char*>(&dirSize), sizeof(dirSize));

    for (int i = 0; i < dirSize; ++i) {
        size_t indexSize;
        indexFile.read(reinterpret_cast<char*>(&indexSize), sizeof(indexSize));
        std::string bucketIndex(indexSize, '\0');
        indexFile.read(&bucketIndex[0], indexSize);

        int bucketOffset;
        indexFile.read(reinterpret_cast<char*>(&bucketOffset), sizeof(bucketOffset));
        dataFile.seekg(bucketOffset);

        Bucket_EH<T>* bucket = new Bucket_EH<T>(size_bucket, globalDepth);
        bucket->load(dataFile);

        directory[bucketIndex] = bucket;
    }
}

#endif //PROYECTO1_BASE_DE_DATOS_II_EXTENDIBLEHASHING_HH

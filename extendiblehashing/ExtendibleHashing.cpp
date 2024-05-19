//#include "ExtendibleHashing.hh"
//
//template <typename T>
//directory_EH<T>::directory_EH(int depth, int size_bucket) {
//    this->globalDepth = depth;
//    this->size_bucket = size_bucket;
//
//    int size = pow(2, globalDepth);
//    for (int i = 0; i < size; ++i) {
//        string bucketIndex = bitset<32>(i).to_string().substr(32 - globalDepth);
//        directory[bucketIndex] = new Bucket_EH<T>(size_bucket, globalDepth);
//    }
//}
//
//template <typename T>
//directory_EH<T>::~directory_EH() {
//    save();
//    for (auto& pair : directory) {
//        delete pair.second;
//    }
//}
//
//template <typename T>
//vector<Registro> directory_EH<T>::search(T key) const {
//    // aplicar la funcion hash y obtener la secuencia D-bit
//    string bucketIndex = getBucketIndex(key);
//
//    auto it = directory.find(bucketIndex);
//    if (it != directory.end()) {
//        return  it->second->search(key);
//    }
//    return {};
//}
//
//template <typename T>
//bool directory_EH<T>::add(Registro registro) {
//    string bucketIndex = getBucketIndex(registro.key);
//    Bucket_EH<T>* bucket = directory[bucketIndex];
//
//    if(!bucket->isFull())
//        return bucket->add(registro);
//    else { // si el bucket esta lleno
//        splitBucket(bucketIndex);
//        // ingreso el nuevo registro
//        return add(registro);
//    }
//}
//
//template <typename T>
//bool directory_EH<T>::remove(T key) {
//    string bucketIndex = getBucketIndex(key);
//    auto it = directory.find(bucketIndex);
//    if (it != directory.end()){
//        return it->second->remove(key);
//    }
//    // si no removio nada, retorna false
//    return false;
//}
//
//template <typename T>
//void directory_EH<T>::doubleDir() {
//    globalDepth++;
//    unordered_map<string, Bucket_EH<T>*> newDirectory;
//
//    for (auto& pair : directory) {
//        string oldIndex = pair.first;
//        Bucket_EH<T>* bucket = pair.second;
//
//        newDirectory[oldIndex + "O"] = bucket;
//        newDirectory[oldIndex + "1"] = new Bucket_EH<T>(*bucket);
//    }
//    directory = newDirectory;
//}
//
//template <typename T>
//void directory_EH<T>::splitBucket(int bucketIndex) {
//    Bucket_EH<T>* bucket = directory[bucketIndex];
//    int localDepth = bucket->getDepth();
//
//    if (localDepth == globalDepth)
//        doubleDir();
//
//    bucket->increaseDepth();
//
//    unordered_map<string, Bucket_EH<T>*> newDirectory;
//
//    for (auto& pair : directory){
//        string oldIndex = pair.first;
//        Bucket_EH<T>* oldBucket = pair.second;
//
//        if (oldBucket == bucket){
//            string newPrefix = oldIndex.substr(0, localDepth);
//            newDirectory[newPrefix + "0"] = bucket;
//            newDirectory[newPrefix + "1"] = new Bucket_EH<T>(size_bucket + 1);
//        }
//        else {
//            newDirectory[oldIndex] = oldBucket;
//        }
//    }
//
//    directory = newDirectory;
//    bucket->redistribute(directory);
//}
//
//template <typename T>
//std::string directory_EH<T>::getBucketIndex(T key) {
//    // funcion para el hash con el valor de tipo T
//    std::hash<T> hasher;
//    // genera el size_t que es hash obtenido por el key y lo pasa a binario
//    std::bitset<sizeof(std::size_t)*8> binaryHash(hasher(key));
//    //convierte el bitset a string
//    string HashBitString = binaryHash.to_string();
//    //retorna los Deph global digitos que se usaran para directorio
//    return HashBitString.substr(HashBitString.size() - this->globalDepth);
//}

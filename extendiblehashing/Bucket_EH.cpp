//#include "Bucket_EH.hh"
//
//template <typename T>
//Bucket_EH<T>::Bucket_EH(int localDepth, int max) {
//    this->size_max = max;
//    this->localDepth = localDepth;
//    this->size = 0;
//}
//
//template <typename T>
//Bucket_EH<T>::~Bucket_EH() {}
//
//template <typename T>
//bool Bucket_EH<T>::isFull() const {
//    return records.size() >= size;
//}
//
//template <typename T>
//bool Bucket_EH<T>::isEmpty() {
//    return size == 0;
//}
//
//template <typename T>
//bool Bucket_EH<T>::add(const Registro& record) {
//    if (!isFull()) {
//        records.push_back(record);
//        return true;
//    }
//    return false;
//}
//
//template <typename T>
//bool Bucket_EH<T>::remove(const T& key) {
//    auto it = std::remove_if(records.begin(), records.end(), [&key](const Registro& record) {
//        return record.key == key;
//    });
//    if (it != records.end()) {
//        records.erase(it, records.end());
//        return true;
//    }
//    return false;
//}
//
//template <typename T>
//std::vector<Registro> Bucket_EH<T>::search(const T& key) const {
//    std::vector<Registro> result;
//    for (const auto& record : records) {
//        if (record.key == key) {
//            result.push_back(record);
//        }
//    }
//    return result;
//}
//
//template <typename T>
//void Bucket_EH<T>::increaseDepth() {
//    localDepth++;
//}
//
//template <typename T>
//void Bucket_EH<T>::decreaseDepth() {
//    localDepth--;
//}
//
//template <typename T>
//int Bucket_EH<T>::getDepth() const {
//    return localDepth;
//}
//
//template <typename T>
//std::vector<Registro> Bucket_EH<T>::getRecords() const {
//    return records;
//}
//
//template <typename T>
//void Bucket_EH<T>::clear() {
//    records.clear();
//}
//
//template <typename T>
//void Bucket_EH<T>::redistribute(std::unordered_map<std::string, Bucket_EH<T>*>& directory) {
//    std::vector<Registro> oldRecords = records;
//    clear();
//    for (const auto& record : oldRecords) {
//        std::string newBucketIndex = std::bitset<sizeof(std::size_t) * 8>(std::hash<T>{}(record.key)).to_string();
//        newBucketIndex = newBucketIndex.substr(newBucketIndex.size() - localDepth);
//        directory[newBucketIndex]->add(record);
//    }
//}
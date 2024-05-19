#include <iostream>
#include <fstream>
#include <cstring>
#include <climits>
#include <vector>
#include <sstream>
using namespace std;

struct Record {
    int key;
    int pokedex;
    char name[50];
    char type1[20];
    char type2[20];
    int total;
    int hp;
    int attack;
    int defense;
    int sp_attack;
    int sp_defense;
    int speed;
    int generation;
    bool legendary;
    long left;
    long right;
    int height;

    Record() : key(0), pokedex(0), total(0), hp(0), attack(0), defense(0),
               sp_attack(0), sp_defense(0), speed(0), generation(0), legendary(false),
               left(-1), right(-1), height(0) {
        memset(name, 0, sizeof(name));
        memset(type1, 0, sizeof(type1));
        memset(type2, 0, sizeof(type2));
    }

    void showData() const {
        cout << key << " " << pokedex << " " << name << " " << type1 << " " << type2 << " "
             << total << " " << hp << " " << attack << " " << defense << " "
             << sp_attack << " " << sp_defense << " " << speed << " " << generation << " "
             << (legendary ? "TRUE" : "FALSE") << endl;
    }
};

class AVLFile {
private:
    string filename;
    long root_pos;

    void writeRootPosition() {
        fstream f(this->filename, ios::in | ios::out | ios::binary);
        f.seekp(0, ios::beg);
        f.write((char*)&root_pos, sizeof(long));
        f.close();
    }

public:
    AVLFile(string filename) {
        this->root_pos = -1;
        this->filename = filename;

        ifstream file(filename, ios::binary);
        if (file.good()) {
            file.seekg(0, ios::beg);
            file.read((char*)&root_pos, sizeof(long));
        } else {
            ofstream f(filename, ios::binary);
            f.write((char*)&root_pos, sizeof(long));
            f.close();
        }
        file.close();
    }

    Record find(int key) {
        ifstream file(filename, ios::binary);
        Record result = find(root_pos, key, file);
        file.close();
        return result;
    }

    void insert(Record record) {
        fstream f(this->filename, ios::in | ios::out | ios::binary);
        insert(root_pos, record, f);
        writeRootPosition();
        f.close();
    }

    template <class T>
    void remove(T key) {
        fstream f(this->filename, ios::in | ios::out | ios::binary);
        remove(root_pos, key, f);
        writeRootPosition();
        f.close();
    }

    vector<Record> inorder() {
        ifstream file(filename, ios::binary);
        vector<Record> result = inorder(root_pos, file);
        file.close();
        return result;
    }

    vector<Record> searchRange(int start, int end) {
        std::vector<Record> result;
        ifstream file(filename, ios::binary);
        searchRange(root_pos, start, end, result, file);
        file.close();
        return result;
    }

private:
    vector<Record> inorder(long pos_node, ifstream& file) {
        vector<Record> result;

        if (pos_node != -1) {
            Record record;
            file.seekg(pos_node, ios::beg);
            file.read((char*)&record, sizeof(Record));

            vector<Record> leftSubtree = inorder(record.left, file);
            result.insert(result.end(), leftSubtree.begin(), leftSubtree.end());

            result.push_back(record);

            vector<Record> rightSubtree = inorder(record.right, file);
            result.insert(result.end(), rightSubtree.begin(), rightSubtree.end());
        }

        return result;
    }

    Record find(long pos_node, int key, ifstream& file) {
        if (pos_node == -1) {
            Record notFoundRecord;
            notFoundRecord.key = -1;
            return notFoundRecord;
        }

        Record record;
        file.seekg(pos_node, ios::beg);
        file.read((char*)&record, sizeof(Record));

        if (record.key == key) {
            return record;
        } else if (key > record.key) {
            return find(record.right, key, file);
        } else {
            return find(record.left, key, file);
        }
    }

    void insert(long& pos_node, Record record, fstream& file) {
        if (pos_node == -1) {
            file.seekp(0, ios::end);
            pos_node = file.tellp();
            file.write((char*)&record, sizeof(Record));
            return;
        }

        Record curr_record;
        file.seekg(pos_node, ios::beg);
        file.read((char*)&curr_record, sizeof(Record));

        if (record.key < curr_record.key) {
            insert(curr_record.left, record, file);
        } else if (record.key > curr_record.key) {
            insert(curr_record.right, record, file);
        }

        file.seekp(pos_node, ios::beg);
        file.write((char*)&curr_record, sizeof(Record));

        updateHeight(pos_node, file);
        balance(pos_node, file);
    }

    template <class T>
    void remove(long& pos_node, T key, fstream& file) {
        if (pos_node == -1) return;

        Record curr_record;
        file.seekg(pos_node, ios::beg);
        file.read((char*)&curr_record, sizeof(Record));

        if (key < curr_record.key) {
            remove(curr_record.left, key, file);
        } else if (key > curr_record.key) {
            remove(curr_record.right, key, file);
        } else {
            if (curr_record.left == -1 && curr_record.right == -1) {
                pos_node = -1;
            } else if (curr_record.left == -1) {
                pos_node = curr_record.right;
            } else if (curr_record.right == -1) {
                pos_node = curr_record.left;
            } else {
                long min_larger_node = findMin(curr_record.right, file);
                Record min_larger_record;
                file.seekg(min_larger_node, ios::beg);
                file.read((char*)&min_larger_record, sizeof(Record));
                curr_record = min_larger_record;

                file.seekp(pos_node, ios::beg);
                file.write((char*)&curr_record, sizeof(Record));

                remove(curr_record.right, min_larger_record.key, file);
            }
        }

        if (pos_node != -1) {
            file.seekp(pos_node, ios::beg);
            file.write((char*)&curr_record, sizeof(Record));

            updateHeight(pos_node, file);
            balance(pos_node, file);
        }
    }

    long findMin(long pos_node, fstream& file) {
        Record record;
        file.seekg(pos_node, ios::beg);
        file.read((char*)&record, sizeof(Record));
        while (record.left != -1) {
            pos_node = record.left;
            file.seekg(pos_node, ios::beg);
            file.read((char*)&record, sizeof(Record));
        }
        return pos_node;
    }

    void searchRange(long pos_node, int start, int end, vector<Record>& result, ifstream& file) {
        if (pos_node == -1) return;

        Record curr_record;
        file.seekg(pos_node, ios::beg);
        file.read((char*)&curr_record, sizeof(Record));

        if (curr_record.key >= start && curr_record.key <= end) {
            result.push_back(curr_record);
            searchRange(curr_record.left, start, end, result, file);
            searchRange(curr_record.right, start, end, result, file);
        } else if (curr_record.key < start) {
            searchRange(curr_record.right, start, end, result, file);
        } else if (curr_record.key > end) {
            searchRange(curr_record.left, start, end, result, file);
        }
    }

    int height(long pos_node, fstream& file) {
        if (pos_node == -1) return -1;

        Record record;
        file.seekg(pos_node, ios::beg);
        file.read((char*)&record, sizeof(Record));
        return record.height;
    }

    void updateHeight(long pos_node, fstream& file) {
        if (pos_node == -1) return;

        Record record;
        file.seekg(pos_node, ios::beg);
        file.read((char*)&record, sizeof(Record));

        int left_height = height(record.left, file);
        int right_height = height(record.right, file);

        record.height = max(left_height, right_height) + 1;

        file.seekp(pos_node, ios::beg);
        file.write((char*)&record, sizeof(Record));
    }

    int balancingFactor(long pos_node, fstream& file) {
        if (pos_node == -1) return 0;

        Record record;
        file.seekg(pos_node, ios::beg);
        file.read((char*)&record, sizeof(Record));
        return height(record.left, file) - height(record.right, file);
    }

    void balance(long pos_node, fstream& file) {
        if (pos_node == -1) return;

        Record record;
        file.seekg(pos_node, ios::beg);
        file.read((char*)&record, sizeof(Record));

        if (balancingFactor(pos_node, file) == 2) {
            if (balancingFactor(record.left, file) < 0) {
                leftRotate(record.left, file);
            }
            rightRotate(pos_node, file);
        } else if (balancingFactor(pos_node, file) == -2) {
            if (balancingFactor(record.right, file) > 0) {
                rightRotate(record.right, file);
            }
            leftRotate(pos_node, file);
        }
        updateHeight(pos_node, file);
    }

    void leftRotate(long& pos_node, fstream& file) {
        Record record;
        file.seekg(pos_node, ios::beg);
        file.read((char*)&record, sizeof(Record));

        Record rightRecord;
        long rightPos = record.right;

        file.seekg(rightPos, ios::beg);
        file.read((char*)&rightRecord, sizeof(Record));

        record.right = rightRecord.left;
        rightRecord.left = pos_node;

        file.seekp(pos_node, ios::beg);
        file.write((char*)&record, sizeof(Record));

        pos_node = rightPos;
        file.seekp(pos_node, ios::beg);
        file.write((char*)&rightRecord, sizeof(Record));

        updateHeight(record.right, file);
        updateHeight(pos_node, file);
    }

    void rightRotate(long& pos_node, fstream& file) {
        Record record;
        file.seekg(pos_node, ios::beg);
        file.read((char*)&record, sizeof(Record));

        Record leftRecord;
        long leftPos = record.left;
        file.seekg(leftPos, ios::beg);
        file.read((char*)&leftRecord, sizeof(Record));

        record.left = leftRecord.right;
        leftRecord.right = pos_node;

        file.seekp(pos_node, ios::beg);
        file.write((char*)&record, sizeof(Record));

        pos_node = leftPos;
        file.seekp(pos_node, ios::beg);
        file.write((char*)&leftRecord, sizeof(Record));

        updateHeight(record.left, file);
        updateHeight(pos_node, file);
    }
};

void readCSVAndInsertToAVL(const string& csvFilename, const string& avlFilename) {
    ifstream csvFile(csvFilename);
    if (!csvFile.is_open()) {
        cerr << "Unable to open CSV file" << endl;
        return;
    }

    AVLFile avlFile(avlFilename);
    string line;

    // Skip the header line if it exists
    getline(csvFile, line);

    while (getline(csvFile, line)) {
        stringstream ss(line);
        string item;
        Record record;

        // Read all fields from the CSV
        getline(ss, item, ','); record.key = stoi(item);
        getline(ss, item, ','); record.pokedex = stoi(item);
        getline(ss, item, ','); strncpy(record.name, item.c_str(), sizeof(record.name) - 1); record.name[sizeof(record.name) - 1] = '\0';
        getline(ss, item, ','); strncpy(record.type1, item.c_str(), sizeof(record.type1) - 1); record.type1[sizeof(record.type1) - 1] = '\0';
        getline(ss, item, ','); strncpy(record.type2, item.c_str(), sizeof(record.type2) - 1); record.type2[sizeof(record.type2) - 1] = '\0';
        getline(ss, item, ','); record.total = stoi(item);
        getline(ss, item, ','); record.hp = stoi(item);
        getline(ss, item, ','); record.attack = stoi(item);
        getline(ss, item, ','); record.defense = stoi(item);
        getline(ss, item, ','); record.sp_attack = stoi(item);
        getline(ss, item, ','); record.sp_defense = stoi(item);
        getline(ss, item, ','); record.speed = stoi(item);
        getline(ss, item, ','); record.generation = stoi(item);
        getline(ss, item, ','); record.legendary = (item == "TRUE");

        avlFile.insert(record);
    }
    csvFile.close();
}

int main() {
    string csvFilename = "pokemon.csv"; // Path to the CSV file
    string avlFilename = "data.bin";

    readCSVAndInsertToAVL(csvFilename, avlFilename);

    AVLFile file(avlFilename);
    vector<Record> records = file.inorder();
    for (const auto& record : records) {
        record.showData();
    }

    return 0;
}

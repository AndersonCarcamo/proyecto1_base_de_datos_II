#include <iostream>
#include <fstream>
#include <sstream>
#include "ExtendibleHashing.hh" // Asegúrate de que el nombre del archivo coincida con tu implementación

using namespace std;

// Función para cargar datos desde un archivo CSV a la estructura de Extendible Hashing
void loadCSVToExtendibleHashing(directory_EH<int>& hashTable, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    vector<string> columns;
    bool firstLine = true;

    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> values;

        while (getline(ss, token, ',')) {
            values.push_back(token);
        }

        if (firstLine) {
            columns = values;
            firstLine = false;
        } else {
            Registro record;
            for (size_t i = 0; i < values.size(); ++i) {
                record.setField(columns[i], values[i]); // Asigna los campos al registro
            }
            hashTable.add(record); // Agrega el registro al Extendible Hashing
        }
    }

    file.close();
}

int main() {
    // Configuración inicial del directorio de Extendible Hashing
    int globalDepth = 11;  // Profundidad global inicial
    int size_bucket = 4;   // Tamaño máximo del bucket

    directory_EH<int> hashTable(globalDepth, size_bucket);

    // Carga de datos desde un archivo CSV
    //string filename = "prueba1.csv";

    string filename = "prueba1.csv";
    loadCSVToExtendibleHashing(hashTable, filename);

    // Operaciones de prueba: search, insert y delete
    int keyToSearch = 80;  // Ejemplo de clave a buscar
    vector<Registro> result = hashTable.search(keyToSearch);
    cout << "Resultados de la búsqueda para la clave " << keyToSearch << ":" << endl;
    for (const auto& record : result) {
        // Aquí puedes imprimir o procesar los registros encontrados
        cout << "ID: " << record.ID << ", Organization: " << record.Organization << endl;
    }

    // Ejemplo de inserción de un nuevo registro
    Registro newRecord(789, "2024-05-18", "123456789", "NewOrg", "NewOLF", 5, 100, "NewCategory", "NewCountry", "NewCountryCode",
                       "NewState", "NewCity", "NewStreet", "NewBuilding");
    if (hashTable.add(newRecord)) {
        cout << "Nuevo registro agregado exitosamente." << endl;
    } else {
        cout << "No se pudo agregar el nuevo registro (posible bucket lleno)." << endl;
    }

    // Ejemplo de eliminación de un registro por clave
    int keyToDelete = 789;  // Clave del registro a eliminar
    if (hashTable.remove(keyToDelete)) {
        cout << "Registro con clave " << keyToDelete << " eliminado exitosamente." << endl;
    } else {
        cout << "No se encontró registro con clave " << keyToDelete << " para eliminar." << endl;
    }

    // Guardar cambios en los archivos (opcional)
    hashTable.save();

    return 0;
}

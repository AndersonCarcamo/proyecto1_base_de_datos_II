#include <iostream>

#include "./extendiblehashing/ExtendibleHashing.hh"

int main(){
    directory_EH<int> dir(2, 2);

    // Insertar algunos registros
    dir.add(Registro(1, "data1"));
    dir.add(Registro(2, "data2"));
    dir.add(Registro(3, "data3"));
    dir.add(Registro(4, "data4"));

    // Buscar registros
    vector<Registro> result = dir.search(2);
    for (const auto& reg : result) {
        cout << "Found: key=" << reg.key << ", data=" << reg.data << endl;
    }

    // Verificar si se realiza la división de buckets correctamente
    dir.add(Registro(5, "data5"));
    dir.add(Registro(6, "data6"));

    // Buscar registros nuevamente
    result = dir.search(5);
    for (const auto& reg : result) {
        cout << "Found: key=" << reg.key << ", data=" << reg.data << endl;
    }

    // Eliminar un registro y verificar
    if (dir.remove(2)) {
        cout << "Registro con clave 2 eliminado correctamente." << endl;
    }

    result = dir.search(2);
    if (result.empty()) {
        cout << "No se encontró el registro con clave 2 (esperado)." << endl;
    } else {
        for (const auto& reg : result) {
            cout << "Found: key=" << reg.key << ", data=" << reg.data << endl;
        }
    }

    return 0;
}
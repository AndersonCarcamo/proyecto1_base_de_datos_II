# Proyecto 1 Base de Datos 2
# Efficient Data Structures for File Organization

[![VIDEO PROYECTO 1](foto.png)](https://www.youtube.com/watch?v=JfH7mxtyovg)


## INTEGRANTES:
- Anderson Carcamo
- Sebastian Chu [@ChuSebastian](https://github.com/ChuSebastian)
- Cristopher Meneses


## 1. OBJETIVO

Este proyecto se centra en el desarrollo e implementación de técnicas avanzadas para la organización de archivos en memoria secundaria. Además, se incluirá un parser de código SQL que facilitará la ejecución de sentencias SQL, mejorando así la interacción y manipulación de los datos almacenados. Las estructuras elegidas son:

- AVL file
- ISAM (sebastian chu)
- Extendible Hashing

### Objetivo Principal
La organización física de archivos en memoria secundaria es crucial para el diseño y operación eficiente de los sistemas de gestión de bases de datos (DBMS). En un mundo cada vez más impulsado por la información, es esencial que los archivos en memoria secundaria estén estructurados de manera que aseguren la integridad, disponibilidad y rendimiento óptimo de los datos. Este proyecto tiene como propósito aplicar varias técnicas de organización de archivos, incluyendo Sequential File Organization, AVL File Organization y Extendible Hashing, para gestionar eficazmente los registros almacenados en memoria secundaria.

### Objetivos Secundarios
1. **Desarrollo de Operaciones Eficientes**: Crear una interfaz gráfica de usuario (GUI) que permita realizar consultas SQL de manera eficiente y gestionar registros de forma intuitiva.

2. **Evaluación y Comparación**: Analizar y comparar las ventajas y desventajas de cada técnica de organización de archivos implementada, destacando su rendimiento, aplicabilidad y eficiencia en diferentes contextos.

3. **Optimización del Rendimiento**: Implementar estrategias para optimizar el tiempo de acceso y la gestión de los archivos, mejorando el rendimiento general del sistema.

4. **Mantenimiento de la Integridad de los Datos**: Asegurar que las técnicas implementadas mantengan la integridad y consistencia de los datos durante las operaciones de inserción, eliminación y actualización.

5. **Escalabilidad**: Evaluar cómo cada técnica se adapta al crecimiento de la base de datos y su capacidad para manejar grandes volúmenes de datos sin degradar el rendimiento.

## 2. BASE DE DATOS
# Descripción de la Base de Datos

Esta base de datos contiene información detallada sobre varias especies de Pokémon, abarcando diferentes generaciones y formas. A continuación, se describen los campos incluidos en la base de datos y se presentan algunos ejemplos de los registros:

## Campos de la Base de Datos

1. **key**: Identificador único para cada entrada en la base de datos.
2. **pokedex**: Número de la Pokédex nacional del Pokémon.
3. **name**: Nombre del Pokémon.
4. **type1**: Tipo principal del Pokémon.
5. **type2**: Tipo secundario del Pokémon (si tiene uno).
6. **total**: Suma total de todas las estadísticas base del Pokémon.
7. **hp**: Puntos de salud (Hit Points) del Pokémon.
8. **attack**: Estadística de ataque del Pokémon.
9. **defense**: Estadística de defensa del Pokémon.
10. **sp_attack**: Estadística de ataque especial del Pokémon.
11. **sp_defense**: Estadística de defensa especial del Pokémon.
12. **speed**: Estadística de velocidad del Pokémon.
13. **generation**: Generación a la que pertenece el Pokémon.
14. **legendary**: Indica si el Pokémon es legendario (TRUE o FALSE).

## Ejemplos de Registros

1. **Bulbasaur**
   - key: 1
   - pokedex: 1
   - name: Bulbasaur
   - type1: Grass
   - type2: Poison
   - total: 318
   - hp: 45
   - attack: 49
   - defense: 49
   - sp_attack: 65
   - sp_defense: 65
   - speed: 45
   - generation: 1
   - legendary: FALSE

2. **Charizard**
   - key: 8
   - pokedex: 6
   - name: Charizard
   - type1: Fire
   - type2: Flying
   - total: 534
   - hp: 78
   - attack: 84
   - defense: 78
   - sp_attack: 109
   - sp_defense: 85
   - speed: 100
   - generation: 1
   - legendary: FALSE


## 3. IMPLEMENTACIÓN DE ESTRUCTURAS

### 3.1 EXTENDIBLE HASHING

Para el extendible hashing se han implementando los metodos:

1. **Search(key)**

   El método `search` en la clase `directory_EH` facilita la búsqueda de registros mediante una estructura de hashing extendido. Su funcianamiento se divide en:

   1. **Obtención del Índice de Bucket**:
      - Utiliza `getBucketIndex(key)` para calcular el índice del bucket según la clave `key`.

   2. **Búsqueda en el Directorio**:
      - Busca en el `directory` utilizando el índice obtenido para encontrar el `Bucket_EH` correspondiente.

   3. **Búsqueda en el Bucket**:
      - Si encuentra el `Bucket_EH`, invoca `search(key)` para obtener los registros que coinciden con `key`.

   4. **Retorno de Resultados**:
      - Retorna los registros encontrados o un vector vacío si no hay coincidencias.

   #### Pseudocódigo:

   ```pseudo
   Método search(key):
       bucketIndex = getBucketIndex(key)
       bucket = directory[bucketIndex]
       
       si bucket existe entonces:
           registros = bucket.search(key)
           devolver registros
       sino:
           devolver vector vacío
   ```
2. **bool add(Registro registro)**

   El metodo add añade un registro al bucket. Su funcionamiento se divide en:

   1. **Obtención del Índice de Bucket**:
      - Utiliza `getBucketIndex(registro.key)` para calcular el índice del bucket donde se añadirá el nuevo registro.

   2. **Obtención del Bucket**:
      - Accede al `Bucket_EH` correspondiente en el `directory` usando el índice calculado.

   3. **Verificación de Capacidad del Bucket**:
      - Verifica si el `Bucket_EH` no está lleno llamando a `isFull()`.

   4. **Manejo de Caso Lleno**:
      - Si el bucket está lleno, invoca `splitBucket(bucketIndex)` para dividir el bucket y luego intenta agregar el registro nuevamente de forma recursiva.

   5. **Añadir Registro**:
      - Si el bucket tiene capacidad, añade el registro usando `add(registro)` del `Bucket_EH`.

   6. **Retorno de Éxito o Fracaso**:
      - Retorna `true` si el registro se añadió correctamente, `false` si no se pudo añadir debido a limitaciones de capacidad.

   #### Pseudocódigo:

   ```pseudo
   Método add(registro):
       bucketIndex = getBucketIndex(registro.key)
       bucket = directory[bucketIndex]
       
       si bucket no está lleno entonces:
           si bucket.add(registro) es true entonces:
               devolver true
           sino:
               devolver false
       
       sino:
           splitBucket(bucketIndex)
           devolver add(registro)
   ```
3. **rangeSearch(T begin-key, T end-key)**

   Al estar implementando con un hash, el range search no es optimo para su implementación, además que no hay un orden

4. **remove(T key)**

   Elimina del bucket el registros con llave key. Su funcionameinto se divide en:
   1. **Obtención del Índice de Bucket**:
      - Utiliza `getBucketIndex(key)` para calcular el índice del bucket donde se encuentra el registro a eliminar.

   2. **Búsqueda del Bucket**:
      - Busca el bucket correspondiente en el `directory` usando el índice calculado.

   3. **Eliminación del Registro**:
      - Si el bucket es encontrado, invoca `remove(key)` del `Bucket_EH` para eliminar el registro identificado por la clave.

   4. **Retorno de Resultado**:
      - Retorna `true` si se eliminó el registro correctamente, `false` si el bucket no se encontró o si no se pudo realizar la eliminación.

   #### Pseudocódigo:

   ```pseudo
   Método remove(key):
       bucketIndex = getBucketIndex(key)
       bucket = directory[bucketIndex]
       
       si bucket es encontrado entonces:
           devolver bucket.remove(key)
       
       sino:
           devolver false
   ```        

### 3.2 ISAM

### 3.2 AVL
#AVL Tree File Handling for Pokémon Records

![Pokémon Records](arbolavl.jpg)

Este proyecto implementa un árbol AVL para manejar registros de Pokémon almacenados en un archivo binario. Los registros son leídos de un archivo CSV y se almacenan en un archivo binario, manteniendo un árbol AVL para permitir búsquedas eficientes, inserciones y eliminaciones.

## Introducción

Un árbol AVL es un tipo de árbol binario de búsqueda auto-balanceado, nombrado así en honor a sus inventores Adelson-Velsky y Landis. La principal característica de un árbol AVL es que para cada nodo, la diferencia de alturas entre sus subárboles izquierdo y derecho no puede ser mayor que uno. Esta propiedad de balanceo se mantiene mediante la realización de rotaciones durante las operaciones de inserción y eliminación.

### Rotaciones en Árboles AVL

1. **Rotación Simple a la Derecha:** Se usa cuando el subárbol izquierdo de un nodo se desequilibra hacia la izquierda.
2. **Rotación Simple a la Izquierda:** Se usa cuando el subárbol derecho de un nodo se desequilibra hacia la derecha.
3. **Rotación Doble a la Izquierda-Derecha:** Se usa cuando el subárbol izquierdo de un nodo se desequilibra hacia la derecha.
4. **Rotación Doble a la Derecha-Izquierda:** Se usa cuando el subárbol derecho de un nodo se desequilibra hacia la izquierda.

## Técnica Utilizada

El proceso del proyecto se puede dividir en varias etapas clave:

1. **Lectura del Archivo CSV:** Los registros de Pokémon se leen de un archivo CSV que contiene información estructurada de cada Pokémon. Esta información incluye atributos como el nombre, tipo, estadísticas y otros detalles relevantes.

2. **Almacenamiento en Archivo Binario:** Los registros leídos se almacenan en un archivo binario. Esto permite un acceso rápido y eficiente a los datos, además de reducir el espacio de almacenamiento comparado con un archivo de texto.

3. **Manejo del Árbol AVL:**
   - **Inserción:** Cada vez que un nuevo registro de Pokémon se añade al archivo binario, se inserta en el árbol AVL. Durante la inserción, se realizan las rotaciones necesarias para mantener el equilibrio del árbol.
   - **Búsqueda:** Para encontrar un registro específico, el árbol AVL permite una búsqueda eficiente al reducir el número de comparaciones necesarias.
   - **Eliminación:** Similar a la inserción, la eliminación de un registro requiere ajustar el árbol para mantener su equilibrio, lo que puede implicar varias rotaciones.

4. **Actualización del Archivo Binario:** Al eliminar o modificar registros, se actualiza el archivo binario para reflejar los cambios. El árbol AVL se ajusta en consecuencia para mantener su estructura balanceada.


## Estructura de Datos

### Estructura `Record`

La estructura `Record` almacena los datos de un Pokémon:

```cpp
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
````
### Métodos
find(int key)

Busca un registro en el árbol AVL basado en la clave proporcionada.
```cpp
 Record find(int key) {
    ifstream file(filename, ios::binary);
    Record result = find(root_pos, key, file);
    file.close();
    return result;
}
```

insert(Record record)

Inserta un nuevo registro en el árbol AVL. El método se asegura de que el árbol permanezca balanceado después de la inserción.
```cpp
void insert(Record record) {
    fstream f(this->filename, ios::in | ios::out | ios::binary);
    insert(root_pos, record, f);
    writeRootPosition();
    f.close();
}
```
remove(int key)

Elimina un registro del árbol AVL basado en la clave proporcionada. El método ajusta el árbol para mantener el balance después de la eliminación.
```cpp
template <class T>
void remove(T key) {
    fstream f(this->filename, ios::in | ios::out | ios::binary);
    remove(root_pos, key, f);
    writeRootPosition();
    f.close();
}
```
searchRange(int start, int end)

Realiza una búsqueda de rango en el árbol AVL, devolviendo todos los registros cuyas claves están dentro del rango especificado.
```cpp
vector<Record> searchRange(int start, int end) {
    std::vector<Record> result;
    ifstream file(filename, ios::binary);
    searchRange(root_pos, start, end, result, file);
    file.close();
    return result;
}
```
###Funciones Internas

####Remove
El método remove elimina un registro del árbol AVL. Su funcionamiento se divide en:

1. Obtención del Nodo: Busca el nodo con la clave proporcionada.
2. Eliminación del Registro: Si el nodo es encontrado, se elimina y el árbol se rebalancea.
3. Rebalanceo: Ajusta el árbol para mantener las propiedades del árbol AVL.

```cpp
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
```
###Insert
El método insert añade un nuevo registro al árbol AVL, ajustando el árbol para mantener el balance.
```cpp
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
```
###SearchRange
El método searchRange realiza una búsqueda de todos los registros cuyas claves están dentro de un rango especificado.
```cpp
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
```


## 4. Parser

El parser se ha realizado con flex y bison. Esto requiere solo un análisis de los token requeridos para las consultas sql.
Los archivos sql_parser.l genera el Scanner.cpp y Scanner.hpp. En este lado se implementa los tokens y los tipos de datos usados
.


| TOKENS                      | RESPUESTA                                                              |
|-----------------------------|-------------------------------------------------------------------------|
| "create"                    | return Parser::token::CREATE;                                           |
| "table"                     | return Parser::token::TABLE;                                            |
| "from"                      | return Parser::token::FROM;                                             |
| "file"                      | return Parser::token::FILE;                                             |
| "using"                     | return Parser::token::USING;                                            |
| "index"                     | return Parser::token::INDEX;                                            |
| "select"                    | return Parser::token::SELECT;                                           |
| "where"                     | return Parser::token::WHERE;                                            |
| "between"                   | return Parser::token::BETWEEN;                                          |
| "insert"                    | return Parser::token::INSERT;                                           |
| "into"                      | return Parser::token::INTO;                                             |
| "values"                    | return Parser::token::VALUES;                                           |
| "delete"                    | return Parser::token::DELETE;                                           |
| "*"                         | return Parser::token::MULT;                                             |
| "("                         | return Parser::token::LPAREN;                                           |
| ")"                         | return Parser::token::RPAREN;                                           |
| "="                         | return Parser::token::EQ;                                               |
| ","                         | return Parser::token::COMMA;                                            |
| \[A-Za-z_\]\[A-Za-z_0-9\]\* | return Parser::token::IDENTIFIER;                                |

Ahora la grámatica independiente del contexto implementada sería:


Con esta gramtica debería poder aceptar un create a partir de un archivo.csv, insert, remove y un search:
Los parametros son:

- **command_list:** Lista de comandos.
- **command:** Comandos específicos como create_table_command, select_command, insert_command, delete_command.
- **create_table_command:** Comando para crear tablas especificando origen de datos y uso de índice.
- **select_command:** Comando para seleccionar datos con posibilidad de filtrado.
- **where_clause:** Cláusula opcional para especificar condiciones de filtro en SELECT y DELETE.
- **insert_command:** Comando para insertar datos en una tabla especificada.
- **value_list:** Lista de valores para inserción.
- **value:** Valores literales (cadenas o enteros).
- **delete_command:** Comando para eliminar datos basado en condiciones.

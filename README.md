# Proyecto 1 Base de Datos 2
# Efficient Data Structures for File Organization

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



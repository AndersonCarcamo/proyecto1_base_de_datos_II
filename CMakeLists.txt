cmake_minimum_required(VERSION 3.26)
project(proyecto1_base_de_datos_II)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(proyecto1_base_de_datos_II
        extendiblehashing/ExtendibleHashing.hh
        extendiblehashing/ExtendibleHashing.cpp
        extendiblehashing/Bucket_EH.cpp
        extendiblehashing/Bucket_EH.hh
        main.cpp)

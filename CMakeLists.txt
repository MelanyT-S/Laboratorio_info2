cmake_minimum_required(VERSION 3.16)

project(practica_3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(practica_3 main.cpp
    codificaciones.h codificaciones.cpp)

include(GNUInstallDirs)
install(TARGETS practica_3
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

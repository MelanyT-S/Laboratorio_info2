#include <iostream>
#include "integracion.h"

void probarPunto54() {
    std::string archivoEntrada, archivoSalida;
    int opcionMetodo, clave;

    std::cout << "\n--- OPCIONES DEL PUNTO 5.4 ---\n";
    std::cout << "Ingrese la ruta del archivo de entrada (ej: entrada.txt): ";
    std::cin >> archivoEntrada;

    std::cout << "Ingrese la ruta para guardar el archivo final (ej: salida.txt): ";
    std::cin >> archivoSalida;

    std::cout << "Seleccione el metodo de compresion:\n";
    std::cout << "1. RLE\n";
    std::cout << "2. LZ78\n";
    std::cout << "Opcion (1 o 2): ";
    std::cin >> opcionMetodo;

    std::cout << "Ingrese la clave numerica de encriptacion (K): ";
    std::cin >> clave;

    MetodoCompresion metodo = (opcionMetodo == 2) ? LZ78_METODO : RLE_METODO;

    // Ejecutamos el flujo de integracion
    ejecutarIntegracion(archivoEntrada, archivoSalida, metodo, clave);
}

int main() {
    int opcion = 0;
    do {
        std::cout << "\n=========================================\n";
        std::cout << "        PRACTICA 3 - MENU PRINCIPAL      \n";
        std::cout << "=========================================\n";
        std::cout << "1. Probar Punto 5.1 (RLE)\n";
        std::cout << "2. Probar Punto 5.2 (LZ78)\n";
        std::cout << "3. Probar Punto 5.3 (Encriptacion Bits)\n";
        std::cout << "4. Probar Punto 5.4 (Integracion Completa)\n";
        std::cout << "5. Salir\n";
        std::cout << "Seleccione una opcion (1-5): ";
        std::cin >> opcion;

        switch (opcion) {
        case 4:
            probarPunto54();
            break;
        case 5:
            std::cout << "Saliendo del programa...\n";
            break;
        default:
            std::cout << "Opcion no valida.\n";
            break;
        }
    } while (opcion != 5);

    return 0;
}
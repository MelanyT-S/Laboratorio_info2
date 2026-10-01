#include "integracion.h"
#include "codificaciones.h" // Incluimos los módulos previos (RLE, LZ78, Encriptación)
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept> // Necesario para el manejo de excepciones

// Función auxiliar para leer todo el texto de un archivo
static std::string leerArchivo(const std::string& ruta) {
    std::ifstream archivo(ruta, std::ios::in | std::ios::binary);

    // Lanzamos excepción si el archivo no existe o no se puede abrir
    if (!archivo.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo de entrada: " + ruta);
    }

    std::stringstream buffer;
    buffer << archivo.rdbuf(); // Lee el archivo completo en un buffer
    archivo.close();
    return buffer.str();
}

// Función auxiliar para escribir un texto en un archivo
static void escribirArchivo(const std::string& ruta, const std::string& contenido) {
    std::ofstream archivo(ruta, std::ios::out | std::ios::binary);

    // Lanzamos excepción si hay un error al crear o escribir el archivo
    if (!archivo.is_open()) {
        throw std::runtime_error("No se pudo crear/escribir en el archivo de salida: " + ruta);
    }

    archivo << contenido;
    archivo.close();
}

bool ejecutarIntegracion(const std::string& rutaOriginal,
                         const std::string& rutaSalida,
                         MetodoCompresion metodo,
                         int claveEncriptacion) {
    try {

        std::cout << "      PUNTO 5.4: INTEGRACION DE MODULOS\n";

        // PASO 1: Leer el texto original desde el archivo
        std::cout << "[1/5] Leyendo archivo original: " << rutaOriginal << "...\n";
        std::string textoOriginal = leerArchivo(rutaOriginal);
        std::cout << "      -> Tamano original: " << textoOriginal.length() << " bytes.\n";

        // PASO 2: Seleccionar y aplicar método de compresión (RLE o LZ78)
        std::string textoComprimido = "";
        SalidaLZ78 salidaLZ = {nullptr, 0}; // Variable para almacenar la estructura de LZ78

        if (metodo == RLE_METODO) {
            std::cout << "[2/5] Comprimiendo contenido con RLE...\n";
            textoComprimido = comprimirRLE(textoOriginal);
        } else if (metodo == LZ78_METODO) {
            std::cout << "[2/5] Comprimiendo contenido con LZ78...\n";

            // 1. Llamamos a comprimirLZ78 pasando el char* y guardando la estructura SalidaLZ78
            salidaLZ = comprimirLZ78(textoOriginal.c_str());

            // 2. Para poder encriptar los datos, convertimos los pares LZ78 a texto plano de manera temporal
            for (int k = 0; k < salidaLZ.cantidad; k++) {
                textoComprimido += "(" + std::to_string(salidaLZ.pares[k].prefijo) + "," + salidaLZ.pares[k].caracter + ")";
            }
        } else {
            throw std::invalid_argument("Metodo de compresion invalido.");
        }
        // MUESTRA EN PANTALLA EL TEXTO COMPRIMIDO
        std::cout << "      -> Texto Comprimido: \"" << textoComprimido << "\"\n";

        // PASO 3: Encriptar el resultado carácter por carácter
        std::cout << "[3/5] Encriptando el texto comprimido...\n";
        std::string textoEncriptado = "";
        for (char c : textoComprimido) {
            // Se pasan los 3 parámetros: el byte, el desplazamiento n y el carácter K
            textoEncriptado += encriptarByte(static_cast<unsigned char>(c), claveEncriptacion, 'K');
        }
        // MUESTRA EN PANTALLA EL TEXTO ENCRIPTADO (SE VERÁ COMO SIMBOLOS/GARABATOS)
        std::cout << "      -> Texto Encriptado (en memoria): \"" << textoEncriptado << "\"\n";

        // PASO 4: Proceso Inverso (Desencriptar mensaje carácter por carácter)
        std::cout << "[4/5] Desencriptando el mensaje...\n";
        std::string textoDesencriptado = "";
        for (char c : textoEncriptado) {
            // Se pasan los 3 parámetros en el mismo orden
            textoDesencriptado += desencriptarByte(static_cast<unsigned char>(c), claveEncriptacion, 'K');
        }
        // MUESTRA EN PANTALLA EL TEXTO DESENCRIPTADO (DEBE VOLVER A SER EL COMPRIMIDO)
        std::cout << "      -> Texto Desencriptado: \"" << textoDesencriptado << "\"\n";

        std::string textoRestaurado = "";
        if (metodo == RLE_METODO) {
            textoRestaurado = descomprimirRLE(textoDesencriptado);
        } else if (metodo == LZ78_METODO) {
            // 1. Descomprimimos usando directamente la estructura SalidaLZ78 que exige la función[cite: 9]
            char* textoDescomprimidoPtr = descomprimirLZ78(salidaLZ);

            // 2. Lo asignamos a nuestra variable de resultado
            textoRestaurado = std::string(textoDescomprimidoPtr);

            // 3. IMPORTANTE: Liberamos la memoria dinámica usando la función que creaste[cite: 9]
            delete[] textoDescomprimidoPtr; // Liberamos el buffer devuelto por descomprimirLZ78
            liberarSalidaLZ78(salidaLZ);     // Liberamos los pares en el Heap[cite: 9]
        }
        // MUESTRA EN PANTALLA EL TEXTO FINAL RESTAURADO
        std::cout << "      -> Texto Restaurado Final: \"" << textoRestaurado << "\"\n";

        // PASO 5: Guardar el texto final en el archivo de salida[cite: 7]
        std::cout << "[5/5] Escribiendo texto recuperado en el archivo: " << rutaSalida << "...\n";
        escribirArchivo(rutaSalida, textoRestaurado);

        // VERIFICACION: Comprobar si el texto final coincide con el original[cite: 7]
        std::cout << "\n--------------------------------------------\n";
        std::cout << "VERIFICACION DE INTEGRIDAD:\n";
        if (textoOriginal == textoRestaurado) {
            std::cout << "[EXITO] El texto final coincide PERFECTAMENTE con el original.\n";
            std::cout << "--------------------------------------------\n";
            return true;
        } else {
            std::cout << "[ERROR] El texto recuperado NO coincide con el original.\n";
            std::cout << "--------------------------------------------\n";
            return false;
        }

    }
    // MANEJO DE EXCEPCIONES: Atrapa cualquier error durante la ejecucion[cite: 7]
    catch (const std::exception& e) {
        std::cerr << "\n[EXCEPCION ATRAPADA]: " << e.what() << std::endl;
        return false;
    }
}
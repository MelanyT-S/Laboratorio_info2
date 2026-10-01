#ifndef INTEGRACION_H
#define INTEGRACION_H

#include <string>

// Enum para seleccionar el método de compresión de forma clara
enum MetodoCompresion {
    RLE_METODO = 1,
    LZ78_METODO = 2
};

// Prototipo de la función principal de integración del punto 5.4
// Recibe las rutas de archivos, el método seleccionado y la clave para la encriptación
bool ejecutarIntegracion(const std::string& rutaOriginal,
                         const std::string& rutaSalida,
                         MetodoCompresion metodo,
                         int nBits,
                         int claveEncriptacion);

#endif // INTEGRACION_H
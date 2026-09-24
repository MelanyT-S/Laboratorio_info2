#ifndef CODIFICACIONES_H
#define CODIFICACIONES_H

#include <string>

// MÓDULO 5.1: COMPRESIÓN Y DESCOMPRESIÓN RLE

// Comprime una cadena contando repeticiones consecutivas (ej: "AAAABBB" -> "4A3B")
std::string comprimirRLE(const std::string& texto);

// Descomprime una cadena en formato RLE reconstruyendo el texto original
std::string descomprimirRLE(const std::string& comprimido);


// MÓDULO 5.2: COMPRESIÓN Y DESCOMPRESIÓN LZ78

// Estructura para representar el par conceptual (índice, carácter)
struct ParLZ78 {
    int prefijo;    // Índice de la frase previamente almacenada (0 si es nueva)
    char caracter;  // Carácter que extiende la frase
};

// Estructura para almacenar el arreglo dinámico de pares resultantes
struct SalidaLZ78 {
    ParLZ78* pares; // Puntero al bloque de memoria dinámica en el Heap
    int cantidad;   // Cantidad total de pares generados
};

// Comprime un texto usando el algoritmo LZ78 y diccionario dinámico manual
SalidaLZ78 comprimirLZ78(const char* texto);

// Descomprime los pares LZ78 para recuperar exactamente el texto inicial
char* descomprimirLZ78(SalidaLZ78 salida);

// Función auxiliar para liberar la memoria del arreglo de pares
void liberarSalidaLZ78(SalidaLZ78& salida);



// MÓDULO 5.3: ENCRIPTACIÓN Y DESENCRIPTACIÓN DE BITS
// (Rotación circular de bits + Operación XOR)

// Desplaza circularmente los bits de un byte a la izquierda 'n' posiciones
unsigned char rotarIzquierda(unsigned char byte, int n);

// Desplaza circularmente los bits de un byte a la derecha 'n' posiciones
unsigned char rotarDerecha(unsigned char byte, int n);

// Encripta un byte: aplica rotación izquierda por 'n' bits y luego XOR con carácter K
unsigned char encriptarByte(unsigned char byte, int n, unsigned char K);

// Desencripta un byte: aplica XOR con carácter K y luego rotación derecha por 'n' bits
unsigned char desencriptarByte(unsigned char byte, int n, unsigned char K);

#endif // CODIFICACIONES_H
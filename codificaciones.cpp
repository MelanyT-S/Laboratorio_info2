#include "codificaciones.h"
#include <cctype>   // Para std::isdigit (verificar si un carácter es número)
#include <cstring>  // Para strlen (medir tamaño de arreglos char*)

// IMPLEMENTACIÓN MÓDULO 5.1: RLE

std::string comprimirRLE(const std::string& texto) {
    std::string comprimido = "";
    int n = texto.length();

    for (int i = 0; i < n; i++) {
        int contador = 1;

        // Avanzamos y contamos cuántas veces se repite la misma letra de forma continua
        while (i + 1 < n && texto[i] == texto[i + 1]) {
            contador++;
            i++;
        }

        // Guardamos la cantidad convertida a texto seguida del carácter
        comprimido += std::to_string(contador) + texto[i];
    }
    return comprimido;
}

std::string descomprimirRLE(const std::string& comprimido) {
    std::string resultado = "";
    int n = comprimido.length();
    int i = 0;

    while (i < n) {
        int contador = 0;

        // Extraemos el número
        while (i < n && std::isdigit(comprimido[i])) {
            contador = contador * 10 + (comprimido[i] - '0');
            i++;
        }

        // Repetimos el carácter tantas veces como indicó el contador
        if (i < n) {
            resultado.append(contador, comprimido[i]);
            i++;
        }
    }
    return resultado;
}

// IMPLEMENTACIÓN MÓDULO 5.2: LZ78

// Estructura interna para gestionar el diccionario en memoria dinámica
struct DiccionarioLZ78 {
    ParLZ78* entradas; // Arreglo dinamico reservado en el Heap
    int capacidad;     // Tamaño fisico reservado actualmente
    int tamano;        // Cantidad de elementos guardados

    // Constructor: Inicializa la memoria con un tamaño por defecto
    DiccionarioLZ78(int cap = 10) {
        capacidad = cap;
        tamano = 0;
        entradas = new ParLZ78[capacidad];
    }

    // Destructor: Garantiza la liberacion automatica de la memoria al terminar
    ~DiccionarioLZ78() {
        delete[] entradas;
    }

    // Redimensiona el arreglo duplicando su capacidad cuando se llena
    void redimensionar() {
        capacidad *= 2;
        ParLZ78* nuevo = new ParLZ78[capacidad];
        for (int i = 0; i < tamano; i++) {
            nuevo[i] = entradas[i];
        }
        delete[] entradas; // Liberamos el bloque viejo
        entradas = nuevo;  // Apuntamos al nuevo bloque en memoria
    }

    // Inserta un nuevo par (prefijo, caracter) al diccionario
    void agregar(int prefijo, char caracter) {
        if (tamano == capacidad) {
            redimensionar();
        }
        entradas[tamano].prefijo = prefijo;
        entradas[tamano].caracter = caracter;
        tamano++;
    }

    // Busca si ya existe la combinación (prefijo, carácter). Retorna índice (basado en 1) o 0
    int buscar(int prefijo, char caracter) {
        for (int i = 0; i < tamano; i++) {
            if (entradas[i].prefijo == prefijo && entradas[i].caracter == caracter) {
                return i + 1; // Basado en 1 para usar el 0 como "cadena vacía"
            }
        }
        return 0;
    }
};

SalidaLZ78 comprimirLZ78(const char* texto) {
    int len = strlen(texto);
    DiccionarioLZ78 dict;

    int capPares = 10;
    int cantPares = 0;
    ParLZ78* pares = new ParLZ78[capPares];

    int prefijoActual = 0;

    for (int i = 0; i < len; i++) {
        char c = texto[i];
        int indiceEncontrado = dict.buscar(prefijoActual, c);

        if (indiceEncontrado != 0) {
            // Si la subcadena ya existe en el diccionario, extendemos el prefijo
            prefijoActual = indiceEncontrado;
        } else {
            // Si no existe, emitimos el par, lo guardamos en el diccionario y reiniciamos
            if (cantPares == capPares) {
                capPares *= 2;
                ParLZ78* nuevo = new ParLZ78[capPares];
                for (int k = 0; k < cantPares; k++) nuevo[k] = pares[k];
                delete[] pares;
                pares = nuevo;
            }
            pares[cantPares++] = {prefijoActual, c};
            dict.agregar(prefijoActual, c);
            prefijoActual = 0;
        }
    }

    // Si al terminar de leer el texto queda un prefijo pendiente por emitir
    if (prefijoActual != 0) {
        if (cantPares == capPares) {
            capPares *= 2;
            ParLZ78* nuevo = new ParLZ78[capPares];
            for (int k = 0; k < cantPares; k++) nuevo[k] = pares[k];
            delete[] pares;
            pares = nuevo;
        }
        pares[cantPares++] = {prefijoActual, '\0'};
    }

    return {pares, cantPares};
}

char* descomprimirLZ78(SalidaLZ78 salida) {
    DiccionarioLZ78 dict;

    int capTexto = 50;
    int lenTexto = 0;
    char* texto = new char[capTexto];

    for (int i = 0; i < salida.cantidad; i++) {
        ParLZ78 par = salida.pares[i];

        char temp[100];
        int tempLen = 0;

        if (par.caracter != '\0') {
            temp[tempLen++] = par.caracter;
        }

        // Reconstruimos la frase viajando hacia atrás a través de los índices de prefijo
        int curr = par.prefijo;
        while (curr > 0) {
            temp[tempLen++] = dict.entradas[curr - 1].caracter;
            curr = dict.entradas[curr - 1].prefijo;
        }

        // Añadimos la frase en el orden correcto al texto de salida
        for (int j = tempLen - 1; j >= 0; j--) {
            if (lenTexto + 1 >= capTexto) {
                capTexto *= 2;
                char* nuevo = new char[capTexto];
                for (int k = 0; k < lenTexto; k++) nuevo[k] = texto[k];
                delete[] texto;
                texto = nuevo;
            }
            texto[lenTexto++] = temp[j];
        }

        dict.agregar(par.prefijo, par.caracter);
    }

    texto[lenTexto] = '\0'; // Terminación de cadena
    return texto;
}

void liberarSalidaLZ78(SalidaLZ78& salida) {
    delete[] salida.pares;
    salida.pares = nullptr;
    salida.cantidad = 0;
}


// IMPLEMENTACIÓN MÓDULO 5.3: ENCRIPTACIÓN

unsigned char rotarIzquierda(unsigned char byte, int n) {
    // Desplazamiento circular a la izquierda usando operadores << y >>
    return (byte << n) | (byte >> (8 - n));
}

unsigned char rotarDerecha(unsigned char byte, int n) {
    // Desplazamiento circular a la derecha usando operadores >> y <<
    return (byte >> n) | (byte << (8 - n));
}

// Encripta un byte aplicando rotación y luego XOR con el carácter K
unsigned char encriptarByte(unsigned char byte, int n, unsigned char K) {
    unsigned char rotado = rotarIzquierda(byte, n);
    return rotado ^ K; // Operador XOR solicitado
}

// Desencripta un byte invirtiendo el proceso
unsigned char desencriptarByte(unsigned char byte, int n, unsigned char K) {
    unsigned char xorDeshecho = byte ^ K;
    return rotarDerecha(xorDeshecho, n);
}
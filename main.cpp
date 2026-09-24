#include <iostream>
#include <string>
#include <cstring>
#include "codificaciones.h"

using namespace std;


void probarPunto51() {
    cout << "  PUNTO 5.1: COMPRESION Y DESCOMPRESION RLE" << endl;


    string textoRLE;
    cout << "Ingrese el texto a probar con RLE: ";
    getline(cin, textoRLE);

    string comprimido = comprimirRLE(textoRLE);
    string descomprimido = descomprimirRLE(comprimido);

    cout << "\n   [RESULTADOS RLE]" << endl;
    cout << "   Texto Original:     " << textoRLE << endl;
    cout << "   Texto Comprimido:   " << comprimido << endl;
    cout << "   Texto Descomprimido:" << descomprimido << endl;
    cout << "   Estado:             "
         << (textoRLE == descomprimido ? "[OK] COINCIDE PERFECTAMENTE" : "[ERROR] No coincide") << endl;
}

void probarPunto52() {
    cout << "  PUNTO 5.2: COMPRESION Y DESCOMPRESION LZ78" << endl;

    string textoLZ;
    cout << "Ingrese el texto a probar con LZ78: ";
    getline(cin, textoLZ);

    SalidaLZ78 salida = comprimirLZ78(textoLZ.c_str());
    char* descomprimido = descomprimirLZ78(salida);

    cout << "\n   [RESULTADOS LZ78]" << endl;
    cout << "   Texto Original:     " << textoLZ << endl;
    cout << "   Pares Emitidos:     ";
    for (int i = 0; i < salida.cantidad; i++) {
        cout << "(" << salida.pares[i].prefijo << ","
             << (salida.pares[i].caracter == '\0' ? ' ' : salida.pares[i].caracter) << ") ";
    }
    cout << endl;
    cout << "   Texto Descomprimido:" << descomprimido << endl;
    cout << "   Estado:             "
         << (strcmp(textoLZ.c_str(), descomprimido) == 0 ? "[OK] COINCIDE PERFECTAMENTE" : "[ERROR] No coincide") << endl;

    // Liberación de la memoria dinámica utilizada
    liberarSalidaLZ78(salida);
    delete[] descomprimido;
}

void probarPunto53() {
    cout << "  PUNTO 5.3: ENCRIPTACION Y DESENCRIPTACION DE BITS" << endl;

    string texto;
    int n = 0;
    char claveK;

    cout << "Ingrese el texto a encriptar: ";
    getline(cin, texto);

    cout << "Ingrese el numero de posiciones a rotar n (1 a 7): ";
    while (!(cin >> n) || n < 1 || n > 7) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "  [!] Ingrese un valor de n valido entre 1 y 7: ";
    }

    cout << "Ingrese la clave K como un caracter (ej: K, *, a): ";
    cin >> claveK;
    cin.ignore(10000, '\n'); // Limpiar buffer

    string textoEncriptado = "";
    string textoDesencriptado = "";

    // Encriptamos todo el texto de corrido
    for (size_t i = 0; i < texto.length(); i++) {
        unsigned char enc = encriptarByte(texto[i], n, claveK);
        textoEncriptado += enc;
    }

    // Desencriptamos todo el bloque de corrido
    for (size_t i = 0; i < textoEncriptado.length(); i++) {
        unsigned char desenc = desencriptarByte(textoEncriptado[i], n, claveK);
        textoDesencriptado += desenc;
    }

    cout << "\n   [RESULTADOS ENCRIPTACION Y DESENCRIPTACION]" << endl;
    cout << "   Texto Original:      " << texto << endl;
    cout << "   Texto Encriptado:    " << textoEncriptado << endl;
    cout << "   Texto Desencriptado: " << textoDesencriptado << endl;
    cout << "   Estado:              "
         << (texto == textoDesencriptado ? "[OK] COINCIDE PERFECTAMENTE" : "[ERROR] No coincide") << endl;
}

// MENU PRINCIPAL

int main() {
    int opcion = 0;

    do {
        cout << "      PARTE 1 - PRACTICA 3: SELECCION DE MODULOS        " << endl;
        cout << "1. Probar Punto 5.1 (Compresion / Descompresion RLE)" << endl;
        cout << "2. Probar Punto 5.2 (Compresion / Descompresion LZ78)" << endl;
        cout << "3. Probar Punto 5.3 (Encriptacion / Desencriptacion Bits)" << endl;
        cout << "4. Salir" << endl;
        cout << "Seleccione una opcion (1-4): ";

        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\n[!] Entrada invalida. Por favor ingrese un numero de la lista." << endl;
            continue;
        }

        // Limpiamos el salto de linea restante para que 'getline' en las funciones lea correctamente
        cin.ignore(10000, '\n');

        switch (opcion) {
        case 1:
            probarPunto51();
            break;
        case 2:
            probarPunto52();
            break;
        case 3:
            probarPunto53();
            break;
        case 4:
            cout << "\nSaliendo del programa. ¡Exitos en la entrega!" << endl;
            break;
        default:
            cout << "\n[!] Opcion fuera de rango. Seleccione entre 1 y 4." << endl;
            break;
        }

    } while (opcion != 4);

    return 0;
}
#ifndef BANDEJA_H
#define BANDEJA_H
#include <iostream>
#include <correo.h>
#include <ctime>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include "indicePrim.h"
#include "indiceSec.h"
#include "indicePrimPag.h"
#include "LSL.h"
#include "Hash.h"

using namespace std;

class bandeja:public correo, public indicePrim, public indiceSec, public indicePrimPag, public Hash
{
    public:
        bandeja();
        ~bandeja();
        void menu();
        void crearCorreo();
        void leerCorreo();
        void modificar();
        void eliminar();
        bool valID(int id);
        void csv();
        void csvImport();
        void csvModificar();
        void csvEliminar();
        void propExp();
        void propModificar();
        void propEliminar();
        void busquedaEficiente();
        void busquedaIndicePrim(int llave);
        void cargaIP();
        void guardarIP();
        void busquedaIndiceSec();
        void cargaIS();
        void cargarIPP();
        void cargarTablaHash();
        void buscarTablaHash();
        void guardarTablaHash();
        void cifrado();
        void descifrado();
        void compresion();
        void descompresion();
        void RLE(string cadenota);

    private:
        fstream rwFile;
        fstream csvFile;
        fstream csvTemp;
        fstream propFile;
        fstream propTemp;
        fstream indicePFile;
        fstream indicePFileTemp;
        fstream tablaHashFile;
        fstream encryptedFile;
        fstream encryptedTemp;
        fstream comprimido;
        fstream comprimidoTemp;

        int contHash=0;
        bool bandHash=false;
        int sizeArray=0;
        bool bandSrch=false;
        bool bandIPP=false;
        indicePrim ip, ipFound;
        indiceSec is;
        indicePrimPag ipp, ippMin;

};

#endif // BANDEJA_H

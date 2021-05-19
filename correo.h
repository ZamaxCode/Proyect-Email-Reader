#ifndef CORREO_H
#define CORREO_H
#include <iostream>
#include <fstream>

using namespace std;

class correo
{
    private:
        struct Correos
        {
            int id;
            char fecha[10];
            char hora[10];
            char from[100];
            char to[100];
            char cc[100];
            char bcc[100];
            char asunto[100];
            char contenido[300];
        };

    public:
        correo();
        ~correo();
        Correos c;
        Correos cTemp;
        Correos correosRam[25];

};

#endif // CORREO_H

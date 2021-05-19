#ifndef INDICEPRIMPAG_H
#define INDICEPRIMPAG_H
#include "avltree.h"
#include <string>
#include <sstream>
#include <iostream>

class indicePrimPag
{
    private:
        int llavePrim;
        unsigned long referencia;
        int cont;

    public:
        indicePrimPag();
        ~indicePrimPag();
        AVLtree<indicePrimPag> arbolIndicePag;

        void setLlavePrim(int value);
        int getLlavePrim();
        void setReferencia(unsigned long value);
        unsigned long getReferencia();
        void setCont(int value);
        int getCont();

        std::string toString();

        bool operator ==(indicePrimPag der)const;
        bool operator !=(indicePrimPag der)const;

        bool operator >(indicePrimPag der)const;
        bool operator >=(indicePrimPag der)const;

        bool operator <(indicePrimPag der)const;
        bool operator <=(indicePrimPag der)const;

        indicePrimPag operator =(indicePrimPag der)const;
};

#endif // INDICEPRIMPAG_H

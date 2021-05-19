#ifndef INDICEPRIM_H
#define INDICEPRIM_H
#include "avltree.h"
#include <string>
#include <sstream>
#include <iostream>

class indicePrim
{
    private:
        int llavePrim;
        unsigned long referencia;

    public:
        indicePrim();
        ~indicePrim();
        AVLtree<indicePrim> arbolIndice;

        void setLlavePrim(int value);
        int getLlavePrim();
        void setReferencia(unsigned long value);
        unsigned long getReferencia();

        std::string toString();

        bool operator ==(indicePrim der)const;
        bool operator !=(indicePrim der)const;

        bool operator >(indicePrim der)const;
        bool operator >=(indicePrim der)const;

        bool operator <(indicePrim der)const;
        bool operator <=(indicePrim der)const;

        indicePrim operator =(indicePrim der)const;
};

#endif // INDICEPRIM_H

#include "indicePrim.h"

using namespace std;

indicePrim::indicePrim()
{
    //ctor
}

indicePrim::~indicePrim()
{
    //dtor
}

void indicePrim::setLlavePrim(int value)
{
    llavePrim=value;
}

int indicePrim::getLlavePrim()
{
    return llavePrim;
}

void indicePrim::setReferencia(unsigned long value)
{
    referencia=value;
}

unsigned long indicePrim::getReferencia()
{
    return referencia;
}

string indicePrim::toString() {
    stringstream ss;
    ss << llavePrim << "," << referencia;
    return ss.str();
}

bool indicePrim::operator==(indicePrim der)const
{
    indicePrim izq= *this;
    if(izq.llavePrim==der.llavePrim)
        return true;
    return false;
}

bool indicePrim::operator!=(indicePrim der)const
{
    indicePrim izq= *this;
    if(izq.llavePrim!=der.llavePrim)
        return true;
    return false;
}

bool indicePrim::operator>(indicePrim der)const
{
    indicePrim izq= *this;
    if(izq.llavePrim>der.llavePrim)
        return true;
    return false;
}

bool indicePrim::operator>=(indicePrim der)const
{
    indicePrim izq= *this;
    if(izq.llavePrim>=der.llavePrim)
        return true;
    return false;
}

bool indicePrim::operator<(indicePrim der)const
{
    indicePrim izq= *this;
    if(izq.llavePrim<der.llavePrim)
        return true;
    return false;
}

bool indicePrim::operator<=(indicePrim der)const
{
    indicePrim izq= *this;
    if(izq.llavePrim<=der.llavePrim)
        return true;
    return false;
}

indicePrim indicePrim::operator=(indicePrim der)const
{
    indicePrim izq = *this;
    izq.llavePrim=der.llavePrim;
    izq.referencia=der.referencia;
    return izq;
}



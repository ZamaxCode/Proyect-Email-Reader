#include "indicePrimPag.h"

indicePrimPag::indicePrimPag()
{
    //ctor
}

indicePrimPag::~indicePrimPag()
{
    //dtor
}

void indicePrimPag::setLlavePrim(int value)
{
    llavePrim=value;
}

int indicePrimPag::getLlavePrim()
{
    return llavePrim;
}

void indicePrimPag::setReferencia(unsigned long value)
{
    referencia=value;
}

unsigned long indicePrimPag::getReferencia()
{
    return referencia;
}

void indicePrimPag::setCont(int value)
{
    cont=value;
}

int indicePrimPag::getCont()
{
    return cont;
}

string indicePrimPag::toString() {
    stringstream ss;
    ss << llavePrim << "," << referencia << "," << cont;
    return ss.str();
}

bool indicePrimPag::operator==(indicePrimPag der)const
{
    indicePrimPag izq= *this;
    if(izq.llavePrim==der.llavePrim)
        return true;
    return false;
}

bool indicePrimPag::operator!=(indicePrimPag der)const
{
    indicePrimPag izq= *this;
    if(izq.llavePrim!=der.llavePrim)
        return true;
    return false;
}

bool indicePrimPag::operator>(indicePrimPag der)const
{
    indicePrimPag izq= *this;
    if(izq.llavePrim>der.llavePrim)
        return true;
    return false;
}

bool indicePrimPag::operator>=(indicePrimPag der)const
{
    indicePrimPag izq= *this;
    if(izq.llavePrim>=der.llavePrim)
        return true;
    return false;
}

bool indicePrimPag::operator<(indicePrimPag der)const
{
    indicePrimPag izq= *this;
    if(izq.llavePrim<der.llavePrim)
        return true;
    return false;
}

bool indicePrimPag::operator<=(indicePrimPag der)const
{
    indicePrimPag izq= *this;
    if(izq.llavePrim<=der.llavePrim)
        return true;
    return false;
}

indicePrimPag indicePrimPag::operator=(indicePrimPag der)const
{
    indicePrimPag izq = *this;
    izq.setLlavePrim(der.getLlavePrim());
    izq.setReferencia(der.getReferencia());
    izq.setCont(der.getCont());
    return izq;
}

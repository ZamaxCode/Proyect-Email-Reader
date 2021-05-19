#include "indiceSec.h"

indiceSec::indiceSec()
{
    //ctor
}

indiceSec::~indiceSec()
{
    //dtor
}

void indiceSec::setLlaveSec(string value)
{
    llaveSec=value;
}

string indiceSec::getLlaveSec()
{
    return llaveSec;
}

void indiceSec::setListRef(int value)
{
    listRef.push_back(value);
}

LSL<int> indiceSec::getListRef()
{
    return listRef;
}

bool indiceSec::operator==(indiceSec der) const
{
    indiceSec izq= *this;
    if((izq.llaveSec.compare(der.llaveSec))==0)
        return true;
    return false;
}

bool indiceSec::operator!=(indiceSec der) const
{
    indiceSec izq= *this;
    if((izq.llaveSec.compare(der.llaveSec))!=0)
        return true;
    return false;
}

bool indiceSec::operator>(indiceSec der) const
{
    indiceSec izq= *this;
    if((izq.llaveSec.compare(der.llaveSec))>0)
        return true;
    return false;
}

bool indiceSec::operator>=(indiceSec der) const
{
    indiceSec izq= *this;
    if((izq.llaveSec.compare(der.llaveSec))>=0)
        return true;
    return false;
}

bool indiceSec::operator<(indiceSec der) const
{
    indiceSec izq= *this;
    if((izq.llaveSec.compare(der.llaveSec))<0)
        return true;
    return false;
}

bool indiceSec::operator<=(indiceSec der) const
{
    indiceSec izq= *this;
    if((izq.llaveSec.compare(der.llaveSec))<=0)
        return true;
    return false;
}

indiceSec indiceSec::operator=(indiceSec der) const
{
    indiceSec izq = *this;
    izq.llaveSec=der.llaveSec;
    for(int i=0;i<der.listRef.size();i++)
    {
        izq.listRef.push_back(der.listRef[i]);
    }
    return izq;
}

string indiceSec::toString()
{
    stringstream ss;
    ss << llaveSec;
    return ss.str();
}





#ifndef HASH_H
#define HASH_H
#include "LSL.h"

class Hash
{
    public:
        Hash();
        ~Hash();
        struct ipStr{
                char llave[10];
                unsigned long referencia;
        }ipst;
        int hashing(char *clave);
        LSL<ipStr>tablaHash[223];
};

#endif // HASH_H

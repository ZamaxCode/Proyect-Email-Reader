#include "Hash.h"

Hash::Hash()
{
    //ctor
}

Hash::~Hash()
{
    //dtor
}

int Hash::hashing(char *clave)
{
    int primo = 223;
    char *p;
    unsigned int h=0,g;

    for(p = clave; *p != '\0'; p++)
    {
        h = (h << 4) + (*p);
        g = h&0xF0000000;
        if(g > 1)
        {
            h = h^(g<<24);
            h = h^g;
        }
    }
    return (h%primo);
}



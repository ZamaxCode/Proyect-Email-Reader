#ifndef INDICESEC_H
#define INDICESEC_H
#include "avltree.h"
#include "LSL.h"
#include <string>
#include <sstream>
#include <iostream>

class indiceSec
{
    private:
        string llaveSec;
        LSL<int> listRef;

    public:
        indiceSec();
        ~indiceSec();
        AVLtree<indiceSec> arbolIndiceSecRem;
        AVLtree<indiceSec> arbolIndiceSecDes;

        void setLlaveSec(string value);
        string getLlaveSec();
        void setListRef(int value);
        LSL<int> getListRef();

        std::string toString();

        bool operator ==(indiceSec der)const;
        bool operator !=(indiceSec der)const;

        bool operator >(indiceSec der)const;
        bool operator >=(indiceSec der)const;

        bool operator <(indiceSec der)const;
        bool operator <=(indiceSec der)const;

        indiceSec operator =(indiceSec der)const;
};

#endif // INDICESEC_H

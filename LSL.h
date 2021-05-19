#ifndef LSL_H_INCLUDED
#define LSL_H_INCLUDED
#include <iostream>
#include <stdexcept>

using namespace std;

template<typename T>
class LSL
{
private:
    struct NodoLSL
    {
        T dato;
        NodoLSL* siguiente;
        //sig toma nullptr como valor por omisión
        NodoLSL(const T& elem, NodoLSL* sig = nullptr)
        {
            dato = elem;
            siguiente = sig;
        }
    };
    size_t listSize;
    NodoLSL* listFront;
    NodoLSL* listBack;

public:
    LSL()
    {
        listSize = 0;
        listFront = nullptr;
        listBack = nullptr;
    }
    LSL(const LSL& other)
    {
        listSize = 0;
        listFront = nullptr;
        listBack = nullptr;
        for (size_t i(0); i < other.size(); ++i)
            push_back(other[i]);
    }
    LSL& operator() (const LSL& other)
    {
        listSize = 0;
        listFront = nullptr;
        listBack = nullptr;
        for (size_t i(0); i < other.size(); ++i)
            push_back(other[i]);
        return *this;
    }
    ~LSL()
    {
        clear();
    }

    bool empty() const;
    size_t size() const;
    void push_front(const T& element);
    void push_back(const T& element);
    const T& front() const;
    const T& back() const;
    void pop_front();
    void pop_back();
    void insert(size_t position, const T& element);
    void erase(size_t position);
    void clear();
    void remove(const T& value);
    T& operator [](size_t index) const;
};

template<typename T>
bool LSL<T>::empty() const
{
    return listSize == 0;
}

template<typename T>
size_t LSL<T>::size() const
{
    return listSize;
}

template<typename T>
void LSL<T>::push_front(const T &element)
{
    if (empty())
    {
        listFront = new NodoLSL(element);
        listBack = listFront;
    }
    else
    {
        NodoLSL* temp = new NodoLSL(element, listFront);
        listFront = temp;
    }

    ++listSize;
}

template<typename T>
void LSL<T>::push_back(const T &element)
{
    if (empty())
    {
        listFront = new NodoLSL(element);
        listBack = listFront;
    }
    else
    {
        NodoLSL* temp = new NodoLSL(element);
        listBack->siguiente = temp;
        listBack = temp;
    }

    ++listSize;
}

template<typename T>
const T& LSL<T>::front() const
{
    if (empty())
        throw range_error("front() on empty list");

    return listFront->dato;
}

template<typename T>
const T& LSL<T>::back() const
{
    if (empty())
        throw range_error("back() on empty list");

    return listBack->dato;
}

template<typename T>
void LSL<T>::pop_front()
{
    if (empty())
        throw range_error("pop_front() on empty list");

    NodoLSL* temp = listFront;
    listFront = listFront->siguiente;
    delete temp;
    temp = nullptr;
    --listSize;
}

template<typename T>
void LSL<T>::pop_back()
{
    if (empty())
        throw range_error("pop_back() on empty list");

    NodoLSL* temp = listFront;
    while(temp->siguiente != nullptr && temp->siguiente != listBack)
        temp = temp->siguiente;
    listBack = temp;
    delete temp->siguiente;
    temp->siguiente = nullptr;
    listBack->siguiente = nullptr;
    --listSize;
}

template<typename T>
void LSL<T>::insert(size_t position, const T &element)
{
    if (position > listSize)
        throw range_error("insert() in non valid position");
    if (position == 0)
        push_front(element);
    else if (position == listSize)
        push_back(element);
    else
    {
        NodoLSL* temp = listFront;
        for (size_t i(0); i < position-1; ++i)
            temp = temp->siguiente;
        NodoLSL* nuevo = new NodoLSL(element, temp->siguiente);
        temp->siguiente = nuevo;
        ++listSize;
    }
}

template<typename T>
void LSL<T>::erase(size_t position)
{
    if (empty())
        throw range_error("erase() in non valid position");
    else if (position >= listSize)
        throw range_error("erase() in non valid position");
    else if (position == 0)
        pop_front();
    else if (position == listSize-1)
        pop_back();
    else
    {
        NodoLSL* temp = listFront;
        for (size_t i(0); i < position-1; ++i)
            temp = temp->siguiente;
        NodoLSL* eliminar = temp->siguiente;
        temp->siguiente = eliminar->siguiente;
        delete eliminar;
        eliminar = nullptr;
        --listSize;
    }
}

template<typename T>
void LSL<T>::clear()
{
    for (size_t i(0); i < listSize; ++i)
        pop_front();

    listSize = 0;
    listFront = nullptr;
    listBack = nullptr;
}

template<typename T>
void LSL<T>::remove(const T &value)
{
    if (empty())
        throw range_error("remove() from empty list");
    size_t i = 0;
    T dato;
    NodoLSL* temp = listFront;
    while(temp != nullptr)
    {
        dato = temp->dato;
        temp = temp->siguiente;
        if (dato == value)
        {
            erase(i);
            --i;
        }
        ++i;
    }
}

template<typename T>
T& LSL<T>::operator [](size_t index) const
{
    if (empty())
        throw range_error("[] in empty list");
    if (index >= listSize)
        throw range_error("[] in non valid position");
    NodoLSL* temp = listFront;
    for (size_t i(0); i < index; ++i)
        temp = temp->siguiente;
    return temp->dato;
}
#endif // LSL_H_INCLUDED


#ifndef AVLTREE_H
#define AVLTREE_H

#include <exception>
#include <string>
#include <stdio.h>

#include <iostream>

using namespace std;

//Clase del nodo

template <class Alpha>
class TreeNode
{
private:
    Alpha* dataPtr;
    TreeNode* right;
    TreeNode* left;
public:
    class Exception : std::exception{
    private:
        std::string msg;

    public:
        explicit Exception(const char* message) : msg(message) { }
        explicit Exception(const std::string& message) : msg(message) { }
        virtual ~Exception() throw() {}
        virtual const char* what() const throw(){
            return msg.c_str();
        }
    };
    TreeNode();
    TreeNode(const Alpha&);
    ~TreeNode();
    Alpha* getDataPtr();
    Alpha& getData();
    TreeNode<Alpha>*& getRight();
    TreeNode<Alpha>*& getLeft();
    void setData(const Alpha&);
    void setDataPtr(Alpha*);
    void setRight(TreeNode<Alpha>*&);
    void setLeft(TreeNode<Alpha>*&);
};

template<class Alpha>
TreeNode<Alpha>::TreeNode() : right(nullptr), left(nullptr), dataPtr(nullptr) {}

template<class Alpha>
TreeNode<Alpha>::TreeNode(const Alpha& e) : right(nullptr), left(nullptr), dataPtr(new Alpha(e))
{
    if(dataPtr==nullptr)
    {
        throw TreeNode::Exception("Unable to create the node, kill yourself");
    }
}

template<class Alpha>
TreeNode<Alpha>::~TreeNode()
{
    delete dataPtr;
}

template<class Alpha>
Alpha* TreeNode<Alpha>::getDataPtr()
{
    return dataPtr;
}

template<class Alpha>
Alpha& TreeNode<Alpha>::getData()
{
    if(dataPtr==nullptr)
    {
        throw TreeNode::Exception("invalid data");
    }
    return *dataPtr;
}

template<class Alpha>
TreeNode<Alpha>*& TreeNode<Alpha>::getLeft()
{
    return left;
}

template<class Alpha>
TreeNode<Alpha>*& TreeNode<Alpha>::getRight()
{
    return right;
}

template<class Alpha>
void TreeNode<Alpha>::setData(const Alpha& e)
{
    if(dataPtr==nullptr)
    {
        try {
            if((dataPtr = new Alpha(e))==nullptr)
            {
                throw TreeNode::Exception("Unable the create the node");
            }
        } catch (TreeNode::Exception& ex) {
            throw TreeNode::Exception(ex.what());

        }
    }
    *dataPtr=e;
}

template<class Alpha>
void TreeNode<Alpha>::setDataPtr(Alpha* p)
{
    dataPtr=p;
}

template<class Alpha>
void TreeNode<Alpha>::setLeft(TreeNode<Alpha>*& p)
{
    left=p;
}

template<class Alpha>
void TreeNode<Alpha>::setRight(TreeNode<Alpha>*& p)
{
    right=p;
}

//Clase del arbol

template<class ALPHA>
class AVLtree {
private:
    TreeNode<ALPHA>* root;

    void deleteAll(TreeNode<ALPHA>*&);
    void copyNodes(TreeNode<ALPHA>*&);
    void insertData(TreeNode<ALPHA>*& r, const ALPHA& e);

    TreeNode<ALPHA>*& findData(TreeNode<ALPHA>*& r, const ALPHA& e);

    int getHeight(TreeNode<ALPHA>*&);

    int getBalanceFactor(TreeNode<ALPHA>*&);

    void simpleLeftRotation(TreeNode<ALPHA>*&);
    void simpleRightRotation(TreeNode<ALPHA>*&);
    void doubleLeftRotation(TreeNode<ALPHA>*&);
    void doubleRightRotation(TreeNode<ALPHA>*&);

    void doBalancing(TreeNode<ALPHA>*&);

    std::string& parsePreOrder(string&,TreeNode<ALPHA>*&);
    std::string parseInOrder(string,TreeNode<ALPHA>*&);
    std::string& parsePostOrder(string&,TreeNode<ALPHA>*&);

public:
    class Exception : std::exception{
    private:
        std::string msg;

    public:
        explicit Exception(const char* message) : msg(message) { }
        explicit Exception(const std::string& message) : msg(message) { }
        virtual ~Exception() throw() {}
        virtual const char* what() const throw(){
            return msg.c_str();
        }
    };

    AVLtree();
    AVLtree(const AVLtree&);

    ~AVLtree();

    bool isEmpty() const;
    void insertData(const ALPHA&);
    void deleteData(TreeNode<ALPHA>*&);

    bool isLeaf(TreeNode<ALPHA>*&);

    TreeNode<ALPHA>*& getLowest(TreeNode<ALPHA>*&);
    TreeNode<ALPHA>*& getHighest(TreeNode<ALPHA>*&);

    TreeNode<ALPHA>*& findData(const ALPHA&);
    ALPHA* retrieve(TreeNode<ALPHA>*&);
    ALPHA* retrievePtr(TreeNode<ALPHA>*&);
    int getHeight();
    int getRightHeight();
    int getLeftHeight();

    std::string parsePreOrder();
    std::string parseInOrder();
    std::string parsePostOrder();

    void deleteAll();
    void copyAll(const AVLtree<ALPHA>&);

    AVLtree<ALPHA>& operator = (const AVLtree<ALPHA>&);
};

using namespace std;

template<class ALPHA>
AVLtree<ALPHA>::AVLtree() : root(nullptr) {}


template<class ALPHA>
AVLtree<ALPHA>::AVLtree(const AVLtree<ALPHA>& t) : root(nullptr)
{
    copyAll(t);
}

template<class ALPHA>
AVLtree<ALPHA>::~AVLtree()
{
    deleteAll();
}

template<class ALPHA>
bool AVLtree<ALPHA>::isEmpty() const
{
    return root==nullptr;
}

template<class ALPHA>
void AVLtree<ALPHA>::copyAll(const AVLtree<ALPHA> & t)
{
    //copyNodes(t.root);
}

template<class ALPHA>
void AVLtree<ALPHA>::insertData(TreeNode<ALPHA>*& r,const ALPHA& e)
{
    if(r == nullptr)
    {
        try {
            if((r = new TreeNode<ALPHA>(e)) == nullptr)
            {
                throw Exception("Not enought memory, inserData()");
            }
        } catch (typename TreeNode<ALPHA>::Exception &ex) {
            throw Exception(ex.what());
        }
    }
    else
    {
        if(e < r->getData())
        {
            insertData(r->getLeft(), e);
        }
        else
        {
            insertData(r->getRight() ,e);
        }
        doBalancing(r);
    }
}

template<class ALPHA>
TreeNode<ALPHA>*& AVLtree<ALPHA>::findData(TreeNode<ALPHA> *&r, const ALPHA &e)
{
    if (r == nullptr or r->getData() == e) {
        return r;
    }
    if (e < r->getData()) {
        return findData(r->getLeft(), e);
    }
    return findData(r->getRight(), e);
}



template<class ALPHA>
int AVLtree<ALPHA>::getHeight(TreeNode<ALPHA> *& r)
{
    if(r == nullptr)
    {
        return 0;
    }
    if(isLeaf(r))
    {
        return 1;
    }
    int leftHeight(getHeight(r->getLeft()));
    int rightHeight(getHeight(r->getRight()));
    return 1+ (leftHeight > rightHeight ? leftHeight : rightHeight);
}

template<class ALPHA>
int AVLtree<ALPHA>::getBalanceFactor(TreeNode<ALPHA> *& r)
{
    return getHeight(r->getRight())-getHeight(r->getLeft());
}

template<class ALPHA>
void AVLtree<ALPHA>::simpleLeftRotation(TreeNode<ALPHA> *& r)
{
    TreeNode<ALPHA> *aux(r->getRight());
    TreeNode<ALPHA> *aux1(aux->getLeft());
    r->setRight(aux1);
    aux->setLeft(r);
    r=aux;
}

template<class ALPHA>
void AVLtree<ALPHA>::simpleRightRotation(TreeNode<ALPHA> *& r)
{
    TreeNode<ALPHA>* aux1(r->getLeft());
    TreeNode<ALPHA>* aux2(aux1->getRight());

    r->setLeft(aux2);
    aux1->setRight(r);
    r=aux1;
}

template<class ALPHA>
void AVLtree<ALPHA>::doubleLeftRotation(TreeNode<ALPHA> *& r)
{
    simpleRightRotation(r->getRight());
    simpleLeftRotation(r);
}

template<class ALPHA>
void AVLtree<ALPHA>::doubleRightRotation(TreeNode<ALPHA> *& r)
{
    simpleLeftRotation(r->getLeft());
    simpleRightRotation(r);
}

template<class ALPHA>
void AVLtree<ALPHA>::doBalancing(TreeNode<ALPHA> *& r)
{
    switch (getBalanceFactor(r)) {
    case -2:
        if(getBalanceFactor(r->getLeft())== -1){
            simpleRightRotation(r);
        }
        else{
            doubleRightRotation(r);
        }
        break;
    case 2:
        if(getBalanceFactor(r->getRight())==1){
            simpleLeftRotation(r);
        }
        else{
            doubleLeftRotation(r);
        }
        break;
    }
}

template<class ALPHA>
void AVLtree<ALPHA>::insertData(const ALPHA& e)
{
    insertData(root,e);
}

template<class ALPHA>
void AVLtree<ALPHA>::deleteData(TreeNode<ALPHA> *& r)
{
    if (r == nullptr) {
        cout<<"Posicion invalida"<<endl;
		throw Exception("POSICION INVALIDA, deleteData");
	}

	if (isLeaf(r)) {
		delete r;
		r = nullptr;
		return;
	}

	TreeNode<ALPHA>*& substitute(r->getLeft() != nullptr ? getHighest(r->getLeft()) : getLowest(r->getRight()));
	swap(r->getData(), substitute->getData());
	deleteData(substitute);
}

template<class ALPHA>
bool AVLtree<ALPHA>::isLeaf(TreeNode<ALPHA> *& r)
{
    return (r != nullptr and r->getLeft() == r->getRight());
}

template<class ALPHA>
TreeNode<ALPHA> *&AVLtree<ALPHA>::getLowest(TreeNode<ALPHA> *& r)
{
    if(r==nullptr or r->getLeft()==nullptr)
    {
        return r;
    }
    return getLowest(r->getLeft());
}

template<class ALPHA>
TreeNode<ALPHA> *&AVLtree<ALPHA>::getHighest(TreeNode<ALPHA> *& r)
{
    if(r==nullptr or r->getRight()==nullptr)
    {
        return r;
    }
    return getHighest(r->getRight());
}

template<class ALPHA>
TreeNode<ALPHA>*& AVLtree<ALPHA>::findData(const ALPHA & e)
{
    return findData(root,e);
}

template<class ALPHA>
ALPHA* AVLtree<ALPHA>::retrieve(TreeNode<ALPHA> *& r)
{
    if (r) {
        return &r->getData();
    } else {
        throw Exception("Posicion invalida");
    }
}

template<class ALPHA>
ALPHA *AVLtree<ALPHA>::retrievePtr(TreeNode<ALPHA> *& r)
{
    return r->getDataPtr();
}

template<class ALPHA>
int AVLtree<ALPHA>::getHeight()
{
    return getHeight(root);
}

template<class ALPHA>
int AVLtree<ALPHA>::getRightHeight()
{
    return getHeight(root->getRight());
}

template<class ALPHA>
int AVLtree<ALPHA>::getLeftHeight()
{
    return getHeight(root->getLeft());
}

template<class ALPHA>
string& AVLtree<ALPHA>::parsePostOrder(string& result,TreeNode<ALPHA>*& r)
{
    if(r != nullptr)
    {
        result=parsePostOrder(result,r->getLeft());
        result=parsePostOrder(result,r->getRight());
        result+= r->getData().toString();
    }

    return result;
}

template <class ALPHA>
string AVLtree<ALPHA>::parseInOrder(string result,TreeNode<ALPHA>*& r)
{
    if(r!=nullptr)
    {
        result=parseInOrder(result,r->getLeft());
        result+=r->getData().toString() + "\n";
        result=parseInOrder(result,r->getRight());
    }
    return result;
}

template <class ALPHA>
string& AVLtree<ALPHA>::parsePreOrder(string& result,TreeNode<ALPHA>*& r)
{
    if(r!=nullptr)
    {
        result+=r->getData().toString();
        result=parsePreOrder(result,r->getLeft());
        result=parsePreOrder(result,r->getRight());
    }
    return result;
}

template <class ALPHA>
string AVLtree<ALPHA>::parsePreOrder()
{
    string resulty;
    return parsePreOrder(resulty,root);
}
template <class ALPHA>
string AVLtree<ALPHA>::parseInOrder()
{
    string resulty;
    return parseInOrder(resulty,root);
}
template <class ALPHA>
string AVLtree<ALPHA>::parsePostOrder()
{
    string resulty;
    return parsePostOrder(resulty,root);
}

template<class ALPHA>
void AVLtree<ALPHA>::deleteAll()
{
    deleteAll(root);
}

template <class ALPHA>
void AVLtree<ALPHA>::deleteAll(TreeNode<ALPHA>*& node)
{
    if(node != nullptr)
    {
        deleteAll(node->getLeft());
        deleteAll(node->getRight());
        delete node;
    }
    node = nullptr;
}

template<class ALPHA>
void AVLtree<ALPHA>::copyNodes(TreeNode<ALPHA> *& r)
{
    if(r!=nullptr)
    {
        this->insertData(r->getData());
        copyNodes(r->getLeft());
        copyNodes(r->getRight());
    }
}

#endif // AVLTREE_H

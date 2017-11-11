/***************************LIBRERIA LINKED LISTS***********************/
//MANEJO DE LISTAS ENLAZADAS 
//DANIEL RIOS SEADE


#ifndef __H__DINAMIC_LINKED_LIST
#define __H__DINAMIC_LINKED_LIST
#include <stdio.h>
#include <stdlib.h>
#define TREENODE_RIGHT 0
#define TREENODE_LEFT 1
#define _DSTRUCTS_111

#ifdef _DSTRUCTS_111
typedef unsigned int dword;
struct ldata
{
    void *pData;
    unsigned long dwIndex;
};

typedef ldata lvoid;
#else
typedef void *lvoid;
#endif
//Estructura BASICA de una lista terminada en cero
typedef struct _DYNLINKLIST
{
        lvoid data;
	struct _DYNLINKLIST *s;
	
}DYNLINKLIST,*LPDYNLINKLIST;

typedef struct _DBDYNLINKLIST
{
	void *data;
	struct _DBDYNLINKLIST *l;
	struct _DBDYNLINKLIST *r;
}DBDYNLINKLIST,*LPDBDYNLINKLIST,*LPTREENODE,TREENODE;
//Lista enlazada de recorrido lineal
typedef struct
{
        dword NumElements;//Numero de elementos de 0 a n
	LPDYNLINKLIST firstelement;//primer elemento de la lista
	LPDYNLINKLIST list;//Posicion del elemento actual
	LPDYNLINKLIST lastelement;//Ultimo elemento
}LINKEDLIST,*LPLINKEDLIST;

typedef struct//LISTAS DOBLEMENTE ENLAZADAS :NO IMPLEMENTADO
{
	int NumElements;
	LPDBDYNLINKLIST firstelement;
	LPDBDYNLINKLIST list;
	LPDBDYNLINKLIST lastelement;
}DBLINKEDLIST,*LPDBLINKEDLIST;


class CLinkedList
{
public:
    LPLINKEDLIST CreateList(void);//INICIA LA INTERFACE
    bool AddElement(LPLINKEDLIST,void*);//Añade un elemento
    bool InsertElement(LPLINKEDLIST,void*,dword);//Añade un elemento en la posicion
    bool DeleteElement(LPLINKEDLIST,dword);//Borra un elemento
    bool DeleteCurrElement(LPLINKEDLIST);//Borra el elemento actual
    bool SetElement(LPLINKEDLIST,dword);//Establece un elemento
    void *GetElementData(LPLINKEDLIST,dword);//Obtiene el elemento especificado
    void *GetCurrElementData(LPLINKEDLIST);//Obtiene el elemento apuntado por list

    bool SetElementData(LPLINKEDLIST,dword,void*);//Establece un nuevo elemento
    bool DeleteList(LPLINKEDLIST*);//Borra toda la lista
    dword GetNumOfElements(LPLINKEDLIST);//Obtiene el total de elementos de la lista
    bool ForwardElement(LPLINKEDLIST);//Adelanta un elemento
    bool SetFirstElement(LPLINKEDLIST);//Establece a list con first element
    bool SetLastElement(LPLINKEDLIST);//Establece el ultimo elemento
    bool IsEmpty(LPLINKEDLIST);//Indica que la lista esta vacia
	//Added Feb 2010
	//Mover el elemento index al anterior y pasar anterior a actual
    void MoveElementTop(LPLINKEDLIST pList, dword dwIndex);
	//Mover el elemento index a posterior y pasar posterior a anterior
    void MoveElementDown(LPLINKEDLIST pList, dword dwIndex);
	//
    virtual void DeleteElementData(void*){} //Funcion virtual para eliminar los datos agregados
#ifdef _DSTRUCTS_111
    int GetCurrentIndex(LPLINKEDLIST pList);
    void MarkForDeletion(LPLINKEDLIST pList,dword dwIndex);
    void MarkCurrentForDeletion(LPLINKEDLIST pList);
    void DeleteMarked(LPLINKEDLIST pList);
#endif

};
//NO IMPLEMENTADO TODAVIA

class CDBDynLinkedList
{
public:
	LPDBLINKEDLIST CreateList(void);
	bool AddElement(LPDBLINKEDLIST,void*);
	bool DeleteElement(LPDBLINKEDLIST,int);
	bool SetElement(LPDBLINKEDLIST,int);
	void *GetElementData(LPDBLINKEDLIST,int);
	bool SetElementData(LPDBLINKEDLIST,int,void*);
	bool DeleteList(LPDBLINKEDLIST*);
        virtual void DeleteElementData(void*){}

};




#endif

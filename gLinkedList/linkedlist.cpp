#include "linkedl.h"

/****************LISTAS TERMINADAS EN CERO************************/
LPLINKEDLIST CLinkedList::CreateList(void)
{
	LPLINKEDLIST mlist;
#ifdef _VIRTUAL_MEMORY
        mlist=(LINKEDLIST *)vm_alloc(1,sizeof(LINKEDLIST));
#else
	mlist=new LINKEDLIST;//Creamos la estructura en memoria
#endif

	if(mlist)//Si tenemos exito
	{
#ifdef _VIRTUAL_MEMORY
                mlist->list=(DYNLINKLIST *)vm_alloc(1,sizeof(DYNLINKLIST));
#else
                mlist->list=new DYNLINKLIST;//Creamos la lista
#endif
		mlist->firstelement=mlist->list;//Apuntamos el primer elemento como el mismo de list
		mlist->lastelement=mlist->list;//Igual con el ultimo
		mlist->list->s=NULL;//El siguiente elemento es NULO
                mlist->list->data.pData=NULL;//Los datos los ponemos como nulos
		mlist->NumElements=0;//numero de elementos es -1 para indicar que no hay
	}
	return mlist;
}

//AÑADE UN ELEMENTO A LA LISTA
bool CLinkedList::AddElement(LPLINKEDLIST mlist,void *data)
{
	LPDYNLINKLIST nlist;//Lista nueva
	if(!mlist)return false;//si mlist es NULL entonces no hacemos nada
	//Observamos si list sea igual a lastelement e igual a firstelement y que los datos sean nulos
	//significa que la lista no tiene datos
        if(((mlist->list==mlist->lastelement)&&(mlist->firstelement==mlist->list))&&(mlist->list->data.pData==NULL))
	{
                mlist->list->data.pData=data;//Agregamos los datos
                mlist->list->data.dwIndex=0;
		mlist->NumElements=0;//Los datos se guardan basados en 1
	}
	else
	{
#ifdef _VIRTUAL_MEMORY
                nlist=(DYNLINKLIST *)vm_alloc(1,sizeof(DYNLINKLIST));
#else
		nlist=new DYNLINKLIST;//Creamos la nueva lista
#endif
		nlist->s=NULL;//Ponemos al siguiente de la nueva lista en NULL
                //****************Cambio a version 1.1.1****************************/
                //Apuntamos los datos
                nlist->data.pData=data;
                nlist->data.dwIndex=mlist->NumElements + 1;
                /*********************************************/
		mlist->list=mlist->lastelement;//nos posicionamos en el ultimo elemento
		mlist->list->s=nlist;//apuntamos el siguiente de la principal el recien creado
		mlist->lastelement=nlist;//El ultimo elemento es el recien creado
		mlist->list=mlist->firstelement;//Ponemos a la lista en el primero
		mlist->NumElements++;//Incrementamos la cantidad de elementos
	}
	return true;
}
//Added for rebuild the list index zone 1.1.1
void RebuildIndex(LPLINKEDLIST mlist)
{
    dword dwI=0;

    mlist->list=mlist->firstelement;
    while(mlist->list!=NULL)
    {
        mlist->list->data.dwIndex=dwI;
        dwI++;
        mlist->list=mlist->list->s;
    }
}

//****************************InsertElement*********************/
bool CLinkedList::InsertElement(LPLINKEDLIST mlist,void *pData,dword dwIndex)
{
	LPDYNLINKLIST pNewElement=NULL;
	if(!mlist)return false;
	if(!mlist->list)return false;
	//Verificamos el indice
	if(dwIndex>mlist->NumElements)return false; 
	//Creamos el elemento
#ifdef _VIRTUAL_MEMORY
        pNewElement=(DYNLINKLIST *)vm_alloc(1,sizeof(DYNLINKLIST));
#else

	pNewElement=new DYNLINKLIST;
#endif
	//Establecemos sus propiedades
        pNewElement->data.pData=pData;//El dato
	//Cuando es el primer elemento
	if(dwIndex==0)//Elemento 0
	{
		SetFirstElement(mlist);//Primer elemento
		//El nuevo elemento apunta al viejo
		pNewElement->s=mlist->list;
		//Asignamos el primer elemento con el nuevo
		mlist->firstelement =pNewElement;
		//asignamos a la lista el primero
		mlist->list =pNewElement;
                //Va actualizacion 1.1.1************************************/
                RebuildIndex(mlist);
                /************************************************************/
		return true;//Regresamos verdadero
	};
	//Cuando es el ultimo elemento o cualquiera que no sea el primero
	if(dwIndex<=mlist->NumElements)
	{
		//Nos posicionamos en el anterior a index
		SetElement(mlist,dwIndex-1);//Uno antes
		//El nuevo apunta al actual
		pNewElement->s =mlist->list->s;  
		
		mlist->list->s=pNewElement;//El anterior apunta al nuevo
                /*******************************1.1.1*******************************************/
                //Tricky part
                RebuildIndex(mlist);
                //*******************************************************************************/
		return true;
	}
	return true;

	


}
	
//Borramos el elemento actual de la lista
bool CLinkedList::DeleteCurrElement(LPLINKEDLIST mlist)
{
	//Buscamos el Elemento
	LPDYNLINKLIST pTemp;//Temporal
        dword i=0;//Contador
	if(!mlist)return false;
	if(!mlist->list)return false; 
	pTemp=mlist->firstelement;//Para busqueda sin alterar la lista
	
	while(pTemp!=mlist->list)
	{
		pTemp=pTemp->s;//Recorremos la lista
		i++;
	}
	//Se sale ya que encontro el elemento
	//Ya encontrado se borra
	DeleteElement(mlist,i);

	return true;
}
	

/***************************************************************/
//BORRAMOS UN ELEMENTO DE LA LISTA
bool CLinkedList::DeleteElement(LPLINKEDLIST mlist,dword id)
{
	LPDYNLINKLIST temp;
	if(SetElement(mlist,id))//ESTABLECEMOS AL ELEMENTO QUE QUEREMOS BORRAR
	{
		if(mlist->NumElements>0)//Si tenemos mas de un elemento
		{
			if(mlist->list==mlist->firstelement)//Si elemento a borrar es igual al primer elemento
			{
				mlist->firstelement=mlist->list->s;//Pasamo el apuntar el firstelement al siguiente elemento
                                DeleteElementData(mlist->list->data.pData);//Borramos los datos
#ifdef _VIRTUAL_MEMORY
                                vm_free(mlist->list);
#else
				delete mlist->list;                  //Borramos la lista
#endif
				mlist->list=mlist->firstelement;     //Pasamos al apuntador al primer elemento
			}
			else if(mlist->lastelement==mlist->list)//Si elemento a borrar es el ultimo
			{
				SetElement(mlist,id-1);//Nos posicionamos en uno anterior
				mlist->lastelement=mlist->list;//Apuntamos lastelement al anterior posicionado
				mlist->list=mlist->list->s;//apuntamos a lista al elemento que deseamos borrar
                                DeleteElementData(mlist->list->data.pData);//Borramos los datos
				mlist->list->s=NULL;//No muy necesario
#ifdef _VIRTUAL_MEMORY
                                vm_free(mlist->list);
#else
                                delete mlist->list;                  //Borramos la lista
#endif
				mlist->lastelement->s=NULL;//Apuntamos el siguiente del ultimo elemento al ultimo
				mlist->list=mlist->firstelement;//Nos posicionamos en el primer elemento
			}
			else//Estamos en medio
			{
				temp=mlist->list;//guardamos en temp el elemento a borrar
				SetElement(mlist,id-1);//posicionamos a mlist en elemento anterior
				mlist->list->s=temp->s;//Pasamos el elemento siguiente de quien vamos a borrar al elemento anterior
				temp->s=NULL;//No muy necesario
                                DeleteElementData(temp->data.pData);//Borramos los datos
#ifdef _VIRTUAL_MEMORY
                                vm_free(temp);
#else
                                delete temp;                  //Borramos la lista
#endif
				mlist->list=mlist->firstelement;//Nos posicionamos en el primer elemento
			};
			mlist->NumElements--;
                        RebuildIndex(mlist);
			return true;
		}
		else if(mlist->NumElements==0)//Si la lista solo tiene un elemento
		{
                        DeleteElementData(mlist->list->data.pData);//Borramos los datos
                        mlist->list->data.pData =NULL;
                        RebuildIndex(mlist);
			/*Solamente borramos la informacion asshole
			delete mlist->list;//Borramos la lista
			mlist->list=NULL;//y ponemos la lista en ceros y los demas elementos
			mlist->firstelement=NULL;
			mlist->lastelement=NULL;
			mlist->NumElements=0;
			*/
			return true;
		
		}
		else
		{
			return false;
		};
	}
	else
	{
		return false;
	};

				
			

};

bool CLinkedList::SetElement(LPLINKEDLIST mlist,dword id)
{
        dword cont=0;
	//Nos posicionamos especificamente en un elemento
	if(id>mlist->NumElements)return false;//Si ID es mayor a el numero de elementos
										 //el elemento no existe			
	if(!mlist)return false;

	mlist->list=mlist->firstelement;//Nos posicionamos en el primer elemento


	while(cont<id)//mientras el contador sea menor que id
	{
		mlist->list=mlist->list->s;//nos pasamos al siguiente elemento de la lista
		if(mlist->list==NULL)return false;//si es nulo no se encontro el dato
		cont++;
	};
        return true;//DONE
};
	
//BORRaMOS TODA LA LISTA y ponemos en ceros el parametro
bool CLinkedList::DeleteList(LPLINKEDLIST *lpmlist)
{
	LPLINKEDLIST mlist=*lpmlist;//Obtenemos la lista
	LPDYNLINKLIST temp;

	if(!mlist)return false;

	mlist->list=mlist->firstelement;//Nos posicionamos en el primer elemento

	while(mlist->list!=NULL)//Mientras list no sea nulo
	{
		temp=mlist->list;//pasamos a list al temporal
		mlist->list=mlist->list->s;//nos posicionamos en el siguiente elemento
                DeleteElementData(temp->data.pData);//Borramos los datos
#ifdef _VIRTUAL_MEMORY
                vm_free(temp);
#else
                delete temp;                  //Borramos la lista
#endif
	};
	//Ponemos en ceros todo
	mlist->firstelement=NULL;
	mlist->lastelement=NULL;
#ifdef _VIRTUAL_MEMORY
               vm_free(mlist);
#else
               delete mlist;                  //Borramos la lista enlazada
#endif
	*lpmlist=NULL;//Regresamos 0
	return true;
};

//Obtenemos el elemento especificado

void *CLinkedList::GetElementData(LPLINKEDLIST mlist,dword id)
{
	if(!mlist)return NULL;
	if(!mlist->firstelement)return NULL;

    if(!SetElement(mlist,id))return NULL;//Nos posicionamos en el sitio especifico

        return mlist->list->data.pData;//Regresamos el dato
};

//Remplazamos un dato especificado
bool CLinkedList::SetElementData(LPLINKEDLIST mlist,dword id,void *ndata)
{
	if(!mlist)return false;
	if(!mlist->firstelement)return false;

	if(!SetElement(mlist,id))return false;//Establecemos el datos

        DeleteElementData(mlist->list->data.pData);//Borramos el dato anterior
        mlist->list->data.pData=ndata;//Apuntamos al nuevo dato
	return true;
};

dword CLinkedList::GetNumOfElements(LPLINKEDLIST list)
{

	if(!list)return false;
	if(!list->firstelement)return false;

	return list->NumElements+1;//Regresamos mas uno por el indice 0
};
//Nos pasamos al siguiente elemento

bool CLinkedList::ForwardElement(LPLINKEDLIST mlist)
{
	if(!mlist)return false;

	if(mlist->list==NULL)return false;

	mlist->list=mlist->list->s;
	return true;
};
//Nos ponemos en el primer elemento
bool CLinkedList::SetFirstElement(LPLINKEDLIST mlist)
{
	if(!mlist)return false;
	if(mlist->firstelement==NULL)return false;
	mlist->list=mlist->firstelement;
	return true;
};
//Obtenemos el elemento que apunta mlist
void *CLinkedList::GetCurrElementData(LPLINKEDLIST mlist)
{
	if(!mlist)return NULL;
	if(!mlist->list)return NULL;

        return mlist->list->data.pData;
};
//Mostramos si la lista no tiene datos
bool CLinkedList::IsEmpty(LPLINKEDLIST mlist)
{
    if(!mlist)return true;
    if(!mlist->firstelement)return true;
	
    if(mlist->firstelement==mlist->lastelement && mlist->firstelement->data.pData==NULL)
	{

		return true;
	}
	else
	{
		return false;
	}
};

bool CLinkedList::SetLastElement(LPLINKEDLIST mlist)
{
	if(!mlist)return false;
	if(!mlist->list)return false;
	mlist->list=mlist->lastelement;
	return true;
};


//Functions added Feb 2010

void CLinkedList::MoveElementDown(LPLINKEDLIST pList, dword dwIndex)
{
	LPDYNLINKLIST pTempFirst,pTempCurrent,pTempLast;//For temporal data
        dword dwCount=0;//Our counter
	//check out list data
	//Means no data
	if(!pList->firstelement) return;

	if(dwIndex>pList->NumElements)return;//Out of range
	if(dwIndex==pList->NumElements)return;//Not applicable
	if(pList->NumElements==1)return;//No applicable only 1 element

	////Do the recorrido
	pList->list=pList->firstelement;//Set to first element
	//Check counter
	if(dwIndex==0)//We attempt to move the first element
	{

		//No need to get last element
	
		pTempCurrent=pList->list;
		pTempLast=pList->list->s;

		//Swap them
		pTempCurrent->s=pTempLast->s;//Point to Last next element
		pTempLast->s=pTempCurrent;//Point next element to current

		//Save the list
		pList->firstelement=pTempLast;//now pTempLast is the first element and current the next
		pList->list=pList->firstelement;//Set to first
                //Rebuild the the index
                RebuildIndex(pList);
		return; //End of function

    }


		
    if (SetElement(pList,dwIndex-1))
	{
		//Position on anterior
		//set anterior
		pTempFirst=pList->list;
		//Set the desired
		pTempCurrent=pList->list->s;
		//Set the last
		pTempLast=pTempCurrent->s;

		//Now first most point to last
		pTempFirst->s=pTempLast;
		//Next of Current to next of last
		pTempCurrent->s=pTempLast->s;
		//and Nexto of last to current
		pTempLast->s=pTempCurrent;

		pList->list=pTempCurrent;//Point to the moved element

			//Next case we are on the last element
		if(dwIndex==pList->NumElements-1)//The element was above the last element now is the last element
		{
		
			pList->lastelement=pTempCurrent;


		};

               //Rebuild the the index
                 RebuildIndex(pList);
		//Element moved

	}

}
   


//MoveElementTop

void CLinkedList::MoveElementTop(LPLINKEDLIST pList, dword dwIndex)
{
    //Restrictions
	if(dwIndex==0) return;//Not applicable
	MoveElementDown(pList,dwIndex-1);
         RebuildIndex(pList);

}

int CLinkedList::GetCurrentIndex(LPLINKEDLIST pList)
{
    if(pList->list==NULL)return -1;

    return pList->list->data.dwIndex;
}

//Even more
void CLinkedList::MarkForDeletion(LPLINKEDLIST pList, dword dwIndex)
{

    //
    if(SetElement(pList,dwIndex))
    {
        DeleteElementData(pList->list->data.pData);
        pList->list->data.pData=NULL;//Setting for deletion
    }
}

void CLinkedList::MarkCurrentForDeletion(LPLINKEDLIST pList)
{
    if(pList->list==NULL) return;//current is null
    DeleteElementData(pList->list->data.pData);
    pList->list->data.pData=NULL;//Marked for deletion
}

//Delete Elements marked for deletion
void CLinkedList::DeleteMarked(LPLINKEDLIST mlist)
{

    DYNLINKLIST *lastfound=NULL,*temp;

    mlist->list=mlist->firstelement;//Set to first element
    while(mlist->list!=NULL)
    {

        if(mlist->list->data.pData==NULL)
        {
            if(mlist->NumElements>0)//Si tenemos mas de un elemento
            {
                    if(mlist->list==mlist->firstelement)//Si elemento a borrar es igual al primer elemento
                    {
                            mlist->firstelement=mlist->list->s;//Pasamo el apuntar el firstelement al siguiente elemento

#ifdef _VIRTUAL_MEMORY
                            vm_free(mlist->list);
#else
                            delete mlist->list;                  //Borramos la lista
#endif
                            mlist->list=mlist->firstelement;



                    }
                    if(mlist->lastelement==mlist->list)//Si elemento a borrar es el ultimo
                    {
                        if(mlist->lastelement==mlist->firstelement)
                        {
                           //Do nothing because the first element is the last one
                            mlist->list=NULL;//End of list
                            mlist->NumElements=0;
                            return;//bye
                        }
                        else
                        {
                            //Nos posicionamos en uno anterior
                            mlist->lastelement=lastfound;//Apuntamos lastelement al anterior posicionado


                            mlist->list->s=NULL;//No muy necesario
#ifdef _VIRTUAL_MEMORY
                            vm_free(mlist->list);
#else
                            delete mlist->list;                  //Borramos la lista
#endif
                            mlist->lastelement->s=NULL;//Apuntamos el siguiente del ultimo elemento al ultimo
                            mlist->list=mlist->lastelement;

                        }


                    }
                    if((mlist->list!=mlist->firstelement)&&(mlist->list!=mlist->lastelement))//Estamos en medio
                    {
                            temp=mlist->list;//guardamos en temp el elemento a borrar
                            mlist->list=lastfound;//nos posicionamos en 1 antes
                            mlist->list->s=temp->s;//Pasamos el elemento siguiente de quien vamos a borrar al elemento anterior
                            temp->s=NULL;//No muy necesario

#ifdef _VIRTUAL_MEMORY
                             vm_free(temp);
#else
                             delete temp;                  //Borramos la lista
#endif

                    };
                    mlist->NumElements--;


            }
            if(mlist->NumElements==0)//Si la lista solo tiene un elemento
            {

                mlist->list=NULL;
                return;

            }
            lastfound=mlist->list;


        }
        else
        {
            lastfound=mlist->list;
            mlist->list=mlist->list->s;

        }


    }
    RebuildIndex(mlist);
}

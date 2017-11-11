#include "gVmAsm.h"

//Funciones de destruccion de listas
namespace gfw
{
void CleanParam(void *pData)
{
    LPPARAM pParam=(LPPARAM)pData;
	if(!pParam)return;
	delete pParam;//Borramos el parametros
}


//CleanIns
void CleanIns(void *pData)
{
	LPEASMINS pIns=(LPEASMINS)pData;
	if(!pIns)return;
	delete pIns;//Borra la instruccion
}

//CleanProc
void CleanProc(void *pData)
{
	LPLBPROC pProc=(LPLBPROC)pData;
	if(!pProc)return;
	delete pProc;
}
//CleanVars
void CleanVars(void *pData)
{
	LPEVAR pVar=(LPEVAR)pData;
	if(!pVar)return;
	delete pVar;
}
//CleanErr
void CleanErr(void *pData)
{
    gString *szErr=(gString *)pData;
    delete szErr;
}
}

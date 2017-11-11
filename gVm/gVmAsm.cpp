// evmasm.cpp : Defines the entry point for the DLL application.
//


#include "gVmAsm.h"
#include <gBaseArray.h>
//#include <gFile.h>
#include "gVmAsmConst.h"
using namespace gfw;

/***************************Open ASMFILE***************/
bool gVmAsm::OpenVmAsmFile(const gString &fileName)
{
    bool ret;
    //return OpenFileForTK(szFileName);
    ret=openScript(fileName);
    if(ret)
        pToken=(gToken *)&token();
    return ret;
}
/********************Assamle**************************/
bool gVmAsm::Assamble(const gString &FileName)
{
	EPROGHEADER ph;//Cabecera del archivo
	EPROGINFO pi;//Informacion del archivo
	FILE *pF;//Apuntador al archivo
    EASMINS *pIns;//Instruccion
    PARAM *pParam;//PArametro
    gu32 *pdwMagic=(gu32 *)&Magic;
    gu32 *pdwData;
    gu32 i=0;//Contador
    gByteArray program; //Program memory
    gu8 *pMem;//Memoria de programa
    gu8 bFlag=0,byData;//Flag
    gu32 dwVar;//Variable
	float fVar;//Variable flotante
    EVAR *pVar;
    LBPROC *pLabel=NULL;
    gString szMsg;
	//Analizamos el codigo
	ParseCode();
	//Verificamos si hay errores
    if(!errlist.isEmpty())return false;
	//Verificamos etiquetas y procedimientos
	if(!VerifyLabelsAndProcs())
		return false;
	if(!VerifyApiDecls())//Verificamos que todas las llamadas API fueron declaradas
		return false;
	//Verificamos que existe el punto de entrada
	pLabel=GetProcMain();
	if(!pLabel)
	{
		szMsg="Can not find entry point MAIN";
        errlist << new gString(szMsg);
		return false;
	};

	//Verificamos el stack y el heap que no sean cero
	if(dwHeapSize==0)
        dwHeapSize=1024000;//1 megagu8 default
	if(dwStackSize==0)
        dwStackSize=65535;//64 kilogu8s
	//Abrimos el archivo para escritura
    pF=fopen(FileName.ascii(),"wb");
	if(!pF)
		return false;//No se pudo abrir para escritura
	ph.dwMagic=*pdwMagic;//Tipo de archivo
	ph.wVersion =0x1000;//Version
    pi.dwProgramSize =dwPC;//Tamano del programa
	pi.dwStackSize =dwStackSize;//Stack
	pi.dwVMSize =dwHeapSize;//Heap
	pi.dwStarAddress =pLabel->dwAddress; 
	//Guardamos las cabeceras
	fwrite(&ph,sizeof(EPROGHEADER),1,pF);
	fwrite(&pi,sizeof(EPROGINFO),1,pF);
	//Asignamos memoria para el programa
    program.alloc(dwPC);
    pMem=program.data();
	pLabel=NULL;
   

	//Recorremos la lista de instrucciones
    ins.first();
    while((pIns=(LPEASMINS)ins.data())!=NULL)
    {
        pMem[i]=(gu8)pIns->wOpcode;//Instruccion
		i++;//Se incrementa i
		//Vamos a los parametros
        pIns->lParams.first();
        while((pParam=(LPPARAM)pIns->lParams.data())!=NULL)
		{
			if(pParam->wAddressMode ==ADDR_INDIRECT)
			{
                BYTE_H_WRITE(bFlag,1);
			}
			else
			{
                BYTE_H_WRITE(bFlag,0);
			}
			
			switch(pParam->wParamType)
			{
			case KEY_RACC://Registro acumulador
				//Indicamos con el flag que es un registro
                BYTE_L_WRITE(bFlag,0);
				
				//Guardamos bFlag
				pMem[i]=bFlag;
				i++;//Incrementamos i
                //Obtenemos el apuntador de 4 gu8s
				
                pdwData=(gu32 *)&pMem[i];
				i+=4;
				//Guardamos 0 que el indicador de registro RACC
				*pdwData=0;
				break;
			case KEY_RC://Registro de comparacion
				//Indicamos con el flag que es un registro
                BYTE_L_WRITE(bFlag,0);
			
				//Guardamos bFlag
				pMem[i]=bFlag;
                //Obtenemos el apuntador de 4 gu8s
				i++;//Incrementamos i
                pdwData=(gu32 *)&pMem[i];
                i+=4;//Incrementamos el tamano de 4
				//Guardamos 1 que el indicador de registro RC
				*pdwData=1;
				break;
			case KEY_RGEN://Registro general
				//Indicamos con el flag que es un registro
                BYTE_L_WRITE(bFlag,0);
				//Incrementamos i
				
				//Guardamos bFlag
				pMem[i]=bFlag;
                //Obtenemos el apuntador de 4 gu8s
				i++;//Incrementamos i
                pdwData=(gu32 *)&pMem[i];
				i+=4;
				//Guardamos 2 que el indicador de registro RGEN
				*pdwData=2;
				break;
			case PARAM_TYPE_VAR://Una variable
                //Escribimos el gu8 bajo
                BYTE_L_WRITE(bFlag,1);//1 es una variable
				
				//Guardamos bFlag
				pMem[i]=bFlag;
				i++;//incrementamos i de nuevo
                pdwData=(gu32 *)&pMem[i];
				i+=4;
				//Guardamos el valor de la variable
				*pdwData=GetVarIndex(pParam->szName); 
				
				break;
            case KEY_STACK:
                BYTE_L_WRITE(bFlag,2);
                pMem[i]=bFlag;
                i++;
                pdwData=(gu32 *)&pMem[i];
                i+=4;
                //for now the stack offset is given by a fixed value other is not valid
                *pdwData=pParam->offset.value.toUInt();
                break;


			case PARAM_TYPE_LABEL:
				//Suponemos que siempre es una direccion de programa
				pLabel=GetLabelProc(pParam->szName); 
				
                pdwData=(gu32 *)&pMem[i];
				i+=4;
				//Guardamos la direccion
				*pdwData=pLabel->dwAddress;
				break;
			case PARAM_TYPE_APICALL:
				//Es una direccion API
				pLabel=GetApiProc(pParam->szName); 
				
                pdwData=(gu32 *)&pMem[i];
				i+=4;
				//Guardamos la direccion
				*pdwData=pLabel->dwAddress;
				break;

			case PARAM_TYPE_NUMLABEL:
                pdwData=(gu32 *)&pMem[i];
				i+=4;
				//Guardamos la direccion
                *pdwData=pParam->szName.toUInt();
				break;

			case TOKEN_TYPE_NUMINT://Numerico
                //Escribimos el gu8 bajo
                BYTE_L_WRITE(bFlag,2);//2 es una constante
				
				//Guardamos bFlag
				pMem[i]=bFlag;
				i++;//incrementamos i de nuevo
                pdwData=(gu32 *)&pMem[i];
				i+=4;
				//Guardamos el valor de la constante
                *pdwData=(gu32)pParam->szName.toInt();
				break;
			case TOKEN_TYPE_NUMFLOAT://Numerico de punto flotante
                BYTE_L_WRITE(bFlag,2);//2 es una constante
				
				//Guardamos bFlag
				pMem[i]=bFlag;
				i++;//incrementamos i de nuevo
                pdwData=(gu32 *)&pMem[i];
				i+=4;
				//Guardamos el valor de la constante
                *pdwData=(gu32)pParam->szName.toFloat();
				break;
			};
            pIns->lParams.next();
			bFlag=0;//Ponemos en 0 bFlag
		}
            ins.next();
	}
	//Guardamos el programa
	for(i=0;i<dwPC;i++)
	{
        fwrite(&pMem[i],sizeof(gu8),1,pF);
	}
	//Liberamos memoria de programa
    //program.HeapMemFree();
    program.clear();
	//Guardamos las variables
	//Guardamos la cantidad de variables
    dwVar=vars.count() ;
	fwrite(&dwVar,sizeof(long),1,pF);
    vars.first();
    while((pVar=(LPEVAR)vars.data())!=NULL)
	{
		switch(pVar->dwVarType)
		{
		case KEY_INT:
            dwVar=(gu32)pVar->szVal.toInt();
			byData=0;//Entero
			fwrite(&byData,sizeof(char),1,pF);
			fwrite(&dwVar,sizeof(long),1,pF);
			break;
		case KEY_FLOAT:
            fVar=pVar->szVal.toFloat();
			byData=1;//Entero .flotante
			fwrite(&byData,sizeof(char),1,pF);
			fwrite(&fVar,sizeof(long),1,pF);
			break;
		case KEY_STRING://Cadena de caracteres
			byData=2;//Cadena de caracteres
            dwVar=pVar->szVal.size();
			fwrite(&byData,sizeof(char),1,pF);
            //Tamano
			fwrite(&dwVar,sizeof(long),1,pF);
			//La cadena de caracteres
            fwrite(pVar->szVal.ascii(),sizeof(char),dwVar,pF);
			break;
		}
        vars.next();
	}
	fclose(pF);
	//Cerramos el archivo
	return true;
}

/************************Assamble11**********************************/
bool gVmAsm::Assamble11(const gString &FileName)
{
	EPROGHEADER ph;//Cabecera del archivo
	EPROGINFO pi;//Informacion del archivo
	FILE *pF;//Apuntador al archivo
    EASMINS *pIns;//Instruccion
    PARAM *pParam;//PArametro
    gu32 *pdwMagic=(gu32 *)&Magic;
    gu32 *pdwData;
    gu32 i=0;//Contador
    gByteArray program; //Program memory
    gu8 *pMem;//Memoria de programa
    gu8 bFlag=0,byData;//Flag
    gu32 dwVar;//Variable
	float fVar;//Variable flotante
    EVAR *pVar;
    LBPROC *pLabel=NULL;
    gString szMsg;
	//Added to version 1.1
    gu16 wData;//Just gu16 data
	//Analizamos el codigo
	ParseCode();
	//Verificamos si hay errores
    if(!errlist.isEmpty())return false;
	//Verificamos etiquetas y procedimientos
    if(!VerifyLabelsAndProcs())
		return false;
	if(!VerifyApiDecls())//Verificamos que todas las llamadas API fueron declaradas
		return false;
	//Verificamos que existe el punto de entrada
	pLabel=GetProcMain();
	if(!pLabel)
	{
		szMsg="Can not find entry point MAIN";
        errlist << new gString(szMsg);
		return false;
	};

	//Verificamos el stack y el heap que no sean cero
	if(dwHeapSize==0)
        dwHeapSize=1024000;//1 megagu8 default
	if(dwStackSize==0)
        dwStackSize=65535;//64 kilogu8s
	//Abrimos el archivo para escritura
    pF=fopen(FileName.ascii(),"wb");
	if(!pF)
		return false;//No se pudo abrir para escritura
	ph.dwMagic=*pdwMagic;//Tipo de archivo
	ph.wVersion =0x1100;//Version change 1.1
    pi.dwProgramSize =dwPC;//Tamano del programa
	pi.dwStackSize =dwStackSize;//Stack
	pi.dwVMSize =dwHeapSize;//Heap
	pi.dwStarAddress =pLabel->dwAddress; 
	//Guardamos las cabeceras
	fwrite(&ph,sizeof(EPROGHEADER),1,pF);
	fwrite(&pi,sizeof(EPROGINFO),1,pF);
	//Asignamos memoria para el programa
    program.alloc(dwPC);
    pMem=program.data();
	pLabel=NULL;
	//
//******************Implementacion a la version de archivo 1.1******************************
	//Guardamos la cantidad de procedimientos
	dwVar=GetProcCount();
    fwrite(&dwVar,sizeof(gu32),1,pF);
    procs.first();//Recorremos al principio del la list
	//La intencion es guardar todos los procedimientos en el archivo para poder 
	//hacer llamadas directas a funcion
    while((pLabel=(LPLBPROC)procs.data())!=NULL)
	{
		//reocorremos la lista
		if(pLabel->wType==PROC_TYPE_PROC)//Solamente nos concentramos en procedimientos
		{
			//Una vez obtenido disponemos a guardar el identificador
            //primero el tamano de la cadena
            wData=(gu16 )pLabel->szLabelProcName.size();
            fwrite(&wData,sizeof(gu16),1,pF);
			//Despues la cadena
            fwrite(pLabel->szLabelProcName.ascii(),sizeof(char),wData,pF);
			//Guardamos despues la direccion de memoria
            fwrite(&pLabel->dwAddress,sizeof(gu32),1,pF);

        }
        procs.next();

	}

	//una vez guardado reseteamos pLabel
	pLabel=NULL;
	/*************************************************************************/
	//Recorremos la lista de instrucciones
    ins.first();
    while((pIns=(LPEASMINS)ins.data())!=NULL)
	{
        pMem[i]=(gu8)pIns->wOpcode;//Instruccion
		i++;//Se incrementa i
		//Vamos a los parametros
        pIns->lParams.first();
        while((pParam=(LPPARAM)pIns->lParams.data())!=NULL)
		{
			if(pParam->wAddressMode ==ADDR_INDIRECT)
			{
                BYTE_H_WRITE(bFlag,1);
			}
			else
			{
                BYTE_H_WRITE(bFlag,0);
			}
			
			switch(pParam->wParamType)
			{
			case KEY_RACC://Registro acumulador
				//Indicamos con el flag que es un registro
                BYTE_L_WRITE(bFlag,0);
				
				//Guardamos bFlag
				pMem[i]=bFlag;
				i++;//Incrementamos i
                //Obtenemos el apuntador de 4 gu8s
				
                pdwData=(gu32 *)&pMem[i];
				i+=4;
				//Guardamos 0 que el indicador de registro RACC
				*pdwData=0;
				break;
			case KEY_RC://Registro de comparacion
				//Indicamos con el flag que es un registro
                BYTE_L_WRITE(bFlag,0);
			
				//Guardamos bFlag
				pMem[i]=bFlag;
                //Obtenemos el apuntador de 4 gu8s
				i++;//Incrementamos i
                pdwData=(gu32 *)&pMem[i];
                i+=4;//Incrementamos el tamano de 4
				//Guardamos 1 que el indicador de registro RC
                *pdwData=1;
				break;
			case KEY_RGEN://Registro general
				//Indicamos con el flag que es un registro
                BYTE_L_WRITE(bFlag,0);
				//Incrementamos i
				
				//Guardamos bFlag
				pMem[i]=bFlag;
                //Obtenemos el apuntador de 4 gu8s
				i++;//Incrementamos i
                pdwData=(gu32 *)&pMem[i];
				i+=4;
				//Guardamos 2 que el indicador de registro RGEN
				*pdwData=2;
				break;
            case KEY_RP:
                //Indicamos con el flag que es un registro
                BYTE_L_WRITE(bFlag,0);
                //Incrementamos i

                //Guardamos bFlag
                pMem[i]=bFlag;
                //Obtenemos el apuntador de 4 gu8s
                i++;//Incrementamos i
                pdwData=(gu32 *)&pMem[i];
                i+=4;
                //Guardamos 2 que el indicador de registro RGEN
                *pdwData=3;//Address 3 pointer register
                break;

			case PARAM_TYPE_VAR://Una variable
                //Escribimos el gu8 bajo
                BYTE_L_WRITE(bFlag,1);//1 es una variable
				
				//Guardamos bFlag
				pMem[i]=bFlag;
				i++;//incrementamos i de nuevo
                pdwData=(gu32 *)&pMem[i];
				i+=4;
				//Guardamos el valor de la variable
				*pdwData=GetVarIndex(pParam->szName); 
				
				break;
            case KEY_STACK:
                BYTE_L_WRITE(bFlag,3);
                pMem[i]=bFlag;
                i++;
                pdwData=(gu32 *)&pMem[i];
                i+=4;
                //for now the stack offset is given by a fixed value other is not valid
                if(pParam->offset.offsettype==gvmasm_offset::OFFSET_NUMERIC && !pParam->offset.value.isEmpty())
                    *pdwData=pParam->offset.value.toUInt();
                else
                    *pdwData=0;
                break;
             case KEY_STACKDIRECT:
                BYTE_L_WRITE(bFlag,5);
                pMem[i]=bFlag;
                i++;
                pdwData=(gu32 *)&pMem[i];
                i+=4;
                if(pParam->offset.offsettype==gvmasm_offset::OFFSET_NUMERIC && !pParam->offset.value.isEmpty())
                    *pdwData=pParam->daddress + pParam->offset.value.toUInt();
                else
                    *pdwData=pParam->daddress;
                break;
            case KEY_STACKPOINTER:
                BYTE_L_WRITE(bFlag,4);
                pMem[i]=bFlag;
                i++;
                pdwData=(gu32 *)&pMem[i];
                i+=4;
                *pdwData=0;//not really important just to fill the gap as the stack pointer is private variable
                break;
			case PARAM_TYPE_LABEL:
				//Suponemos que siempre es una direccion de programa
				pLabel=GetLabelProc(pParam->szName); 
				
                pdwData=(gu32 *)&pMem[i];
				i+=4;
                //Guardamos la direccion
				*pdwData=pLabel->dwAddress;
				break;
			case PARAM_TYPE_APICALL:
				//Es una direccion API
				pLabel=GetApiProc(pParam->szName); 
				
                pdwData=(gu32 *)&pMem[i];
				i+=4;
				//Guardamos la direccion
				*pdwData=pLabel->dwAddress;
				break;

			case PARAM_TYPE_NUMLABEL:
                pdwData=(gu32 *)&pMem[i];
				i+=4;
				//Guardamos la direccion
                *pdwData=pParam->szName.toUInt();
				break;

			case TOKEN_TYPE_NUMINT://Numerico
                //Escribimos el gu8 bajo
                BYTE_L_WRITE(bFlag,2);//2 es una constante
				
				//Guardamos bFlag
				pMem[i]=bFlag;
				i++;//incrementamos i de nuevo
                pdwData=(gu32 *)&pMem[i];
				i+=4;
				//Guardamos el valor de la constante
                *pdwData=(gu32)pParam->szName.toInt();
				break;
			case TOKEN_TYPE_NUMFLOAT://Numerico de punto flotante
                BYTE_L_WRITE(bFlag,2);//2 es una constante
				
				//Guardamos bFlag
				pMem[i]=bFlag;
				i++;//incrementamos i de nuevo
                pdwData=(gu32 *)&pMem[i];
				i+=4;
				//Guardamos el valor de la constante
                *pdwData=(gu32)pParam->szName.toFloat();
				break;
			};
            pIns->lParams.next();
			bFlag=0;//Ponemos en 0 bFlag
		}
        ins.next();
	}
	//Guardamos el programa
	for(i=0;i<dwPC;i++)
	{
        fwrite(&pMem[i],sizeof(gu8),1,pF);
	}
	//Liberamos memoria de programa
    program.clear();
	//Guardamos las variables
	//Guardamos la cantidad de variables
    dwVar=vars.count() ;
	fwrite(&dwVar,sizeof(long),1,pF);
    vars.first();
    while((pVar=(LPEVAR)vars.data())!=NULL)
	{
		switch(pVar->dwVarType)
		{
		case KEY_INT:
            dwVar=(gu32)pVar->szVal.toInt();
			byData=0;//Entero
			fwrite(&byData,sizeof(char),1,pF);
			fwrite(&dwVar,sizeof(long),1,pF);
			break;
		case KEY_FLOAT:
            fVar=pVar->szVal.toFloat();
			byData=1;//Entero .flotante
			fwrite(&byData,sizeof(char),1,pF);
			fwrite(&fVar,sizeof(long),1,pF);
			break;
		case KEY_STRING://Cadena de caracteres
			byData=2;//Cadena de caracteres
            dwVar=pVar->szVal.size();
			fwrite(&byData,sizeof(char),1,pF);
            //Tamano
			fwrite(&dwVar,sizeof(long),1,pF);
			//La cadena de caracteres
            fwrite(pVar->szVal.ascii(),sizeof(char),dwVar,pF);
			break;
		}
        vars.next();
	}
	fclose(pF);
	//Cerramos el archivo
	return true;
}

/***********************InitErrTour********************/
void gVmAsm::InitErrTour()
{
    errlist.first();
}

/************************GetError***********************/

const gString &gVmAsm::GetError() const
{
    gString *szErr;
    szErr=(gString *)errlist.data();
    errlist.next();
    return (const gString &)(*szErr);
}

bool gVmAsm::endOfErrors() const
{
    return errlist.data()==0;
}


void gVmAsm::clearAll()
{
    errlist.clear();
    vars.clear();
    procs.clear();
    ins.clear();
    dwHeapSize=0;
    dwStackSize=0;
    dwPC=0;
    clear();//lexer clear

}


	
	

	


	

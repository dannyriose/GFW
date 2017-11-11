#include "gVmAsm.h"
#include "gVmAsmConst.h"
#include "stdlib.h"
#include "string.h"
#include <gString/gStringList.h>
//Clean functions
using namespace gfw;
//Added new

EVAR *LBPROC::getVariable(const gString &val){
    EVAR *r;
    local.first();
    while((r=local.pvalue())!=0){
        if(r->szVarName==val){
            return r;
        }
        local.next();
    }
    return 0;
}

/***************************GetReservedWord******************/
bool gVmAsm::GetReservedWord()
{
    if(pToken->tokenID1!=TOKEN_TYPE_IDENT)return false;
	//Convertimos a minusculas
    //strlwr(pToken->tokenString);
    pToken->tokenString.thisToLowCase();
	GET_KWORD("mov",I_MOV);
	GET_KWORD("movb",I_MOVB);
	GET_KWORD("add",I_ADD);
	GET_KWORD("mul",I_MUL);
	GET_KWORD("sub",I_SUB);
	GET_KWORD("div",I_DIV);
	GET_KWORD("fadd",I_FADD);
	GET_KWORD("fmul",I_FMUL);
	GET_KWORD("fdiv",I_FDIV);
	GET_KWORD("fsub",I_FSUB);
	GET_KWORD("and",I_AND);
	GET_KWORD("or",I_OR);
	GET_KWORD("xor",I_XOR);
	GET_KWORD("not",I_NOT);
	GET_KWORD("cmp",I_CMP);
	GET_KWORD("je",I_JE);
	GET_KWORD("jne",I_JNE);
	GET_KWORD("jl",I_JL);
	GET_KWORD("jg",I_JG);
    GET_KWORD("jle",I_JLE);
    GET_KWORD("jge",I_JGE);
	GET_KWORD("jmp",I_JMP);
	GET_KWORD("push",I_PUSH);
	GET_KWORD("pop",I_POP);
	GET_KWORD("neg",I_NEG);
	GET_KWORD("fneg",I_FNEG);
	GET_KWORD("call",I_CALL);
	GET_KWORD("ret",I_RET);
	GET_KWORD("calle",I_CALLE);
	GET_KWORD("halt",I_HALT);
	GET_KWORD("stack_size",KEY_STACKSIZE);
	GET_KWORD("vars",KEY_VARS);
	GET_KWORD("heap_size",KEY_HEAPSIZE);
	GET_KWORD("main",KEY_MAIN);
	GET_KWORD("proc",KEY_PROC);
	GET_KWORD("int",KEY_INT);
	GET_KWORD("float",KEY_FLOAT);
	GET_KWORD("string",KEY_STRING);
	GET_KWORD("racc",KEY_RACC);
	GET_KWORD("rc",KEY_RC);
    GET_KWORD("rp",KEY_RP);
	GET_KWORD("rgen",KEY_RGEN);
	GET_KWORD("api_call",KEY_APICALL);
	GET_KWORD("include",KEY_INCLUDE);
	//Added FCMP
	GET_KWORD("fcmp",I_FCMP);
    GET_KWORD("cmp8",I_CMP8);
    GET_KWORD("cmp16",I_CMP16);
    GET_KWORD("mov8",I_MOV8);
    GET_KWORD("mov16",I_MOV16);
    //Added Alloc
    GET_KWORD("alloc",I_ALLOC);
    GET_KWORD("free",I_FREE);
    GET_KWORD("and8",I_AND8);
    GET_KWORD("and16",I_AND16);
    GET_KWORD("or8",I_OR8);
    GET_KWORD("or16",I_OR16);
    GET_KWORD("not8",I_NOT8);
    GET_KWORD("not16",I_NOT16);
    GET_KWORD("xor8",I_XOR8);
    GET_KWORD("xor16",I_XOR16);
    GET_KWORD("inc",I_INC);
    GET_KWORD("dec",I_DEC);
    GET_KWORD("stack",KEY_STACK);
    GET_KWORD("sp",KEY_STACKPOINTER);
    GET_KWORD("var",KEY_VAR);
    GET_KWORD("d_stack",KEY_STACKDIRECT);
	return false;
}
/***************************WriteError***************************/
void gVmAsm::WriteError(const gString &szError)
{
    gString *szErr;
    if(szError.isEmpty())return;
    szErr =new gString(szError);

    (*szErr)+=" ";
    (*szErr)+=pToken->tokenString;


    (*szErr)+=" Line ";
    (*szErr)+=gString::number(line());
    (*szErr)+=" Col ";
    (*szErr)+=gString::number(column());
    errlist.append(szErr);//Regresamos el buffer copiado a la cadena
};

	

/****************************IsVariable*************************/
bool gVmAsm::IsVariable()
{
    EVAR *pVars;

    vars.first();//Posicionamos en el primer elemento
	
    while((pVars=(EVAR*)vars.data())!=NULL)
	{
        if(pVars->szVarName==pToken->tokenString)
		{
			return true;
		}
        vars.next();
	}
	return false;
}
/**************************IsVariable 2***********************/
bool gVmAsm::IsVariable(const gString &szStr)
{
    EVAR *pVars;

    vars.first();//Posicionamos en el primer elemento
	
    while((pVars=(LPEVAR)vars.data())!=NULL)
	{
		if(pVars->szVarName==szStr)
		{
			return true;
		}
        vars.next();
	}
	return false;
}
/************************IsApiProc*******************************/
bool gVmAsm::IsApiProc(const gString &szStr)
{
    LBPROC *pProc;

    apidecl.first();//Primera posicion
	
    while((pProc=(LPLBPROC)apidecl.data())!=NULL)
	{
		if(pProc->szLabelProcName==szStr)
		{
			return true;
		}
        apidecl.next();
	}
	return false;
}
/************************IsLabelProc*****************************/
bool gVmAsm::IsLabelProc()
{
    LBPROC *pProc;

    procs.first();//Primera posicion
	
    while((pProc=(LPLBPROC)procs.data())!=NULL)
	{
        if(pProc->szLabelProcName==pToken->tokenString)
		{
			return true;
		}
        procs.next();
	}
	return false;
}
/***********************IslabelProc*******************************/
bool gVmAsm::IsLabelProc(const gString &szStr)
{
    LBPROC *pProc;

    procs.first();//Primera posicion
	
    while((pProc=(LPLBPROC)procs.data())!=NULL)
	{
		if(pProc->szLabelProcName==szStr)
		{
			return true;
		}
                procs.next();
	}
	return false;
}
/**********************GetLabelProc************************/
LBPROC *gVmAsm::GetLabelProc(const gString &szStr)
{
    LBPROC *pProc;

    procs.first();//Primera posicion
	
    while((pProc=(LPLBPROC)procs.data())!=NULL)
	{
		if(pProc->szLabelProcName==szStr)
		{
			return pProc;
		}
        procs.next();
	}
	return NULL;
}

/********************GetProcCount Added for version 1.1*******************************/
gu32 gVmAsm::GetProcCount()
{
    gu32 i=0;//Counter
    LBPROC *pProc;

    procs.first();//Primera posicion
	
    while((pProc=(LBPROC*)procs.data())!=NULL)
	{
		if(pProc->wType=PROC_TYPE_PROC)
		{
			i++;
		}
        procs.next();
	}
	return i;;

}
/********************GetApiProc********************************/
LBPROC *gVmAsm::GetApiProc(const gString &szStr)
{
    LBPROC *pProc;

    apidecl.first();//Primera posicion
	
    while((pProc=(LPLBPROC)apidecl.data())!=NULL)
	{
		if(pProc->szLabelProcName==szStr)
		{
			return pProc;
		}
        apidecl.next();
	}
	return NULL;
}
/******************GetProcMain*********************************/
LBPROC *gVmAsm::GetProcMain()
{
    LBPROC *pProc;

    procs.first();//Primera posicion
	
    while((pProc=(LPLBPROC)procs.data())!=NULL)
	{
        pProc->szLabelProcName.thisToLowCase();
		if(pProc->szLabelProcName=="main")
		{
			return pProc;
		}
        procs.next();
	}
	return NULL;
}
	
/*****************GetVarIndex**********************************/
gu32 gVmAsm::GetVarIndex(const gString &szStr)
{
    EVAR *pVars;
    gu32 i;

    gu32 block_size=gVirtualMemory::blockHeaderSize();

    //first offset by default is the stack so
    //i=dwStackSize+block_size*2;
    i=0;

    vars.first();//Posicionamos en el primer elemento

    while((pVars=(LPEVAR)vars.data())!=NULL)
	{
		if(pVars->szVarName==szStr)
		{
            return i+block_size;
		}
        //Nos saltamos las variables que no sean
		switch(pVars->dwVarType)
		{
		case KEY_INT:
            i+=block_size+4;
			break;
		case KEY_FLOAT:
            i+=block_size+4;
			break;
		case KEY_STRING:
            i+=block_size+(pVars->szVal.size()+1);
			break;
        };
	
        vars.next();
	}
	return false;
}
/*****************************GetVarSizes***************/
gu32 gVmAsm::GetVarsSizes()
{
    EVAR *pVars;
    gu32 i=0;

    vars.first();//Posicionamos en el primer elemento
	
    while((pVars=(LPEVAR)vars.data())!=NULL)
	{
		switch(pVars->dwVarType)
		{
		case KEY_INT://4 bytes
			i+=4;
			break;
		case KEY_FLOAT:
			i+=4;   //4 bytes
			break;
		case KEY_STRING:
            i+=pVars->szVal.size();
			break;
		}
                vars.next();
	}
	return i;
}
	
//Constructor
gVmAsm::gVmAsm():dwStackSize(0),dwHeapSize(0),dwPC(0),procInside(0),dwSP(0)
{
    ins.setFunction(CleanIns);
    procs.setFunction(CleanProc);
    apidecl.setFunction(CleanProc);
    errlist.setFunction(CleanErr);
    vars.setFunction(CleanVars);



}
/****************************ParseCode********************/

void gVmAsm::ParseCode()
{
	//Principal

	bool bDone=false;

	while(!bDone)
	{
		ParseMain();
        getToken();

        switch(pToken->tokenID1)
		{
		case TOKEN_TYPE_ERROR:
            WRITE_ERR("InvalidToken");
		case TOKEN_TYPE_ENDOF_FILE:
			bDone=true;
			break;
		case TOKEN_TYPE_DOTCOMA:
			ParseComment();
			break;
		default:
            stepBack();
		};

	}
}


/************************ParseMain()****************************/

void  gVmAsm::ParseMain()
{

    getToken();

    switch(pToken->tokenID1)
	{
	case TOKEN_TYPE_IDENT:
		//Es palabra reservada
        if(GetReservedWord())
		{
		//Instruccion MOV
            if(pToken->tokenID2>=I_MOV && pToken->tokenID2<=I_MOV16)
			{
                ParseMOV();
				break;
			}
			//Instruccion MOVB
            if(pToken->tokenID2==I_MOVB)
			{
				ParseMOVB();
				break;
			}
			//Instrucciones de ADD a CMP (FCMP Added 4 Jan 2010)
			
            if((pToken->tokenID2>=I_ADD)&&(pToken->tokenID2<=I_XOR))
			{
                ParseABC();
				break;
			};
			
            //Instrucciones de NOT a ALLOC
            if((pToken->tokenID2>=I_NOT)&&(pToken->tokenID2<=I_FREE))
			{
                ParseNOTSI();
				break;
			};
			//Instrucciones de JE a CALL
            if((pToken->tokenID2>=I_JE)&&(pToken->tokenID2<=I_CALL))
			{
                ParseJumps();
				break;
			};
			//Instrucciones HALT,RET y NOP
            if((pToken->tokenID2>=I_HALT)&&(pToken->tokenID2<=I_NOP))
			{
                ParseHaltRetNop();
				break;
			};

			//Atributos y Instrucciones unicas
            switch(pToken->tokenID2)
			{
			case I_CALLE://Llamadas api
				ParseApiCall();
				break;
			case KEY_HEAPSIZE:
				ParseHeapSize();
				break;
			case KEY_STACKSIZE:
				ParseStackSize();
				break;
			case KEY_PROC:
				ParseProc();
				break;
			case KEY_VARS:
				ParseVars();
				break;
			case KEY_APICALL://Equivalentes APICALL
				ParseApiDecl();
				break;
			case KEY_INCLUDE:
				ParseInclude();
				break;
            case KEY_VAR:
                ParseLocalVar();
                break;
			default:
                WRITE_ERR("Invalid use of keyword ");
			};
			
		}
		else
		{
			//Si no es palabra reservada es una etiqueta
            ParseLabel();
		}
		break;
	case TOKEN_TYPE_ERROR:
        WRITE_ERR("Invalid Token ");
	case TOKEN_TYPE_DOTCOMA://Comentario
		ParseComment();
		break;
	default:
        WRITE_ERR("Invalid Simbol ");
	}


}
		


/***********************Parse MOV************************/
/*******ins->opcode op1,op2
********op1->REG|$NUM|VAR|$REG|$VAR
********op2->REG|NUM|VAR|$REG|$NUM|$VAR
********opcode-> mov*/

void gVmAsm::ParseMOV()
{

	
    PARAM *pParam;//Parametro
    EASMINS *pIns;//Instruccion
    gu16 wState=0;//Estado
    gu16 wAddrMode;//Modo de direccionamiento
	bool bDone=false;
	//Creamos la estructura
	pIns=new EASMINS;
    pIns->wOpcode =pToken->tokenID2;//MOV
    pIns->lParams.setFunction(CleanParam);
	while(!bDone)
	{
		//Obtenemos primer parametro
		//Si no existe el estado de error
		if(wState!=15)
            getToken();
		switch(wState)
		{
			//Tomamos las opciones validas
		case 0:

            switch(pToken->tokenID1 )
            {
			case TOKEN_TYPE_MONEY: //$ direccion de memoria
				wState=1;//Pasamos al siguiente estado donde todo es valido
				wAddrMode=ADDR_INDIRECT;//Modo de direccionamiento indirecto
				break;
			case TOKEN_TYPE_IDENT: //Variable
				wAddrMode=ADDR_DIRECT;//Modo directo
				wState=2;
				break;
			default:
				wState=15;
				break;
			}
			break;
		case 1:
            switch(pToken->tokenID1)
			{
			case TOKEN_TYPE_IDENT:
				wState=2;
				
				break;
			case TOKEN_TYPE_NUMINT:
				//Creamos el parametro
				pParam=new PARAM;
				//Guardamos el valor
                pParam->szName=pToken->tokenString;
				//Tipo de parametro
				pParam->wAddressMode =wAddrMode;
				pParam->wParamType =TOKEN_TYPE_NUMINT;//Numero entero
				wState=3;//Pasamos al proximo
				break;
			default:
				wState=15;
				break;
			};

	
			break;	
        case 3: //Lee la coma y termina instruccion
            if(pIns->lParams.count()==2)
			{
				bDone=true;//Si ya tenemos los 2 parametros
                //stepBack();
                stepBack();
				break;
			};
            switch(pToken->tokenID1)
            {
            case TOKEN_TYPE_COMA:
                wState=4;
                break;

            default:
                wState=15;
                break;
            }
			break;
		case 4://Segundo parametros
            switch(pToken->tokenID1 )
			{
			case TOKEN_TYPE_MONEY: //$ direccion de memoria
				wState=1;//Pasamos al siguiente estado donde todo es valido
				wAddrMode=ADDR_INDIRECT;//Modo de direccionamiento indirecto
				break;
			case TOKEN_TYPE_IDENT: //Variable
				wAddrMode=ADDR_DIRECT;//Modo directo
				wState=2;
				break;
			case TOKEN_TYPE_NUMINT:
				//Creamos el parametro
				pParam=new PARAM;
				//Guardamos el valor
                pParam->szName=pToken->tokenString;
				//Tipo de parametro
				pParam->wAddressMode =ADDR_DIRECT;
				pParam->wParamType =TOKEN_TYPE_NUMINT;//Numero entero
                pIns->lParams.append(pParam);
				wState=3;//Pasamos al proximo
				break;
			default:
				wState=15;
				break;
			};
            break;
		case 15:
			bDone=true;
            WRITE_ERR("Expression error");
		}
		if(wState==2)//Identificador
		{
            if(GetReservedWord())
			{
                if((pToken->tokenID2>=KEY_RACC)&&(pToken->tokenID2<=KEY_RGEN))
				{
					
					pParam=new PARAM;//Creamos el parametro
					//Asignamos el modo de direccionamiento
					pParam->wAddressMode =wAddrMode;
					//Parametro del registro acumulador
                    pParam->wParamType =pToken->tokenID2;
                    pParam->offset=checkOffSet();

					//Lo agregamos
                    pIns->lParams.append(pParam);
					wState=3;
					
				}
				else
				{
					wState=15;//Error
					
				};	
						
					
				}
				else
				{	//Si no es un registro es una variable
					
                    if(IsLabelProc())
					{
						wState=15;
						
					}
                    EVAR *lv=0;
                    if(!IsVariable()){
                        if(procInside){
                            if((lv=procInside->getVariable(pToken->tokenString))==0){
                                wState=15;
                                break;
                            }

                        }else{
                            wState=15;
                            break;
                        }

                    }

					{
                        pParam=new PARAM(procInside);//Creamos el parametro
						//Guardamos el nombre
                        pParam->szName =pToken->tokenString;
						//Guardamos el modo de direccionamiento
						pParam->wAddressMode = wAddrMode;
                        //tricky here now with local vars we must make sure to set this correctly
                        if(lv){
                            pParam->wParamType=KEY_STACKDIRECT;
                            pParam->daddress=lv->spAddress;
                        }else{
                            pParam->wParamType =PARAM_TYPE_VAR;

                        }
                        pParam->offset=checkOffSet();

						//Guardamos el parametro
                        pIns->lParams.append(pParam);
						wState=3;
					}
				}
		}

        /*if(wState!=15)
            FreeToken(&pToken);*/
	}

    ins.append(pIns);//Agregamos la instruccion
	//Aumentamos el PC
	dwPC+=11;//La instruccion mov tiene 1 byte por el opcode
			 //1 byte por el flag y 8 bytes de los parametros


}

/*******************ParseMOVB**************************
ins->movb op,op,size
	op->$REG|$NUM|$IDENT
	size->NUM|ident
	ident->INT
	***************/
void gVmAsm::ParseMOVB()
{
    //LPTOKEN pToken;//Token
    PARAM *pParam;//Parametro
    EASMINS *pIns;//Instruccion
    gu16 wState=0;
	bool bDone=false;
	bool bSize=false;

	pIns=new EASMINS;//Creamos la instruccion

	pIns->wOpcode =I_MOVB;
    pIns->lParams.setFunction(CleanParam);
	
	while(!bDone)
	{
		if(wState!=15)
            getToken();//Obtenemos el token
		switch(wState)
		{
		case 0://Primer estado
			//Esperamos a fuerzas $
            if(pToken->tokenID1!=TOKEN_TYPE_MONEY)
			{
				wState=15;
				break;
			}
			wState=1;//Pasamos al siguiente estado
			break;
		case 1://Esperamos un identificador o un numero
            switch(pToken->tokenID1)
			{
			case TOKEN_TYPE_IDENT://Identificador
				wState=2;//Pasa al estado de identificador
				break;
			case TOKEN_TYPE_NUMINT://Encontro un numero entero
				//Creamos el parametro
				pParam=new PARAM;
				//Copiamos el valor
                pParam->szName = pToken->tokenString;
				if(!bSize)//Se analiza el parametro size
				{
					pParam->wAddressMode =ADDR_INDIRECT;//Modo indirecto
					
				}
				else
				{
					pParam->wAddressMode =ADDR_DIRECT;//Numero directo 
				}
				pParam->wParamType =TOKEN_TYPE_NUMINT;
				//Guardamos el parametro
                pIns->lParams.append(pParam);
				wState=3;
				break;
			default:
				wState=15;//error
				break;
			};
			break;
		case 3://Coma
            if(pIns->lParams.count()==3)
			{
				//Se tienen los parametros
				bDone=true;
                //stepBack();
                stepBack();
				break;
			};
            if(pToken->tokenID1!=TOKEN_TYPE_COMA)
			{
				wState=15;
			};
            if(pIns->lParams.count()==2)
			{
				wState=1;//Parametro size
				bSize=true;//Indicamos que es el ultimo parametro
				break;
			}
			wState=0;//Regresamos para tomar el siguiente parametro
			break;
		case 15:
			delete pIns;//Borramos pIns
            //FreeToken(&pToken);//Liberamos memoria del token
            WRITE_ERR("Sintaxis Error on");
		};

		if(wState==2)//Identificador
		{
            if(GetReservedWord())
			{
                if((pToken->tokenID2>=KEY_RACC)&&(pToken->tokenID2<=KEY_RGEN))
				{
					
					pParam=new PARAM;//Creamos el parametro
					//Asignamos el modo de direccionamiento
					if(!bSize)
					{
						pParam->wAddressMode =ADDR_INDIRECT;
					}
					else
					{
						pParam->wAddressMode=ADDR_DIRECT;
					}
					//Parametro del registro acumulador
                    pParam->wParamType =pToken->tokenID2;
                    pParam->offset=checkOffSet();
					//Lo agregamos
                    pIns->lParams.append(pParam);
					wState=3;
					
				}
				else
				{
					wState=15;//Error
					
				};	
						
					
			}
			else
			{	//Si no es un registro es una variable
				
                if(IsLabelProc())
				{
					wState=15;
                    break;
					
				}
                EVAR *lv=0;
                if(!IsVariable()){
                    if(procInside){
                        if((lv=procInside->getVariable(pToken->tokenString))==0){
                            wState=15;
                            break;
                        }

                    }else{
                        wState=15;
                        break;
                    }

                }

                    pParam=new PARAM(procInside);//Creamos el parametro
					//Guardamos el nombre
                    pParam->szName =pToken->tokenString;
					//Guardamos el modo de direccionamiento
					if(!bSize)
					{
						pParam->wAddressMode =ADDR_INDIRECT;
					}
					else
					{
						pParam->wAddressMode=ADDR_DIRECT;
					}
					//Guardamos el parametro
                    if(lv){
                        pParam->wParamType=KEY_STACKDIRECT;
                        pParam->daddress=lv->spAddress;
                    }else{
                        pParam->wParamType =PARAM_TYPE_VAR;

                    }
                    pParam->offset=checkOffSet();
                    pIns->lParams.append(pParam);
					wState=3;

			}
		}
        /*if(wState!=15)
            FreeToken(&pToken);*/
	}
    ins.append(pIns);
	dwPC+=16;//La instruccion movb tiene 16 bytes
}
			

/**********************ParseABC
ins->opcode op,op
	op->$REG|$IDENT|$NUM|REG|IDENT|NUM
    opcode->ADD|SUB|DIV|MUL|FADD|FSUB|FDIV|FMUL|AND|OR|XOR|CMP|INC|DEC
*/

void gVmAsm::ParseABC()
{
    //LPTOKEN pToken;//Token
    gu16 wAddrMode;//Modo de direccionamiento
    gu16 wState=0;//Estado
    EASMINS *pIns;//Instruccion
    PARAM *pParam;//Parametro
	bool bDone=false;
	bool bIndirectCheck=false;//Indica si se paso por el estado indirecto $


	pIns=new EASMINS;//Creamos la instruccion
    pIns->lParams.setFunction(CleanParam);//Funcion para limpiar la lista
    pIns->wOpcode =pToken->tokenID2;//Se pasa el opcode
	wAddrMode=ADDR_DIRECT;
	while(!bDone)
	{
		if(wState!=15)
            getToken();

		switch(wState)
		{
		case 0://Primer Estado
            switch(pToken->tokenID1)
			{
			case TOKEN_TYPE_MONEY:
				if(!bIndirectCheck)
				{
					wAddrMode=ADDR_INDIRECT;//Modo indirecto
					wState=1;
				}
				else
				{
					wState=15;//Se uso $$
				};
				break;
			case TOKEN_TYPE_IDENT:
				
				wState=2;
				break;
			case TOKEN_TYPE_NUMINT:
				
				wState=3;//Numero entero
				break;
			case TOKEN_TYPE_NUMFLOAT:
				
				wState=3;//Numero entero . flotante
				break;
			default:
				wState=15;
				break;
			};
			break;
		case 1:
			bIndirectCheck=true;//Indica que se checo $
            //stepBack();
            stepBack();
			wState=0;//Se regresa a 0
			break;
		case 4://Coma
			//Verificamos si la lista contiene los parametros
            if(pIns->lParams.count()==2)
			{
				bDone=true;//Listo!! se tienen los parametros
                stepBack();
				break;
			}
			//Esperamos ,
            if(pToken->tokenID1 !=TOKEN_TYPE_COMA)
			{
				wState=15;
				break;
			};
			wAddrMode=ADDR_DIRECT;
			wState=0;//Regresamos al estado 0
            bIndirectCheck=false;
			break;
		case 15://Estado Error
			delete pIns;//Borramos la instruccion
            WRITE_ERR("Sintaxis Error");
		};

		//Verificamos los estados 2 y 3
		switch(wState)
		{
		case 2://Identificador
            if(GetReservedWord())
			{
                if((pToken->tokenID2>=KEY_RACC)&&(pToken->tokenID2<=KEY_RGEN))
				{
				
					pParam=new PARAM;//Creamos el parametro
					//Asignamos el modo de direccionamiento
					pParam->wAddressMode =wAddrMode;
					//Parametro del registro acumulador
                    pParam->wParamType =pToken->tokenID2;
                    pParam->offset=checkOffSet();
					//Lo agregamos
                    pIns->lParams.append(pParam);
					wState=4;
					break;
				}
				else
				{
					wState=15;//Error
					break;
				};
				
				
				
					
						
					
			}
			else
			{	//Si no es un registro es una variable
                EVAR *lv=0;
                if(!IsVariable()){
                    if(procInside){
                        if((lv=procInside->getVariable(pToken->tokenString))==0){
                            wState=15;
                            break;
                        }

                    }else{
                        wState=15;
                        break;
                    }

                }
                if(IsLabelProc())
				{
					wState=15;
					break;
				}
                pParam=new PARAM(procInside);//Creamos el parametro
				//Guardamos el nombre
                pParam->szName =pToken->tokenString;
				//Guardamos el modo de direccionamiento
				pParam->wAddressMode = wAddrMode;
                pParam->offset=checkOffSet();
                if(lv){
                    pParam->wParamType=KEY_STACKDIRECT;
                    pParam->daddress=lv->spAddress;
                }else{
                    pParam->wParamType =PARAM_TYPE_VAR;

                }
                //Guardamos el parametro
                pIns->lParams.append(pParam);
				wState=4;
			}
			break;
		case 3:
			//Creamos el parametro
			pParam=new PARAM;
			//Copiamos el valor
            pParam->szName = pToken->tokenString;
			pParam->wAddressMode =wAddrMode;//Modo indirecto

            pParam->wParamType =pToken->tokenID1;
			//Guardamos el parametro
            pIns->lParams.append(pParam);
			wState=4;
			break;
		}
    /*	if(wState!=15)
            FreeToken(&pToken);*/
	}
    ins.append(pIns);//Anadimos la instruccion
	dwPC+=11;//Las instrucciones tiene 11 bytes
}

/*******************ParseNOTSI***************************
ins->opcode op
	op->REG|IDENT|NUM|$REG|$IDENT|$NUM
	opcode->NOP|POP|PUSH|
***********************************************************/

void gVmAsm::ParseNOTSI()
{
    //LPTOKEN pToken;//Token
    PARAM *pParam;//Parametro
    EASMINS *pIns;//instruccion
    gu16 wState=0;//Estado
    gu16 wAddrMode;//Modo de direccionamiento
	bool bDone=false;//Estado del bucle
	bool bIndirectCheck=false;

	//Creamos la instruccion
	pIns=new EASMINS;
    //Anadimos el opcode
    pIns->wOpcode =pToken->tokenID2;
	//Agregamos la funcion de destruccion de la lista
    pIns->lParams.setFunction(CleanParam);
	//Iniciamos el analisis
	wAddrMode=ADDR_DIRECT;
	while(!bDone)
	{
		if(wState!=15)
            getToken();
		switch(wState)
		{
		case 0://Estado inicial las intrucciones nada mas tienen 1 parametro
            switch(pToken->tokenID1)
			{
			case TOKEN_TYPE_MONEY://Modo indirecto
				if(!bIndirectCheck)
				{
					//No se ha verificado el modo indirecto
					//Pasamos a la verificacion
					wAddrMode=ADDR_INDIRECT;
					wState=1;
				}
				else
				{
					//ya se verifico y no se permite doble $$
					wState=15;
				}
				break;
			case TOKEN_TYPE_IDENT://Identificador
                if(GetReservedWord())
				{
                    if((pToken->tokenID2>=KEY_RACC)&&(pToken->tokenID2<=KEY_RGEN))
					{
						
						pParam=new PARAM;//Creamos el parametro
						//Asignamos el modo de direccionamiento
						pParam->wAddressMode =wAddrMode;
						//Parametro del registro acumulador
                        pParam->wParamType =pToken->tokenID2;
						//Lo agregamos
                        pIns->lParams.append(pParam);
                        pParam->offset=checkOffSet();
						wState=3;
						break;
					}
					else
					{
						wState=15;//Error
						break;
					};
					
					
					
					
					
					
				}
				else
				{	//Si no es un registro es una variable
                    EVAR *lv=0;
                    if(!IsVariable()){
                        if(procInside){
                            if((lv=procInside->getVariable(pToken->tokenString))==0){
                                wState=15;
                                break;
                            }

                        }else{
                            wState=15;
                            break;
                        }

                    }
                    //from a variabla inside function procedure

                    if(IsLabelProc())
					{
						wState=15;
						break;
					}
                    pParam=new PARAM(procInside);//Creamos el parametro
					//Guardamos el nombre
                    pParam->szName =pToken->tokenString;
					//Guardamos el modo de direccionamiento
					pParam->wAddressMode = wAddrMode;
                    if(lv){
                        pParam->wParamType=KEY_STACKDIRECT;
                        pParam->daddress=lv->spAddress;
                    }else{
                        pParam->wParamType =PARAM_TYPE_VAR;

                    }
                    pParam->offset=checkOffSet();
					//Guardamos el parametro
                    pIns->lParams.append(pParam);
					wState=3;
				}
				
				break;
			case TOKEN_TYPE_NUMINT:
				wState=2;
				break;
			case TOKEN_TYPE_NUMFLOAT:
				wState=2;
				break;
			};
			break;
		case 1:
			bIndirectCheck=true;
			wState=0;
            stepBack();
			break;
		case 3:
			bDone=true;
            stepBack();//Regresamos al token anterior
			break;
		case 15://error
			delete pIns;
            WRITE_ERR("Sintaxis Error");
		};
		if(wState==2)//Estado de numero
		{
			//Creamos el parametro
			pParam=new PARAM;
			//Copiamos el valor
            pParam->szName = pToken->tokenString;
			pParam->wAddressMode =wAddrMode;//Modo indirecto
            pParam->wParamType =pToken->tokenID1;

			//Guardamos el parametro
            pIns->lParams.append(pParam);
			wState=3;
		}
        /*if(wState!=15)
            FreeToken(&pToken);*/
	}

	//Agregamos las instruccion
    //make adjustments for stack instructions
    if(pIns->wOpcode==I_PUSH)
        dwSP+=4;
    else if(pIns->wOpcode==I_POP && dwSP>0)
        dwSP-=4;

    ins.append(pIns);
	dwPC+=6;//Las instrucciones nop,push,pop tienen 1 byte del opcode
			 //1 byte del flag y 4 bytes del parametro 6 bytes+1 7
}

/*
Instrucciones de Salto y pila y llamado de funcion
	ins->opcode op
	op->ident|NUM
    opcode->JE|JNE|JL|JG|JMP|CALL
	ident->VAR INT
*/

void gVmAsm::ParseJumps()
{
    //LPTOKEN pToken;
    PARAM *pParam;//Parametro
    EASMINS *pIns;//Instruccion
	


	pIns=new EASMINS;//Crea la instruccion

    pIns->lParams.setFunction(CleanParam);
    pIns->wOpcode =pToken->tokenID2;//Asignamos la instruccion
	
    getToken();//Obtenemos el token;

    switch(pToken->tokenID1)
	{
	case TOKEN_TYPE_IDENT:
        ISNOT_RESERVED();//Se verifica si es una palabra reservada
		
		pParam=new PARAM;//Creamos el parametro
        pParam->szName =pToken->tokenString;//Pasamos la etiqueta no importa si existe o no
		pParam->wParamType = PARAM_TYPE_LABEL;//Es una etiqueta
		pParam->wAddressMode =ADDR_DIRECT;//Modo indirecto
        pIns->lParams.append(pParam);//Anadimos el parametro
		break;
	case TOKEN_TYPE_NUMINT:
		pParam=new PARAM;//Creamos el parametro
        pParam->szName =pToken->tokenString;//Pasamos la etiqueta no importa si existe o no
		pParam->wParamType = PARAM_TYPE_NUMLABEL;//Es una etiqueta
		pParam->wAddressMode =ADDR_DIRECT;//Modo indirecto
        pIns->lParams.append(pParam);//Anadimos el parametro
		break;
	default:
		//Cualquier otra cosa genera un error
        WRITE_ERR("Sintaxis Error");
	}

    //FreeToken(&pToken);

	
    ins.append(pIns);//Anadimos la instruccion
	
	dwPC+=5;//La instruccion vale por 1 byte de le opcode 4 bytes del parametro
}	

/*Instruccion de retorno y fin de programa
	ins->opcode
	opcode->HALT|RET
*/

void gVmAsm::ParseHaltRetNop()
{
    EASMINS *pIns;

	pIns=new EASMINS;//Creamos las instruccion
    //Anadimos el opcode
    pIns->wOpcode =pToken->tokenID2;
    if(pIns->wOpcode==I_RET || pIns->wOpcode==I_HALT){
        //fall back to -1
        procInside=0;//end of function
    }
    //Las instrucciones senaladas no tienen parametros
    ins.append(pIns);//Agregamos la instruccion
	//Estas instrucciones no incrementan el PC pero
	//se ocupa la direccion
	dwPC+=1;//Solamente el byte
}




/*Regla para etiquetas
	label->ident:
*/

void gVmAsm::ParseLabel()
{
    //LPTOKEN pToken;//Token
    LBPROC *pLabel;//Estructura de la etiqueta.

    gString name=pToken->tokenString;
	//Buscamos 2 puntos
    getToken();
    if(pToken->tokenID1 !=TOKEN_TYPE_TWODOTS)
	{
        WRITE_ERR("Expected : After Label");
	};

	pLabel=new LBPROC;//Creamos la etiqueta
	pLabel->dwAddress =dwPC;//Direccion actual
    pLabel->szLabelProcName=name;
	pLabel->wType=PROC_TYPE_LABEL;
    procs.append(pLabel);//Agregamos la etiqueta
    //FreeToken(&pToken);

}

/*
Regla para procedimientos
	proc->PROC ident:
*/
void gVmAsm::ParseProc()
{
    //LPTOKEN pToken;
    LBPROC *pProc;
	//Buscamos el identificador
    MATCH(TOKEN_TYPE_IDENT,"Expected Identifier");

    if(IsLabelProc())

	{
        WRITE_ERR("Label already defined ");
	};
	pProc=new LBPROC;//Se crea el proc
    pProc->szLabelProcName =pToken->tokenString;
    pProc->szLabelProcName.thisToLowCase();//Convertimos a minusculas
	pProc->dwAddress =dwPC;
	//Line added for version 1.1
	if(pProc->szLabelProcName=="main")
	{
		pProc->wType=PROC_TYPE_MAIN;//Is the main procedure
    }
	else
	{
		pProc->wType=PROC_TYPE_PROC;//It's any type of proc
	}
    //Anadimos el procedimiento
    procs.append(pProc);
    //get the index
    procInside=pProc;
    //FreeToken(&pToken);
}
		
 

/******************ParseComment************/

void gVmAsm::ParseComment()
{

    while(currentChar()!='\n')
	{
        //GetNextChar();
        nextChar();
	}
}
		


/***************Parse Vars*******************/
/*Declaracion de Variable
	vardecl->ndecl ident,nval|sdecl ident,sval
	ncdel->INT|FLOAT
	nval->numero
	sdecl->STR
	sval->"Cadena"
	ident->Identificador
*/

void gVmAsm::ParseVars ()
{
    //LPTOKEN pToken;//Token;
    EVAR *pVars;//Var
    gu16 wState=0;//Estado
    gu16 wVarType;//Tipo de variable
	
    gString szVarName;//Nombre
    gString szVal;
	bool bDone=false;


	while(!bDone)
	{
		if(wState!=15)
            getToken();

		switch(wState)
		{
		case 0:// buscamos {
            MATCHTOK(TOKEN_TYPE_KEYOPEN,"Expected {");
			wState=1;
			break;
		case 1:
			//Buscamos tipo de datos
            if(pToken->tokenID1==TOKEN_TYPE_KEYCLOSE)
			{
				bDone=true;
				
				break;
			};
            if(pToken->tokenID1==TOKEN_TYPE_DOTCOMA)
			{
				ParseComment();//Un comentario
				break;
			};
            MATCHTOK(TOKEN_TYPE_IDENT,"Expected Identifier");
            IS_RESERVED();//Vemos si es reservada
            switch(pToken->tokenID2)
			{
			case KEY_INT:
				wVarType=KEY_INT;
				
				wState=2;
				break;
			case KEY_FLOAT:
				wVarType=KEY_FLOAT;
				wState=2;
				break;
			case KEY_STRING:
				wVarType=KEY_STRING;
				wState=2;
				break;
			default:
				//Cualquier otra produce error
                WRITE_ERR("Invalid Use of keyword");
				break;
			}
			break;
		case 2:
			//Buscamos identificador
            MATCHTOK(TOKEN_TYPE_IDENT,"Expected Identifier");
            ISNOT_RESERVED();//Vemos si es reservado y si lo es producer un error
            szVarName=pToken->tokenString;
			wState=3;
			break;
		case 3:
			//Buscamos ,
            MATCHTOK(TOKEN_TYPE_COMA,"Expected ,");
			wState=4;
			break;
		case 4:
			//Buscamos un valor
            switch(pToken->tokenID1)
			{
			case TOKEN_TYPE_NUMINT://Valor numerico
                szVal=pToken->tokenString;
				break;
			case TOKEN_TYPE_NUMFLOAT://.flotante
                szVal=pToken->tokenString;
				break;
			case TOKEN_TYPE_COMILLADBL://Cadena
                while(currentChar()!='"')
				{
                    szVal+=nextChar();
				}
                MATCH(TOKEN_TYPE_COMILLADBL,"Expected \"");
			break;
			wState=1;
			default:
                WRITE_ERR("Sintaxis Error");
			}
			//Agregamos la variable
			pVars=new EVAR;
			pVars->dwVarType =wVarType;
			pVars->szVal =szVal;
			pVars->szVarName =szVarName;

            vars.append(pVars);//Agregamos la variable
			wState=1;//Regresamos al primero
            szVal.clear();
            /*if(wState!=15)
                FreeToken(&pToken);*/

		}
	}
}



	
/**************************ParseHeapSize*******************/

void gVmAsm::ParseHeapSize()
{
    //LPTOKEN pToken;

	//Esperamos un numero entero
    MATCH(TOKEN_TYPE_NUMINT,"Expected Integer");

    dwHeapSize=pToken->tokenString.toUInt();
    //FreeToken(&pToken);

};

/**********************ParseStackSize*******************/
void gVmAsm::ParseStackSize()
{
    //LPTOKEN pToken;

	//Esperamos un numero entero
    MATCH(TOKEN_TYPE_NUMINT,"Expected Integer");

    dwStackSize=pToken->tokenString.toUInt();
    //FreeToken(&pToken);

};



/******************Verify Labels and Procs****************/

bool gVmAsm::VerifyLabelsAndProcs()
{
    EASMINS *pIns;//Instrucciones
    PARAM *pParam;//Parametro
	bool bRetVal=true;
    gString *szmsg;
    ins.first();
    while((pIns=(LPEASMINS)ins.data())!=NULL)
	{
		//Verifica las instrucciones

		if((pIns->wOpcode>=I_JE)&&(pIns->wOpcode<=I_CALL))
		{
			//El rango de instrucciones
			//Suponemos bien que lleva un solo parametro
            pParam=(LPPARAM)pIns->lParams.data(0);
			//Convertimos a minusculas
            pParam->szName.thisToLowCase();
			if(!IsLabelProc(pParam->szName))
			{
                szmsg= new gString("Label or proc unidentified ");
                (*szmsg)+=pParam->szName;
                errlist.append(szmsg);
				bRetVal= false;
			}
		}
        ins.next();
	}

	return bRetVal;
}
//***********************VerifyApiDecls****************************/
bool gVmAsm::VerifyApiDecls()
{
    EASMINS *pIns;//Instrucciones
    PARAM *pParam;//Parametro
	bool bRetVal=true;
    gString *szmsg;
    ins.first();
    while((pIns=(LPEASMINS)ins.data())!=NULL)
	{
		//Verifica las instrucciones

		if(pIns->wOpcode==I_CALLE)
		{
			//El rango de instrucciones
			//Suponemos bien que lleva un solo parametro
            pParam=(LPPARAM)pIns->lParams.data(0);
				//Convertimos a minusculas
            pParam->szName.thisToLowCase();
			if(pParam->wParamType==PARAM_TYPE_APICALL)
			{
				//Solamente si es un identificador
				if(!IsApiProc(pParam->szName))
				{
                    szmsg=new gString("Api Call UnIdentified ");
                    (*szmsg)+=pParam->szName;
                    errlist.append(szmsg);
					bRetVal= false;
				}
			}
		}
                ins.next();
	}

	return bRetVal;
}
/************************ParseApiDecl******************************/
//Declaraciones API
//decl->api_call num
void gVmAsm::ParseApiDecl()
{
    //LPTOKEN pToken;//Token
	
    LBPROC *pApiDecl;
    gString szName;
	//Buscamos el identificador
    MATCH(TOKEN_TYPE_IDENT,"Expected Identifier");
	
    ISNOT_RESERVED();//Verificamos que no sea palabra reservada
    szName=pToken->tokenString;//Copiamos el nombre
    //FreeToken(&pToken);
    MATCH(TOKEN_TYPE_NUMINT,"Expected Address Value");
	pApiDecl=new LBPROC;//Creamos la estructura
    pApiDecl->dwAddress=pToken->tokenString.toUInt();//Copiamos la direccion
	pApiDecl->szLabelProcName=szName;//Pasamos el nombre
	//Agregamos a la lista

    apidecl.append(pApiDecl);
    //FreeToken(&pToken);
	

	
}

/*****************************ParseApiCall**************************/
void gVmAsm::ParseApiCall()
{ 
    //LPTOKEN pToken;
    PARAM *pParam;//Parametro
    EASMINS *pIns;//Instruccion
	


	pIns=new EASMINS;//Crea la instruccion

    pIns->lParams.setFunction(CleanParam);
	pIns->wOpcode =I_CALLE;//Asignamos la instruccion
	
    getToken();//Obtenemos el token;

    switch(pToken->tokenID1)
	{
	case TOKEN_TYPE_IDENT:
        ISNOT_RESERVED();//Se verifica si es una palabra reservada
		
		pParam=new PARAM;//Creamos el parametro
	
        pParam->szName =pToken->tokenString;//Pasamos la etiqueta no importa si existe o no
		pParam->wParamType = PARAM_TYPE_APICALL;//Es una etiqueta
		pParam->wAddressMode =ADDR_DIRECT;//Modo indirecto
        pIns->lParams.append(pParam);//Anadimos el parametro
		break;
	case TOKEN_TYPE_NUMINT:
		pParam=new PARAM;//Creamos el parametro
        pParam->szName =pToken->tokenString;//Pasamos la etiqueta no importa si existe o no
		pParam->wParamType = PARAM_TYPE_NUMLABEL;//Es una etiqueta
		pParam->wAddressMode =ADDR_DIRECT;//Modo indirecto
        pIns->lParams.append(pParam);//Anadimos el parametro
		break;
	default:
		//Cualquier otra cosa genera un error
        WRITE_ERR("Sintaxis Error");
	}

    //FreeToken(&pToken);

	
    ins.append(pIns);//Anadimos la instruccion
	
	dwPC+=5;//La instruccion vale por 1 byte de le opcode 4 bytes del parametro
}


/**************************ParseInclude***************************/
void gVmAsm::ParseInclude()
{
    //LPTOKEN pToken;
	char szFile[MAX_PATH];//Esperamos el nombre de un archivo
    gString szTempData;
    gu32 dwPosition;
    gu32 i=0;//contador

    //Esta funcion solo recibe un codigo extra de un archivo y lo anade a la compilacion
	//Se compila el archivo que se recibe
	//Esperamos una comilla

	


    MATCH(TOKEN_TYPE_COMILLADBL,"Expected \"");
	memset(szFile,0,MAX_PATH);
    //FreeToken(&pToken);
    while(currentChar()!='"')
	{
		if(i==MAX_PATH)
		{
            WRITE_ERR("File Name to long");
		}
        szFile[i]=nextChar();
		i++;
	}
    MATCH(TOKEN_TYPE_COMILLADBL,"Expected \"");//we close the "
    //FreeToken(&pToken);
	//we have the file name we must compile it
    szTempData=script();//Openned file

    dwPosition=position();
    clear();
    OpenVmAsmFile(szFile);
	ParseCode();//Compilamos
	//we restore file data
    clear();
    setScript(szTempData);
    setPosition(dwPosition);
	
}

//check offset
gvmasm_offset gVmAsm::checkOffSet()
{
    gvmasm_offset offset;


    getToken();

    if(pToken->tokenID1==TOKEN_TYPE_PLUS)
    {
        //now get the next token we expect either a num int or a variable
        getToken();
        if(pToken->tokenID1==TOKEN_TYPE_NUMINT)
        {
            offset.value=pToken->tokenString;
            offset.offsettype=gvmasm_offset::OFFSET_NUMERIC;
            offset.value.setShared(true);
            return gvmasm_offset(offset);
        }
        else if(pToken->tokenID1==TOKEN_TYPE_IDENT)
        {
            if(GetReservedWord())
            {
                WriteError("Invalid usage of reserved word");
                return gvmasm_offset();
            }
            if(IsVariable())
            {
                offset.value=pToken->tokenString;
                offset.offsettype=gvmasm_offset::OFFSET_VARIABLE;
                offset.value.setShared(true);
                return gvmasm_offset(offset);
            }
            else
            {
                WriteError("Undeclated variable ");
                return gvmasm_offset();
            }
        }
        else
        {
            WriteError("Syntaxis error expected, variable or integer as offset ");
            return gvmasm_offset();
        }


    }
    else
    {
        stepBack();

        return gvmasm_offset();
    }

}

//Added new parse local variable

void gVmAsm::ParseLocalVar(){
    int state=0;//use state machine for this
    EVAR *v;

    if(!procInside){
        WriteError("Invalid call of variable declaration outside a proc");
        return;
    }
    v=new EVAR;

    const gToken &t=token();
    while(state!=15 && state!=10){
        //first state var type

            getToken();
        switch(state){
        case 0:
            switch(t.tokenID1){
            case TOKEN_TYPE_IDENT:
                //get the reserved word
                if(GetReservedWord()){
                    //now make sure we have the correct keyword
                    switch(t.tokenID2){
                    case KEY_INT://type integer
                    case KEY_FLOAT: //type float
                    case KEY_STRING://string
                        v->dwVarType=t.tokenID2;//store the var type
                        //go to next state
                        state=1;
                        break;
                    default:
                        //Anything else is wrong
                        WriteError("Error Invalid use of keyword, expected var type");
                        state=15;
                    }
                }else{
                    //Not a defined type error
                    WriteError("Expected var type");
                    state=15;
                }
                break;
            default:
                WriteError("Invalid token,expected var type");
                state=15;
            }
            break;
        case 1://next state
            //expected identifier only
            if(t.tokenID1==TOKEN_TYPE_IDENT){
                //must not be a reserved word
                if(GetReservedWord()){
                    WriteError("Invalid use of keyword, expected variable identifier");
                    state=15;
                }else{
                    //check if variable has been declared on global scope
                    if(IsVariable(t.tokenString)){
                        WriteError("Variable has been declared on global scope");
                        state=15;
                    }else{
                        //now check if variable has not been defined on local scope
                        if(procInside->getVariable(t.tokenString)){
                            WriteError("Variabla already defined on local scope");
                            state=15;
                        }
                        else{
                            //we are fine
                            v->szVarName=t.tokenString;
                            state=2;
                        }
                    }
                }
            }
            break;
        case 2:
            //two posibilities a direct value or a bracked with a size
            if(t.tokenID1==TOKEN_TYPE_COROPEN){

                gToken ret;
                if(parseBracketValue(ret)){
                    //anything alright
                    if(ret.tokenID1==TOKEN_TYPE_NUMINT){
                        v->size=ret.tokenString.toUInt();
                        //state remains the same expecting a coma
                    }else{
                        WriteError("Array size expected to be unsigned integer");
                        state=15;
                    }
                }else { state = 15; }

            }else{
                if(t.tokenID1==TOKEN_TYPE_COMA){
                    state=3;
                }else {
                    state=15;
                    WriteError("Expected ,variable initialization");
                }
            }
                break;
        case 3:{
             //first value of variable
            if(t.tokenID1!=TOKEN_TYPE_PAROPEN){
                gString out;
                stepBack();
                if(v->dwVarType==KEY_INT){
                    if(parseInt(out)){
                        v->szVal=out;
                        state=10;
                    }else{
                        state=15;
                    }
                }else if(v->dwVarType==KEY_FLOAT){
                    if(parseFloat(out)){
                        v->szVal=out;
                        state=10;
                    }else{
                        state=15;
                    }
                }
                else if(v->dwVarType==KEY_STRING){
                    if(parseString(out)){
                        v->szVal=out;
                        state=10;
                    }else{
                        state=15;
                    }
                }
            }
            else{
                gString o;
                if(parseArray(o,v->dwVarType)){
                    v->szVal=o;
                }else{
                    state=15;
                }
            }
        }
            break;
        }
    }
    if(state==15){
        delete v;
        return;
    }
    gu32 incval=0;
    v->spAddress=dwSP;
    procInside->local.append(v);
    EASMINS *lins;
    gStringList s;

    switch(v->dwVarType){
    //Set the size an the increase for the next stack pointer value
    case KEY_INT:
    case KEY_FLOAT:
        if(v->size==0){
            //Add the instruction mov to set the stack value
            incval=4;
            if(!v->szVal.isEmpty()){
                lins=addInstruction(I_MOV);
                //first param
                lins->addParam(KEY_STACKDIRECT,ADDR_DIRECT,gString(),procInside,v->spAddress);
                //next param
                if(v->dwVarType==KEY_INT)
                    lins->addParam(TOKEN_TYPE_NUMINT,ADDR_DIRECT,v->szVal,procInside,0);
                else
                    lins->addParam(TOKEN_TYPE_NUMFLOAT,ADDR_DIRECT,v->szVal,procInside,0);
                dwPC+=11;//increase the PC for this instruction
            }
        }
        else{
            incval=v->size * 4;
            s=v->szVal.split(",");
            for(int i=0;i<s.size();i++){
                lins=addInstruction(I_MOV);
                lins->addParam(KEY_STACKDIRECT,ADDR_DIRECT,gString(),0,v->spAddress+(i*4));
                if(v->dwVarType==KEY_INT)
                    lins->addParam(TOKEN_TYPE_NUMINT,ADDR_DIRECT,s[i],procInside,0);
                else
                    lins->addParam(TOKEN_TYPE_NUMFLOAT,ADDR_DIRECT,s[i],procInside,0);
                dwPC+=11;//increase the PC for this instruction

            }
        }
        break;
    case KEY_STRING:
        if(v->size==0){
            incval=v->szVal.size()+1;//plus 1 for \0
            //since there is no instruction to copy strings directly
            //for now we must copy byte by byte with instruction mov
            char *str=v->szVal.ascii();
            for(int i=0;i<v->szVal.size()+1;i++){
                lins=addInstruction(I_MOV8);
                lins->addParam(KEY_STACKDIRECT,ADDR_DIRECT,gString(),0,v->spAddress + i);
                if(i<v->szVal.size())
                    lins->addParam(TOKEN_TYPE_NUMINT,ADDR_DIRECT,gString::number((int)str[i]),0,0);
                else
                    lins->addParam(TOKEN_TYPE_NUMINT,ADDR_DIRECT,gString::number(0),0,0);
                dwPC+=11;

            }
        }
        else{
            //far more tricky

            s=v->szVal.split(",");
            for(int i=0,k=0;i<s.size();i++){
                incval+=s[i].size() + 1;
                char *str=s[i].ascii();
                for(int j=0;j<s[i].size();i++,k++){
                    lins=addInstruction(I_MOV8);
                    lins->addParam(KEY_STACKDIRECT,ADDR_DIRECT,gString(),0,v->spAddress + k);
                    if(i<s[i].size())
                        lins->addParam(TOKEN_TYPE_NUMINT,ADDR_DIRECT,gString::number((int)str[j]),0,0);
                    else
                        lins->addParam(TOKEN_TYPE_NUMINT,ADDR_DIRECT,gString::number(0),0,0);
                    dwPC+=11;

                }
            }
        }
        break;
    }
    //so simple and easy we create an instruction inc
    lins=new EASMINS;
    lins->wOpcode=I_INC;
    lins->lParams.setFunction(CleanParam);

    PARAM *p=new PARAM;
    p->wAddressMode=ADDR_DIRECT;
    p->wParamType=KEY_STACKPOINTER;

    lins->lParams.append(p);

    p=new PARAM;

    p->wAddressMode=ADDR_DIRECT;
    p->wParamType=TOKEN_TYPE_NUMINT;
    p->szName=gString::number(incval);

    lins->lParams.append(p);
    dwSP+=incval;
    ins.append(lins);
    dwPC+=11;


    //need to set the new value to the stack pointer



}


//utility funcs

bool gVmAsm::parseString(gString &out){
    //expected an string
    const gToken &t=getToken();
    int size=0;
    char *spos;
    char c;
    spos=scriptAtPos(position());
    c=currentChar();
    if(t.tokenID1==TOKEN_TYPE_COMILLADBL){
        while(c!='"' && c > 0){
            nextChar();
            c=currentChar();
            size++;
        }
        nextChar();
        out.setString(spos,size);
        return true;
    }else{
        WriteError("Error Parsing String Expected Quote");
        return false;
    }
}

//
bool gVmAsm::parseInt(gString &out){

    const gToken &t=getToken();
    if(t.tokenID1==TOKEN_TYPE_GUIONNORMAL){
        out+="-";
        getToken();
    }
    if(t.tokenID1==TOKEN_TYPE_NUMINT){
        out+=t.tokenString;
        return true;
    }else if(t.tokenID1==TOKEN_TYPE_NUMFLOAT){
        WriteError("Error parsing integer value, got float value");
        return false;
    }
    else{
        WriteError("Expected integer value");
        return false;
    }
}

bool gVmAsm::parseFloat(gString &out){
    const gToken &t=getToken();
    if(t.tokenID1==TOKEN_TYPE_GUIONNORMAL){
        out="-";
        getToken();
    }
    if(t.tokenID1==TOKEN_TYPE_NUMINT || t.tokenID1==TOKEN_TYPE_NUMFLOAT){
        out+=t.tokenString;
        return true;
    }else {
        WriteError("Error parsing float value");
        return false;
    }

}


bool gVmAsm::parseArray(gString &out,gu32 type){
    int state=0;
    gString acc;
    while(state!=15 && state!=10){
        const gToken &t=getToken();
        switch(state){
        case 0:
            stepBack();
            switch(type){
            case KEY_INT:
                acc.clear();
                if(parseInt(acc)){
                    out+=acc;
                    state=1;
                }else{
                    WriteError("Error Parsing Array");
                    state=15;
                }
                break;
            case KEY_STRING:
                acc.clear();
                if(parseString(acc)){
                    out+=acc;
                    state=1;
                }else{
                    WriteError("Error Parsing Array");
                    state=15;
                }
                break;
            case KEY_FLOAT:
                acc.clear();
                if(parseFloat(acc)){
                    out+=acc;
                    state=1;
                }else{
                    WriteError("Error Parsing Array");
                    state=15;
                }
                break;
            }
            break;
        case 1:
            if(t.tokenID1==TOKEN_TYPE_COMA){
                state=1;
                out+=",";
            }
            else if(t.tokenID1==TOKEN_TYPE_PARCLOSE)
                state=10;
            else{
                state=15;
                WriteError("Error Expecting , or )");
            }
        }
    }
    return state==10;
}


//Bracker value
bool gVmAsm::parseBracketValue(gToken &out){
    const gToken &t=getToken();
    if(t.tokenID1==TOKEN_TYPE_NUMINT){
        out=t;

    }
    getToken();
    if(t.tokenID1==TOKEN_TYPE_CORCLOSE){
        return true;
    }else{
        WriteError("Expected ] close array declaration");
        return false;
    }

}


//Adding extra utility
EASMINS::EASMINS(){
    lParams.setFunction(CleanParam);
}

PARAM *EASMINS::addParam(gu16 wParamType, gu16 addressMode,
                         const gString &_name, LBPROC *p, gu32 _daddress){
    PARAM *pr=new PARAM(p);
    pr->wParamType=wParamType;
    pr->wAddressMode=addressMode;
    pr->szName=_name;
    pr->daddress=_daddress;

    lParams.append(pr);
    return pr;
}

EASMINS *gVmAsm::addInstruction(gu16 _ins){
    EASMINS *e=new EASMINS();

    e->wOpcode=_ins;

    ins.append(e);
    return e;
}

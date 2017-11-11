#include "impl/gVmPrivate.h"
using namespace gfw;
#define ca gVmPrivate *o=(gVmPrivate *)d
#define co gVmPrivate *io=(gVmPrivate *)other.h()
gVm::gVm():gBaseShared(new gVmPrivateFactory){
}

gVm::gVm(const gVm &other):gBaseShared(other,new gVmPrivateFactory){

}
gVm::~gVm(){

}


//The main tamale
bool gVm::create(gu32 ramSize){
    gVmPrivate *o=new gVmPrivate;
    if(o->ram.create(ramSize)){
        d=o;
    }else{
        delete o;
    }
    return d!=0;
}

int gVm::exec()
{
    if(!d)return -1;
    ca;

    gu32 *puData[4];//Recopilacion de apuntadores
    gs32 *pnData[4];
    gf32 *pfData[3];//Apuntadores a flotante
    gu8 *pbData[4];
    gs8 *pcData[3];
    gu16 *pwData[4];
    gs16 *psData[3];


    o->index=0;
    while(o->brun)
    {
        if(o->programs.isEmpty())
        {
           return 0;
        }
        if(o->index>=o->programs.size())
            o->index=0;//reset the counting

        o->currentProg=o->programs.pValue(o->index);
        if(o->currentProg->pc >=o->currentProg->programMemory.size())//reached end of program
        {
            o->currentProg->isActive=false;
        }
        if(o->currentProg->isActive)
        {
            o->restore32L();//copy all values to local
            switch(o->progMemory[o->pc])
            {
            case I_MOV://Instruccion mov
                //Obtenemos el primer parametro
                puData[0]=o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segunndo parametros
                puData[1]=o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));
                //Hacemos la asginacion
                *puData[0]=*puData[1];
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 3+8=11
                break;
            case I_MOV8://Instruccion de mover bloques de 1 gu8
                //Obtenemos el primer parametro
                pbData[0]=(gu8 *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segunndo parametros
                pbData[1]=(gu8 *)o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));
                //Hacemos la asginacion
                *pbData[0]=*pbData[1];
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 3+8=11
                break;
            case I_MOV16://Instruccion de mover bloques de 16 bits
                //Obtenemos el primer parametro
                pwData[0]=(gu16 *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segunndo parametros
                pwData[1]=(gu16 *)o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));
                //Hacemos la asginacion
                *pwData[0]=*pwData[1];
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 3+8=11
                break;
            case I_MOVB://Bloque Move
                //Cada parametro implicitamente representa un bloque de memoria
                //a excepcion que el parametro registro solamente
                //Obtenemos el primer parametro
                puData[0]=o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segunndo parametros
                puData[1]=o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));
                //Obtenemos el 3er parametro
                puData[2]=o->address32(o->progMemory[o->pc+11],*uintp(o->progMemory,o->pc+12));
                //Hacemos la copia
                memcpy(puData[0],puData[1],*puData[2]);
                //Aumentamos el PC
                o->pc+=16;//La instruccion tiene un valor de 4+8+4=16
                break;

                //Aritmeticas
            case I_ADD: //RACC=NUM+NUM add Addr,addr
                //Obtenemos primer operando
                pnData[0]=(int *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                    //Obtenemos el segunndo parametros
                pnData[1]=(int *)o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));

                o->r[0].rs32[0]=*pnData[0]+*pnData[1];//Parte baja
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;

            case I_MUL://Multiplicacion RACC=num1 * num 2
                //Obtenemos primer operando
                pnData[0]=(int *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                    //Obtenemos el segunndo parametros
                pnData[1]=(int *)o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));

                o->r[0].rs32[0]=(*pnData[0]) * (*pnData[1]);//Parte baja
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;

            case I_SUB: //Resta
                //Obtenemos primer operando
                pnData[0]=(int *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                    //Obtenemos el segunndo parametros
                pnData[1]=(int *)o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));

                o->r[0].rs32[0]=(int)(*pnData[0]-*pnData[1]);//Parte baja
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;

            case I_DIV://Division
                //Obtenemos primer operando
                pnData[0]=(int *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                    //Obtenemos el segunndo parametros
                pnData[1]=(int *)o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));
                if(*pnData[1]==0)return 0;//Termina el programa
                o->r[0].rs32[0]=*pnData[0]/(*pnData[1]);//Parte baja
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;
            case I_INC: //Increment
                //Obtenemos destino
                pnData[0]=(int *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                    //Obtenemos el segunndo parametros
                pnData[1]=(int *)o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));

                *pnData[0]+=(*pnData[1]);//Parte baja
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;
            case I_DEC:
                //Obtenemos destino
                pnData[0]=(int *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                    //Obtenemos el segunndo parametros
                pnData[1]=(int *)o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));

                *pnData[0]-=(*pnData[1]);//Parte baja
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;

            case I_NEG://Hace negativo en integer
                //Obtenemos el dato
                pnData[0]=(int *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                (*pnData[0])*=-1;
                o->pc+=6;//La instruccion I_NEG Tiene un valor de 6
                break;

                //Versiones para Punto flotante
                    //Aritmeticas
            case I_FADD: //RACC=NUM+NUM add Addr,addr
                //Obtenemos primer operando
                pfData[0]=(float *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                    //Obtenemos el segunndo parametros
                pfData[1]=(float *)o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));

                o->r[0].rf32[0]=*pfData[0]+*pfData[1];//Parte baja
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;


            case I_FMUL://Multiplicacion RACC=num1 * num 2
                //Obtenemos primer operando
                pfData[0]=(float *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                    //Obtenemos el segunndo parametros
                pfData[1]=(float *)o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));

                o->r[0].rf32[0]=(*pfData[0]) * (*pfData[1]);//Parte baja
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;

            case I_FSUB: //Resta
                //Obtenemos primer operando
                pfData[0]=(float *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                    //Obtenemos el segunndo parametros
                pfData[1]=(float *)o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));

                o->r[0].rf32[0]=*pfData[0]-*pfData[1];//Parte baja
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;

            case I_FDIV://Division
                //Obtenemos primer operando
                pfData[0]=(float *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                    //Obtenemos el segunndo parametros
                pfData[1]=(float *)o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));
                if(*pfData[1]==0)return 0;//Termina el programa
                o->r[0].rf32[0]=*pfData[0]/(*pfData[1]);//Parte baja
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;

            case I_FNEG://Hace negativo en punto flotante
                //Obtenemos el dato
                pfData[0]=(float *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                (*pfData[0])*=-1;
                o->pc+=6;//La instruccion I_NEG Tiene un valor de 6
                break;

                //Operaciones logicas

            case I_AND: //RACC= NUM1 & NUM2
                    //Obtenemos primer operando
                puData[0]=o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                    //Obtenemos el segunndo parametros
                puData[1]=o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));

                o->r[0].ru32[0]=*puData[0] & (*puData[1]);//Parte baja
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;
            case I_AND8: //RACC NUM & NUM2
                //Obtenemos primer operando
                 pbData[0]=(gu8 *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                //Obtenemos el segunndo parametros
                 pbData[1]=(gu8 *)o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));

                o->r[0].ru32[0]=*pbData[0] & (*pbData[1]);//Parte baja
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;
            case I_AND16:
                //Obtenemos primer operando
                 pwData[0]=(gu16 *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                //Obtenemos el segunndo parametros
                 pwData[1]=(gu16 *)o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));

                o->r[0].ru32[0]=*pwData[0] & (*pwData[1]);//Parte baja
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;

            case I_OR://RACC= NUM1 | NUM 2
                //Obtenemos primer operando
                puData[0]=o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                    //Obtenemos el segunndo parametros
                puData[1]=o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));

                o->r[0].ru32[0]=*puData[0] | *puData[1];//Parte baja
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;
            case I_OR8: //RACC=NUM1 | NUM2
                //Obtenemos primer operando
                pbData[0]=(gu8 *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                    //Obtenemos el segunndo parametros
                pbData[1]=(gu8 *)o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));

                o->r[0].ru32[0]=*pbData[0] | *pbData[1];//Parte baja
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;
            case I_OR16:
                //Obtenemos primer operando
                pwData[0]=(gu16 *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                    //Obtenemos el segunndo parametros
                pwData[1]=(gu16 *)o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));

                o->r[0].ru32[0]=*pwData[0] | *pwData[1];//Parte baja
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;

            case I_XOR://RACC= NUM1 ^ NUM2
                //Obtenemos primer operando
                puData[0]=o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                    //Obtenemos el segunndo parametros
                puData[1]=o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));

                o->r[0].ru32[0]=*puData[0] ^ *puData[1];//Parte baja
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;
            case I_XOR8: //8 bit version
                //Obtenemos primer operando
                pbData[0]=(gu8 *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                    //Obtenemos el segunndo parametros
                pbData[1]=(gu8 *)o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));

                o->r[0].ru32[0]=*pbData[0] ^ *pbData[1];//Parte baja
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;
            case I_XOR16: //16 bit version
                //Obtenemos primer operando
                pwData[0]=(gu16 *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                    //Obtenemos el segunndo parametros
                pwData[1]=(gu16 *)o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));

                o->r[0].ru32[0]=*pwData[0] ^ *pwData[1];//Parte baja
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;

            case I_NOT://RACC= NUM1 ~ NUM2
                //Obtenemos primer operando
                //Esta operacion nada mas lleva un operando
                puData[0]=o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));

                o->r[0].ru32[0]=~(*puData[0]);//Parte baja
                //Aumentamos el PC
                o->pc+=6;//La instruccion tiene un valor de 6
                break;
            case I_NOT8: //8 bit
                //Obtenemos primer operando
                //Esta operacion nada mas lleva un operando
                pbData[0]=(gu8 *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));

                o->r[0].ru32[0]=~(*pbData[0]);//Parte baja
                //Aumentamos el PC
                o->pc+=6;//La instruccion tiene un valor de 6
                break;
            case I_NOT16:
                //Obtenemos primer operando
                //Esta operacion nada mas lleva un operando
                pwData[0]=(gu16 *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));

                o->r[0].ru32[0]=~(*pwData[0]);//Parte baja
                //Aumentamos el PC
                o->pc+=6;//La instruccion tiene un valor de 6
                break;
            //Comparacion y salto

            case I_CMP: //Compara 2 numeros y guarda el resultado en RC 32 bits
                    //Obtenemos primer operando
                pnData[0]=(int *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                    //Obtenemos el segunndo parametros
                pnData[1]=(int *)o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));

                o->r[1].rs32[0]=*pnData[0] - *pnData[1];//Parte baja
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;
            case I_CMP8: //Compara dos valores a nivel gu8
                pcData[0]=(gs8 *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                    //Obtenemos el segunndo parametros
                pcData[1]=(gs8 *)o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));

                o->r[1].rs32[0]=(*pcData[0] - *pcData[1]);//Parte baja
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;
            case I_CMP16:
                psData[0]=(short *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                    //Obtenemos el segunndo parametros
                psData[1]=(short *)o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));

                o->r[1].rs32[0]=(*psData[0] - *psData[1]);//Parte baja
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;
                //Added for V1.1
            case I_FCMP:
                        //Obtenemos primer operando
                pfData[0]=(float *)o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el segundo operando
                    //Obtenemos el segunndo parametros
                pfData[1]=(float *)o->address32(o->progMemory[o->pc+6],*uintp(o->progMemory,o->pc+7));

                o->r[1].rf32[0]=*pfData[0] - *pfData[1];//Parte baja

                if(o->r[1].rf32[0]>0) //if the result of the operation is greater than 0 then
                {

                    o->r[1].rs32[0]=1;//we set the compare register to 1
                }

                if(o->r[1].rf32[0]<0)//If the result of the operation is less than 0 then
                {

                    o->r[1].rs32[0]=-1;//we set the compare register to -1
                }

                //This way we avoid the problem to use IEEE floating point format
                //Aumentamos el PC
                o->pc+=11;//La instruccion tiene un valor de 11
                break;


            case I_JE://Brinca is RC=0
                //Obtenemos la direccion
                puData[0]=uintp(o->progMemory,o->pc+1);
                if(o->r[1].rs32[0]==0)
                {
                    o->pc=*puData[0];
                }
                else
                {
                    //La instruccion vale 1+4=5
                    o->pc+=5;
                }
                break;

            case I_JNE://Brinca si RC!=0
                //Obtenemos la direccion
                puData[0]=uintp(o->progMemory,o->pc+1);
                if(o->r[1].rs32[0]!=0)
                {
                    o->pc=*puData[0];
                }
                else
                {
                    //La instruccion vale 1+4=5
                    o->pc+=5;
                }
                break;

            case I_JL://Brinca si RC<0
                //Obtenemos la direccion
                puData[0]=uintp(o->progMemory,o->pc+1);
                if(o->r[1].rs32[0]<0)
                {
                    o->pc=*puData[0];
                }
                else
                {//La instruccion vale 1+4=5
                    o->pc+=5;
                }
                break;

            case I_JG://Brinca si RC>0
                //Obtenemos la direccion
                puData[0]=uintp(o->progMemory,o->pc+1);
                if(o->r[1].rs32[0]>0)
                {
                    o->pc=*puData[0];
                }
                else
                {
                    //La instruccion vale 1+4=5
                    o->pc+=5;
                }
                break;
            case I_JLE:
                //Obtenemos la direccion
                puData[0]=uintp(o->progMemory,o->pc+1);
                if(o->r[1].rs32[0]<=0)
                {
                    o->pc=*puData[0];
                }
                else
                {
                    //La instruccion vale 1+4=5
                    o->pc+=5;
                }
                break;
            case I_JGE:
                //Obtenemos la direccion
                puData[0]=uintp(o->progMemory,o->pc+1);
                if(o->r[1].rs32[0]>=0)
                {
                    o->pc=*puData[0];
                }
                else
                {
                    //La instruccion vale 1+4=5
                    o->pc+=5;
                }
                break;

            case I_JMP: //Brinca incondicionalmente
                //Obtenemos la direccion
                puData[0]=uintp(o->progMemory,o->pc+1);

                o->pc=*puData[0];


                break;

                //Instrucciones de pila

            case I_PUSH:
                //Obtenemos el dato
                puData[0]=o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el apuntador de la pila tamano dgu16
                puData[1]=uintp(o->stack,o->sp);
                *puData[1]=*puData[0];
                o->sp+=4;//Tamano dwgu16
                o->pc+=6;

                break;

            case I_POP:
                //Jalamos el dato
                o->sp-=4;
                //Obtenemos el apuntador de donde vamos a guardar
                puData[0]=o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Obtenemos el apuntador de la pila tamano dgu16
                puData[1]=uintp(o->stack,o->sp);
                *puData[0]=*puData[1];
                o->pc+=6;
                break;
                //Llamadas a procedimientos
            case I_ALLOC: //Memory allocation on data mem
                puData[0]=o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Registro RP contiene direccion virtual
                o->r[3].ru32[0]=o->currentProg->memAlloc(*puData[0]);
                o->pc+=6;
                break;
            case I_FREE: //Frees memory
                puData[0]=o->address32(o->progMemory[o->pc+1],*uintp(o->progMemory,o->pc+2));
                //Registro RP contiene direccion virtual
                puData[1]=uintp(o->dataMemory,*puData[0]);
                o->currentProg->memFree(puData[1]);
                o->pc+=6;
                break;
            case I_CALL:
                //La pila de funciones es diferente
                o->ssp=o->sp;
                o->fsp-=4;//Se resta para obtener la direccion
                //Guardamos el pc en la pila
                puData[0]=uintp(o->stack,o->fsp);
                *puData[0]=o->pc+5;

                puData[1]=uintp(o->progMemory,o->pc+1);//Llamamos la direccion de la funcion
                //Nos posicionamos en la funcion
                o->pc=*puData[1];
                break;

            case I_RET://Regresa el pc de la pila
                //Jalamos el dato
                puData[0]=uintp(o->stack,o->fsp);
                o->fsp+=4;//Y cambiamos el apuntador hacia adelante
                //new change the sp to ssp
                o->sp=o->ssp;
                o->pc=*puData[0];
                break;

                //Llamadas a extensiones
            case I_CALLE:
                puData[0]=uintp(o->progMemory,o->pc+1);
                callc(*puData[0],o->stack,o->sp,o->currentProg);

                o->pc+=5;
                break;

            case I_HALT://Termina el programa
            //		SetEvent(hEventProcessEnd);
                processEnded(o->currentProg,o->index);
                break;
            }

             o->backUp32L();
        }

            o->index++;

    }
    return 0;
}

gVmInstance *gVm::load(const gString &gVMProgramFile,bool bCreateMemory,
                       bool isActive)
{
    //loads a program
    if(!d)return 0;
    ca;
        FILE *f;//Archivo
        EPROGHEADER ph;
        EPROGINFO pi;
        gVmInstance *pProg;
        gu32 *dwMagic=(gu32 *)Magic;
        gu32 dwSize,i,*pdwData,dwStrSize,dwOffset=0;
        gu16 wData;
        gu8 bData,*pData;

        if(o->ram.isEmpty())return NULL;
        f=fopen(gVMProgramFile.ascii(),"rb");//Abrimos el archivo
        if(!f)return NULL; //Ocurrio un error

        fread(&ph,sizeof(EPROGHEADER),1,f);//Leemos la cabecera
        if(ph.dwMagic!=*dwMagic)return false;
        if((ph.wVersion !=0x1000)&& ph.wVersion!=0x1100)return false;

        fread(&pi,sizeof(EPROGINFO),1,f);//Leemos la informacion

        //Creamos la instancia
        pProg=new gVmInstance();


        if(!pProg)
        {
            fclose(f);
            return NULL;
        };
        //Se crea la memoria de datos
        if(bCreateMemory)
        {
    //Allocate Datamemory
            //Data memory will be heap and stack
            //changing this from old code from this
            //dwSize=pi.dwVMSize+pi.dwStackSize+sizeof(VIRTMEM)*2;
            //to this
            dwSize=pi.dwVMSize+gVirtualMemory::blockHeaderSize();

            if((pData=(gu8 *)o->ram.vmAlloc(dwSize))==NULL)
            {
                fclose(f);
                delete pProg;
                return NULL;
            }
            //Create the memspace for this instance comming from the data
            pProg->dataMemory.create(dwSize,pData);
            //pProg->dwHeapSize=dwSize;
            //pProg->memHandler.CreateVirtualMemory(dwSize,VMEM_SUBMEM,NULL,pProg->DataMemory);
            //Se crea la memoria para el stack
            if((pData=(gu8 *)o->ram.vmAlloc(pi.dwStackSize))==NULL)
            {

                fclose(f);
                //m_VirtualMemory.FreeVirtMem(pProg->DataMemory);
                o->ram.vmFree(pProg->dataMemory.data());
                delete pProg;
                return NULL;
            };
            //pProg->dwStackSize=pi.dwStackSize;
            pProg->stack.create(pi.dwStackSize,pData);
            /*OLD CODE
            pProg->dwStackAddressEnd=pProg->dwStackAddressStart+pi.dwStackSize-1;
            No globals yet
            pProg->dwGlobalVarStart=0;
           pProg->dwGlobalVarEnd=0;
            pProg->pProgItems=new PROGITEMS;
            if(!pProg->pProgItems)
            {
                fclose(f);
                delete pProg;
                return NULL;
            }
            pProg->pProgItems->dwPC=pi.dwStarAddress;//Inicio de programa
            pProg->pProgItems->dwSP =0;//Inicio de pila
            pProg->pProgItems->dwFSP=pi.dwStackSize-1;//Inicio de stack de funciones
            pProg->pProgItems->bRunned =false;*/
            //New code
            pProg->pc=pi.dwStarAddress;
            pProg->sp=0;
            pProg->fsp=pi.dwStackSize - 1;
            //runned?
        }
        //Se crea la memoria para el programa
        if((pData=(gu8 *)o->ram.vmAlloc(pi.dwProgramSize))==NULL)
        {
            fclose(f);
            if(bCreateMemory)
            {

                o->ram.vmFree(pProg->dataMemory.data());
                o->ram.vmFree(pProg->stack.data());
               // delete pProg->pProgItems;
                delete pProg;
                return NULL;
            }
        }
        //pProg->dwProgramSize=pi.dwProgramSize;
        pProg->programMemory.setData(pData,pi.dwProgramSize,false,false);


        /*************************************File Version 1.1********************************/
    /***************************Additions**************************************************/
        if(ph.wVersion==0x1100)
        {
            //Frist off lets get how many procs are stored there
            fread(&dwSize,sizeof(gu32),1,f);//We use dwSize to get the number
            //Once obtained we proceed to alloc memory
            if(dwSize>0)
            {
                //pProg->pList=new ProcList;
                //pProg->pList->Init(dwSize);//Initializating arrays
                pProg->procs.alloc(dwSize);
                gVmProcItem *item;
                for(i=0;i<dwSize;i++)
                {
                    //Now leemos primero el tamano de la cadena
                    item = new gVmProcItem();
                    fread(&wData,sizeof(gu16),1,f);
                    //Alocamos memoria para la cadena
                    //pProg->pList->pNames[i].Alloc(wData);
                    item->procName.alloc(wData,gString::ASCII);
                    //Obtenemos el nombre
                    //fread(pProg->pList->pNames[i].GetDataBuffer(),sizeof(char),wData,f);
                    fread(item->procName.ascii(),sizeof(char),wData,f);
                    //Ahora obtenemos la direccion del procedimiento
                    //fread(&pProg->pList->pdwAddresses[i],sizeof(dgu16),1,f);
                    fread(&item->procAddress,sizeof(gu32),1,f);
                    pProg->procs.setProc(i,item);
                }

            }
        }

        /*********************************************************************************/






        //Obtenemos el programa
        pData=pProg->programMemory.data();
        for(i=0;i<pi.dwProgramSize;i++)
        {
            fread(&pData[i],sizeof(gu8),1,f);
        }
        //Obtenemos el dato que indica si hay variables

        fread(&dwSize,sizeof(gu32),1,f);
        if(bCreateMemory)//Si se especifica la creacion de memoria se pasan las variables
        {
            pData=(gu8 *)pProg->dataMemory.data();


            if(!pData)
            {
                return NULL;
            }
            for(i=0;i<dwSize;i++)
            {
                fread(&bData,sizeof(char),1,f);

                switch(bData)
                {
                case 0://Es un entero
                    dwOffset=pProg->memAlloc(sizeof(gu32));
                    pdwData=uintp(pData,dwOffset);
                    fread(pdwData,sizeof(gu32),1,f);

                    break;
                case 1://Es un entero .flotante
                    dwOffset=pProg->memAlloc(sizeof(gu32));
                    pdwData=uintp(pData,dwOffset);
                    fread(pdwData,sizeof(gu32),1,f);

                    break;
                case 2://Cadena de caracteres

                    fread(&dwStrSize,sizeof(gu32),1,f);//Tamano
                    dwOffset=pProg->memAlloc(dwStrSize+1);//Incluyendo terminacion en 0
                    pdwData=uintp(pData,dwOffset);//Estraer direccion
                    fread(pdwData,sizeof(char),dwStrSize,f);
                    pData[dwOffset+dwStrSize]=0;

                    break;
                }
              /*  if(i==0)
                    pProg->dwGlobalVarStart=dwOffset;
                else if(i==dwSize-1)
                    pProg->dwGlobalVarEnd=dwOffset;*/
            }
            /*if(pProg->dwGlobalVarEnd)
                pProg->dwHeapAddressStart=pProg->dwGlobalVarEnd + sizeof(dgu16); //Since all vars are 32 bits integer
            else
                pProg->dwHeapAddressStart=pProg->dwStackAddressEnd+1; //Imediatly after the stack if no globals declared
            pProg->dwHeapAddressEnd=pProg->dwHeapSize-1; //Last element of array*/
        }




        fclose(f);//Cerramos el archivo
        /*if(bAddToProcess)
        {
        m_lPrograms.insertValue(pProg);
        }*/
        pProg->isActive=isActive;
        pProg->pid=o->programs.size()*(GVM_PIDINCREMENT) + GVM_PIDSTART;
        o->programs.append(pProg);
        //added new
        pProg->regs.alloc(MAX_REGISTERS);

        return pProg;
}

void gVm::stop()
{
    if(!d)return;
    ca;
    o->brun=false;
}


//removing
void gVm::removeInstanceByIndex(gu32 index)
{
    if(!d)return;
    ca;
    o->programs.remove(index,1);
}


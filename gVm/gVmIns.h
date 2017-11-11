////////EVM CPU INSTRUCTIONS

#ifndef _GVM_CPU_
#define _GVM_CPU_
enum GVM_CPU_INS
{
//Datos
	I_MOV,  //Dato simple
    I_MOV8, //8 bit data
    I_MOV16, //16 bit data
	I_MOVB, //Bloque de datos
	

	//Aritmeticas
	I_ADD,
	I_MUL,
	I_SUB,
	I_DIV,
    I_INC, //Increment REF+=NUM
    I_DEC, //INCREMENT REF-=NUM
	
	//Versiones para punto flotante
	I_FADD,
	I_FMUL,
	I_FSUB,
	I_FDIV,
	
	//Comparacion
	I_CMP,
	//Added for Version 1.1
	I_FCMP,
    //Added more extra cmps
    I_CMP8, //comparison of 8 bit values
    I_CMP16, //Comparison of 16 bit values

	//Booleanas
    I_AND8, //Operation with 8 bit
    I_AND16, //Operation with 16 bit
    I_OR8, //Operation with 8 bit
    I_OR16, //Operation with 16 bit
    I_XOR8, //Operation with 8 bit
    I_XOR16, //Operation with 16 bit
    I_AND, //Operation with 32 bit
    I_OR, //Operation with 32 bit
    I_XOR, //Operation with 32 bit
	//Not
	I_NOT,
    I_NOT8, //Operation with 8 bit
    I_NOT16, //Opeartion with 16 bit
        //Pila
	I_PUSH,
	I_POP,
	//Cambia el signo
	I_NEG,
	I_FNEG,
    I_ALLOC,
    I_FREE,
	//Salto	
	I_JE,
	I_JNE,
	I_JL,
	I_JG,
    I_JLE,
    I_JGE,
	I_JMP,



	//Procedimientos
	I_CALL,

	//Extensiones
	I_CALLE,
    //Added alloc

	//Find de programa

	I_HALT,
	I_RET,//Retorn de funcion
    I_NOP,//No operacion

    I_POLL=255 //End of program..if nor removed from process the keep sta

};


#endif

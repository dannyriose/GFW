#ifndef GVM_ASM_H
#define GVM_ASM_H


#include <gfw_global.h>
#include <gLinkedList/gLinkedList.h>
#include <gLinkedList/gtLinkedList.h>
#include <gLexer/gLexer.h>
#include <gVirtualMemory/gVirtualMemory.h>

#include "gVmIns.h"

//Macros

#define GET_KWORD(word,id) if(pToken->tokenString==word) \
                            {pToken->tokenID2=id;return true;}
#define IS_RESERVED() if(!GetReservedWord()) \
                     { WriteError("Sintaxis Error:Expected Reserved Word"); \
                      return;}
#define ISNOT_RESERVED() if(GetReservedWord()) \
                                { WriteError("Sintaxis Error:"); \
                                return;}
#define WRITE_ERR(msg) WriteError(msg);return

#define MATCH(id,msg)  getToken();if(pToken->tokenID1!=id) \
                                            { WRITE_ERR(msg);}
#define MATCHTOK(id,msg)if(pToken->tokenID1!=id) \
                                { WRITE_ERR(msg);}

#define BYTE_H_WRITE(byte,val) byte|=val<<4;
#define BYTE_L_WRITE(byte,val) byte|=val; 		
//Constants
#define ADDR_INDIRECT 0							
#define ADDR_DIRECT 1
#define PARAM_TYPE_VAR 0
#define PARAM_TYPE_LABEL 1
#define PARAM_TYPE_NUMLABEL 2
#define PARAM_TYPE_APICALL 3

//Addition to V1.1
#define PROC_TYPE_MAIN 0
#define PROC_TYPE_PROC 1
#define PROC_TYPE_LABEL 2
/***************************/

#ifdef MSVC
#pragma warning(disable :4620)
#pragma warning(disable :4251)
#endif

//Estructuras
//Cabecera de archivo
namespace gfw
{
    //! Header for program files
    static const gu8 Magic[]={'G','V','M',0};
    typedef struct
    {
        gu32 dwMagic;
        gu16 wVersion;
    }EPROGHEADER;

    //! File structure information
    typedef struct
    {
        gu32 dwStackSize;
        gu32 dwProgramSize;
        gu32 dwVMSize;//Tamano de la memoria virtual
        gu32 dwStarAddress;//Inicio

    }EPROGINFO;
    //! Define the variable of set of variables on memory
    struct gvmasm_offset
    {
        enum offset_types
        {
            OFFSET_NUMERIC,
            OFFSET_VARIABLE
        };

        gu16 offsettype;
        gString value;
        gvmasm_offset():offsettype(0)
        {

        }
        gvmasm_offset(const gvmasm_offset &other):offsettype(other.offsettype),value(other.value)
        {

        }

        gvmasm_offset &operator=(const gvmasm_offset &other)
        {
            offsettype=other.offsettype;
            value=other.value;
            return *this;
        }
    };
    //! A variable
    struct EVAR
    {
        gString szVarName;
        gString szVal;//Valor
        gu16 dwVarType;
        gu32 size;//the size of the variable if array defined
        gu32 spAddress;//only for local defs
        EVAR():size(0),spAddress(0){}

    };
    typedef EVAR *LPEVAR;
    //! Labels or procedures
    struct LBPROC
    {
        //! The name of the proc
        gString szLabelProcName;
        //! Address of the procedures
        gu32 dwAddress;
        //! Type of procedure
        gu16 wType;//
        //! The list of local variables
        gtLinkedList<EVAR> local;
        //! Finds a variable a and retunrs its pointer
        EVAR *getVariable(const gString &val);

    };
    typedef LBPROC *LPLBPROC;

    struct PARAM
    {
        gu16 wParamType;//Registro,Variable o valor
        gu16 wAddressMode;//Indirecto , Directo
        gString szName;//Id del parametro
        //newest offset
        gvmasm_offset offset;//
        //added for direct address
        gu32 daddress;
        //save now the calling procedure
        LBPROC *callProc;
        PARAM():daddress(0),callProc(0)
        {

        }
        PARAM(LBPROC *cp):daddress(0),callProc(cp){}
    };
    typedef PARAM *LPPARAM;
    struct EASMINS
    {
        gu16 wOpcode;//Opcode de la instruccion
        gLinkedList lParams;//Parametros
        EASMINS();
        PARAM *addParam(gu16 wParamType,gu16 addressMode,const gString &_name=gString(),LBPROC *p=0,gu32 _daddress=0);

    };
    typedef EASMINS *LPEASMINS;

    //Etiquetas y procedimientos


    //Funciones
    void CleanParam(void *);
    void CleanProc(void *);
    void CleanIns(void *);
    void CleanVars(void *);
    void CleanErr(void *);
    //Clases
    //! Class compiles an assambly language for the gVm virtual machine.
    class SHARED_GFW gVmAsm:private gLexer
    {
    private:
        gu32 dwPC;//PC
        gu32 dwSP;//Stackpointer
        LBPROC *procInside;//inside of which proc we are
        gu32 dwHeapSize;//Tamano de los datos
        gu32 dwStackSize;//Tamano de la pila
        gLinkedList ins;//Instrucciones
        gLinkedList vars;//Variables
        gLinkedList procs;//Procedimientos y etiquetas
        gLinkedList apidecl;//declaraciones api
        gLinkedList errlist;//Lista de errores
        gToken *pToken;//save a lot of time rewriting
        gvmasm_offset checkOffSet();//verifica el offset en un parametro
        bool GetReservedWord();
        void WriteError(const gString &str);
        bool IsVariable();
        bool IsVariable(const gString &var);
        bool IsLabelProc();
        bool IsLabelProc(const gString &lab);
        bool IsApiProc(const gString &api);
        //Added to version 1.1
        gu32 GetProcCount();//Regresa todos los procedmienntos solo procedmientos sin etiquetas
        //Regresa el identificador de la etiqueta
        LBPROC *GetLabelProc(const gString &);
        LBPROC *GetLabelProc(gu32 index);
        //Regresa el identificador de la llamada api
        LBPROC *GetApiProc(const gString &);
        //Regresa el procedimiento main
        LBPROC *GetProcMain();
        gu32 GetVarIndex(const gString &);
        gu32 GetVarsSizes();

        void ParseCode();//Analiza globalmente

        void ParseMain();//Analiza instrucciones ,etiquetas y funciones

        //Sub Parsers
        void ParseMOV();//Analiza la instruccion MOV
        void ParseMOVB();//Analiza la instruccion MOVB
        void ParseABC();//Analiza las instrucciones Aritmeticas y de bitwise
                        //y la decomparacion CMP
        void ParseNOTSI();//Analiza la instruccion NOT y PUSH y POP
        void ParseJumps();//Analiza las instrucciones de salto y la instruccion CALL
        void ParseHaltRetNop();//Analiza las instrucciones Halt y RET
        void ParseProc();//Procedimiento
        void ParseLabel();//Etiqueta
        void ParseComment();//Comentario
        void ParseHeapSize();//HeapSize
        void ParseStackSize();//StackSize
        void ParseVars();//Variables
        void ParseLocalVar();//Parse a local var
        void ParseApiDecl();//Declaraciones API
        void ParseApiCall();//Llamadas API
        void ParseInclude();//Inclusion de archivos
        //Verifica las etiquetas y los procedimientos con las instrucciones que los requieren
        bool VerifyLabelsAndProcs();
        bool VerifyApiDecls();

        //utility to parse a bracked
        bool parseBracketValue(gToken &out);
        //utility to parse a string
        bool parseString(gString &out);
        //utility to parse an array inside a ()
        bool parseArray( gString &out, gu32 type);//clever!!
        bool parseInt(gString &out);//parse a integer value
        bool parseFloat(gString &out);//parse a float value

        EASMINS *addInstruction(gu16 _ins);


    public:
        gVmAsm();//Constructor
        bool OpenVmAsmFile(const gString &fileName);
        bool Assamble(const gString &FileName);
        bool Assamble11(const gString &szFileName);//EVM File version 1.1
        void InitErrTour();
        const gString &GetError() const;
        bool endOfErrors() const;
        void clearAll();//clear all




    };
}

#endif

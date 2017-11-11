#ifndef GTOKENS_H
#define GTOKENS_H
#define TOKEN_TYPE_IDENT 100 //Identificador
#define TOKEN_TYPE_NUMINT 101 //Numero entero
#define TOKEN_TYPE_NUMFLOAT 102 //Numero entero con punto flotante
#define TOKEN_TYPE_PAROPEN 103 //Parentesis abrir
#define TOKEN_TYPE_PARCLOSE 104 //Parentesis cerrar
#define TOKEN_TYPE_COROPEN 105// Corchetes
#define TOKEN_TYPE_CORCLOSE 106
#define TOKEN_TYPE_KEYOPEN 107 //Abrir llave
#define TOKEN_TYPE_KEYCLOSE 108 //Cerrar llave
#define TOKEN_TYPE_COMA 109 //Coma
#define TOKEN_TYPE_DOTCOMA 110 //punto y coma
#define TOKEN_TYPE_TWODOTS 111 //2 puntos
#define TOKEN_TYPE_EXCLAMATIONOPEN 112//¡
#define TOKEN_TYPE_COMILLADBL 113 //comillas doble
#define TOKEN_TYPE_COMILLASIN 114 //comilla simple
#define TOKEN_TYPE_MONEY 115 //Signo de pesos
#define TOKEN_TYPE_PERCENT 116 //Porciento
#define TOKEN_TYPE_ANDSIMBOL 117 //&
#define TOKEN_TYPE_ORSIMBOL 118 //|
#define TOKEN_TYPE_DIAGONAL 119//  /
#define TOKEN_TYPE_INVDIAGONAL 120// \/
#define TOKEN_TYPE_QUESTIONOPEN 121 //¿
#define TOKEN_TYPE_QUESTIONCLOSE 122  // ?
#define TOKEN_TYPE_EXCLAMATIONCLOSE 123 //!//
#define TOKEN_TYPE_POWEROPERATOR 124 //^
#define TOKEN_TYPE_DOT 125 //.
#define TOKEN_TYPE_LESSTHAN 126 // <
#define TOKEN_TYPE_GREATTHAN  127//>
#define TOKEN_TYPE_PLUS 128 //+
#define TOKEN_TYPE_ASTERISCO 129 //*
#define TOKEN_TYPE_MINUS 130 //-
#define TOKEN_TYPE_GUIONDOWN 131//_
#define TOKEN_TYPE_ARROBA 132 //@
#define TOKEN_TYPE_WEIRDSIMBOL 133 //~
#define TOKEN_TYPE_EQUAL 134
#define TOKEN_TYPE_CAT 135
#define TOKEN_TYPE_GUIONNORMAL 136 //-
#define TOKEN_TYPE_CSTYLEHEX 137

//TOKENS INVALIDOS O FIN DE ARCHIVO

#define TOKEN_TYPE_ERROR 1000 //Cualquier simbolo invalido
#define TOKEN_TYPE_ENDOF_FILE 2000 //Fin de archivo
#endif // GTOKENS_H

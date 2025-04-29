#ifndef MEUSTIPOS
#define MEUSTIPOS

// tipos inteiros
typedef char I8; // de -128 a 127
typedef unsigned char U8; // de 0 a 255
typedef short int I16; // de -32768 a 32767
typedef unsigned short int U16; // de 0 a 65535
typedef int I32; // de -2147483648 a 2147483647
typedef unsigned int U32; // de 0 a 4294967295  
typedef long int I64;
typedef unsigned long int U64;

// tipos reais
typedef float R32;
typedef double R64;
typedef long double R128;

// tipo boolean
typedef enum {
    false,
    true
} boolean;

#endif
#ifndef CODIGO
#define CODIGO
#include "meus_tipos.h"

typedef struct{
    U8* byte /* vetor dinâmico de bytes */;
    U8  capacidade /* em bits */;
    U8  tamanho /* em bits */;
} Codigo;

boolean novo_codigo (Codigo* c /* por referência */);
void free_codigo (Codigo* c /* por referência */);

boolean adiciona_bit (Codigo* c /* por referência */,
                      U8 valor /* 0 ou 1 */);
boolean joga_fora_bit (Codigo* c /* por referência */);

boolean clone (Codigo original, /* por valor */
               Codigo* copia /* por referencia */);

#endif
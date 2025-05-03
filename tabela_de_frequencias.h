#ifndef TABELA_DE_FREQUENCIAS
#define TABELA_DE_FREQUENCIAS
#include "meus_tipos.h"

typedef struct {
    U8 byte;
    U64 frequencia;
} Elemento;

typedef struct Struct_do_no_de_arvore_binaria {
    struct Struct_do_no_de_arvore_binaria* esquerda;
    Elemento informacao;
    struct Struct_do_no_de_arvore_binaria* direita;
} Struct_do_no_de_arvore_binaria;

typedef Struct_do_no_de_arvore_binaria* Ptr_de_no_de_arvore_binaria;

typedef struct {
    Ptr_de_no_de_arvore_binaria vetor [256];
    U16 quantidade_de_posicoes_preenchidas;
} Tabela_de_frequencias;

void nova_tabela_de_frequencias (Tabela_de_frequencias* tab /* por referência */);
boolean inclua_byte (U8 byte /* por valor */, Tabela_de_frequencias* tab /* por referencia */);
void junte_nodos_no_inicio_do_vetor (Tabela_de_frequencias* tab /* por referencia */);
void ordenar_nos(Tabela_de_frequencias* tab); // Para ordenar os nós
boolean constroi_arvore_huffman(Tabela_de_frequencias* tab, Ptr_de_no_de_arvore_binaria* raiz); 
void imprime_arvore(Ptr_de_no_de_arvore_binaria raiz, int nivel);

#endif
#ifndef COMPACTADOR_H
#define COMPACTADOR_H
#include "codigo.h"
#include "meus_tipos.h"
#include "tabela_de_frequencias.h"

boolean compactar_arquivo(const char* entrada, const char* saida, Ptr_de_no_de_arvore_binaria raiz);

#endif
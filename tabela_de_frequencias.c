#include <stdio.h>
#include <stdlib.h>
#include "tabela_de_frequencias.h"

void nova_tabela_de_frequencias (Tabela_de_frequencias* tab /* por referência */)
{
    for (U16 i=0; i<256; i++)
        tab->vetor[i]=NULL;

    tab->quantidade_de_posicoes_preenchidas=0;
}
//funcao static, ou seja, so pode ser usada dentro do mesmo arquivo
static boolean novo_no_de_arvore_binaria (Ptr_de_no_de_arvore_binaria* novo /* por referencia */,
                                          Ptr_de_no_de_arvore_binaria esq /* por valor */,
                                          Elemento inf /* por valor */,
                                          Ptr_de_no_de_arvore_binaria dir /* por valor */)
{
    *novo = (Ptr_de_no_de_arvore_binaria)malloc(sizeof(Struct_do_no_de_arvore_binaria));

    if (*novo==NULL) return false;

    (*novo)->esquerda=esq;
    (*novo)->informacao=inf;
    (*novo)->direita=dir;

    return true;
}

boolean inclua_byte (U8 byte /* por valor */,
                     Tabela_de_frequencias* tab /* por referencia */)
{
    if (tab->vetor[byte]!=NULL)
    {
         tab->vetor[byte]->informacao.frequencia++;
         return true;
    }

    Elemento elem;
    elem.byte=byte;
    elem.frequencia=1;

    boolean OK = novo_no_de_arvore_binaria (&(tab->vetor[byte]), NULL, elem, NULL);
    if (!OK) return false;

    tab->quantidade_de_posicoes_preenchidas++;
    return true;
}
//essa função serve para agrupar os dados no inicio, para montar a árvore é necessário ordenar a lista e é isso que a função faz
void junte_nodos_no_inicio_do_vetor (Tabela_de_frequencias* tab /* por referencia */)
{
    U16 primeiro_NULL, primeiro_nao_NULL;

    for(;;) // forever
    {
        primeiro_NULL=0;
        while (primeiro_NULL<256 && tab->vetor[primeiro_NULL]!=NULL)
            primeiro_NULL++;
        if (primeiro_NULL==256) break;

        primeiro_nao_NULL=primeiro_NULL+1;
        while (primeiro_nao_NULL<256 && tab->vetor[primeiro_nao_NULL]==NULL)
            primeiro_nao_NULL++;
        if (primeiro_nao_NULL==256) break;

        tab->vetor[primeiro_NULL]=tab->vetor[primeiro_nao_NULL];
        tab->vetor[primeiro_nao_NULL]=NULL;
    }
}
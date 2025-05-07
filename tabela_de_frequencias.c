#include <stdio.h>
#include <stdlib.h>
#include "tabela_de_frequencias.h"

void nova_tabela_de_frequencias (Tabela_de_frequencias* tab)
{
    for (U16 i=0; i<256; i++)
        tab->vetor[i]=NULL;

    tab->quantidade_de_posicoes_preenchidas=0;
}
static boolean novo_no_de_arvore_binaria (Ptr_de_no_de_arvore_binaria* novo, Ptr_de_no_de_arvore_binaria esq, Elemento inf ,Ptr_de_no_de_arvore_binaria dir)
{
    *novo = (Ptr_de_no_de_arvore_binaria)malloc(sizeof(Struct_do_no_de_arvore_binaria));

    if (*novo==NULL) return false;

    (*novo)->esquerda=esq;
    (*novo)->informacao=inf;
    (*novo)->direita=dir;

    return true;
}

boolean inclua_byte (U8 byte , Tabela_de_frequencias* tab)
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
void junte_nodos_no_inicio_do_vetor (Tabela_de_frequencias* tab)
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

#include <stdlib.h>

int compara_frequencias(const void* a, const void* b) {
    Ptr_de_no_de_arvore_binaria noA = *(Ptr_de_no_de_arvore_binaria*)a;
    Ptr_de_no_de_arvore_binaria noB = *(Ptr_de_no_de_arvore_binaria*)b;

    if (noA == NULL && noB == NULL) return 0;
    if (noA == NULL) return 1; 
    if (noB == NULL) return -1; 

    return (noA->informacao.frequencia > noB->informacao.frequencia) - 
           (noA->informacao.frequencia < noB->informacao.frequencia);
}


void ordenar_nos(Tabela_de_frequencias* tab) {
    qsort(tab->vetor, 256, sizeof(Ptr_de_no_de_arvore_binaria), compara_frequencias);
}


boolean constroi_arvore_huffman(Tabela_de_frequencias* tab, Ptr_de_no_de_arvore_binaria* raiz) {
    while (tab->quantidade_de_posicoes_preenchidas > 1) {
        ordenar_nos(tab);

        Ptr_de_no_de_arvore_binaria esq = tab->vetor[0];
        Ptr_de_no_de_arvore_binaria dir = tab->vetor[1];

        Elemento novo_elem;
        novo_elem.byte = 0; 
        novo_elem.frequencia = esq->informacao.frequencia + dir->informacao.frequencia;

        Ptr_de_no_de_arvore_binaria novo_no;
        if (!novo_no_de_arvore_binaria(&novo_no, esq, novo_elem, dir)) return false;

        tab->vetor[0] = novo_no;  
        tab->vetor[1] = NULL;

        tab->quantidade_de_posicoes_preenchidas--;
    }

    *raiz = tab->vetor[0];
    return true;
}

void salvar_arvore(Ptr_de_no_de_arvore_binaria raiz, FILE* saida) {
    if (raiz == NULL) {
        fputc(0, saida);
        return;
    }

    fputc(1, saida);
    
    if (raiz->esquerda == NULL && raiz->direita == NULL) {
        fputc(1, saida);
        fputc(raiz->informacao.byte, saida); 
        fwrite(&(raiz->informacao.frequencia), sizeof(U32), 1, saida);
    } else {
        fputc(0, saida); 
    }
    
    salvar_arvore(raiz->esquerda, saida);
    salvar_arvore(raiz->direita, saida);
}


void imprime_arvore(Ptr_de_no_de_arvore_binaria raiz, int nivel) {
    if (raiz == NULL) return;

    for (int i = 0; i < nivel; i++) {
        printf("  ");
    }

    if (raiz->esquerda == NULL && raiz->direita == NULL)
        printf("'%c' (%lu)\n", raiz->informacao.byte, raiz->informacao.frequencia);
    else
        printf("(*) (%lu)\n", raiz->informacao.frequencia);

    imprime_arvore(raiz->esquerda, nivel + 1);
    imprime_arvore(raiz->direita, nivel + 1);
}

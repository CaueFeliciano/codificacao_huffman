#include <stdio.h>
#include <stdlib.h>
#include "meus_tipos.h"
#include "codigo.h"
#include "tabela_de_frequencias.h"

// Tamanho máximo do código de Huffman
#define MAX_CODIGO 256

// Estrutura para armazenar o código binário de cada byte
typedef struct {
    U8 bits[MAX_CODIGO];
    int tamanho;
} Codigo;

// Tabela de códigos gerados
Codigo tabela_codigos[256];

// Função recursiva para gerar os códigos a partir da árvore de Huffman
void gerar_codigos(Noh* raiz, Codigo tabela[], U8 caminho[], int profundidade) {
    if (!raiz) return;

    if (!raiz->esq && !raiz->dir) {
        tabela[raiz->informacao.byte].tamanho = profundidade;
        for (int i = 0; i < profundidade; i++) {
            tabela[raiz->informacao.byte].bits[i] = caminho[i];
        }
        return;
    }

    caminho[profundidade] = 0;
    gerar_codigos(raiz->esq, tabela, caminho, profundidade + 1);

    caminho[profundidade] = 1;
    gerar_codigos(raiz->dir, tabela, caminho, profundidade + 1);
}

// Salva a tabela de frequências no início do arquivo compactado
void salvar_tabela_de_frequencias(FILE* f, Tabela_de_frequencias* tab) {
    for (int i = 0; i < 256; i++) {
        U64 freq = 0;
        if (tab->vetor[i] != NULL) {
            freq = tab->vetor[i]->informacao.frequencia;
        }
        fwrite(&freq, sizeof(U64), 1, f);
    }
}

// Escreve os bits compactados no arquivo de saída
void escrever_bits(FILE* f, Codigo tabela[], const char* nome_arquivo_original) {
    FILE* entrada = fopen(nome_arquivo_original, "rb");
    if (!entrada) {
        perror("Erro ao abrir arquivo original");
        exit(1);
    }

    U8 byte;
    int buffer = 0;
    int bits_usados = 0;

    while (fread(&byte, sizeof(U8), 1, entrada) == 1) {
        Codigo cod = tabela[byte];
        for (int i = 0; i < cod.tamanho; i++) {
            buffer <<= 1;
            buffer |= cod.bits[i];
            bits_usados++;

            if (bits_usados == 8) {
                U8 saida = (U8) buffer;
                fwrite(&saida, sizeof(U8), 1, f);
                buffer = 0;
                bits_usados = 0;
            }
        }
    }

    // Escreve os bits restantes no final do arquivo
    if (bits_usados > 0) {
        buffer <<= (8 - bits_usados);
        U8 saida = (U8) buffer;
        fwrite(&saida, sizeof(U8), 1, f);
    }

    fclose(entrada);
}

// Função principal de compactação
void compactar_arquivo(const char* nome_entrada, const char* nome_saida) {
    FILE* entrada = fopen(nome_entrada, "rb");
    if (!entrada) {
        perror("Erro ao abrir arquivo de entrada");
        exit(1);
    }

    Tabela_de_frequencias tab;
    nova_tabela_de_frequencias(&tab);

    // Lê bytes do arquivo original para construir tabela de frequências
    U8 byte;
    while (fread(&byte, sizeof(U8), 1, entrada) == 1) {
        inclua_byte(byte, &tab);
    }
    fclose(entrada);

    FILE* saida = fopen(nome_saida, "wb");
    if (!saida) {
        perror("Erro ao criar arquivo de saída");
        exit(1);
    }

    // Salva as frequências no início do arquivo compactado
    salvar_tabela_de_frequencias(saida, &tab);

    // Constrói a árvore de Huffman
    Noh* raiz = construir_arvore_de_huffman(&tab);

    // Gera a tabela de códigos
    U8 caminho[MAX_CODIGO];
    gerar_codigos(raiz, tabela_codigos, caminho, 0);

    // Escreve os dados compactados
    escrever_bits(saida, tabela_codigos, nome_entrada);

    fclose(saida);
    printf("Compactação concluída com sucesso!\n");
}
// Eu tenteiii 


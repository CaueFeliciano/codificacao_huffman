#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "meus_tipos.h"
#include "codigo.h"
#include "tabela_de_frequencias.h"
#include "compactar.h"

int main() {
    int opcao;
    printf("Escolha uma opção:\n");
    printf("1 - Rodar teste com vetor fixo (Testar árvore)\n");
    printf("2 - Compactar arquivo real\n");
    printf("Opção: ");
    scanf("%d", &opcao);

    if (opcao == 1) {
        Tabela_de_frequencias tab;
        nova_tabela_de_frequencias(&tab);

        U8 dados[] = { 'a', 'b', 'a', 'c', 'a', 'b', 'c' };
        int n = sizeof(dados) / sizeof(dados[0]);

        for (int i = 0; i < n; i++) {
            inclua_byte(dados[i], &tab);
        }

        printf("Tabela de frequências:\n");
        for (int i = 0; i < 256; i++) {
            if (tab.vetor[i] != NULL) {
                printf("Byte '%c' (%d): %lu vezes\n", i, i, tab.vetor[i]->informacao.frequencia);
            }
        }

        Ptr_de_no_de_arvore_binaria raiz;
        if (constroi_arvore_huffman(&tab, &raiz)) {
            printf("\nÁrvore de Huffman construída!\n");
        } else {
            printf("\nErro ao construir a árvore de Huffman.\n");
            return 1;
        }

        Codigo codigo;
        if (!novo_codigo(&codigo)) {
            printf("Erro ao criar código.\n");
            return 1;
        }

        adiciona_bit(&codigo, 1);
        adiciona_bit(&codigo, 0);
        adiciona_bit(&codigo, 1);
        adiciona_bit(&codigo, 1);
        adiciona_bit(&codigo, 0);

        printf("\nTamanho atual do código: %d bits\n", codigo.tamanho);

        printf("\nÁrvore montada:\n");
        imprime_arvore(raiz, 0);

        free_codigo(&codigo);
    }

    else if (opcao == 2) {
        char entrada[100], saida[100];
        printf("Nome do arquivo de entrada: ");
        scanf("%s", entrada);
        printf("Nome do arquivo compactado: ");
        scanf("%s", saida);

        FILE* arq = fopen(entrada, "rb");
        if (arq == NULL) {
            printf("Erro ao abrir arquvo de entrada.\n");
            return 1;
        }

        Tabela_de_frequencias tab;
        nova_tabela_de_frequencias(&tab);

        U8 byte;
        while (fread(&byte, 1, 1, arq) == 1) {
            inclua_byte(byte, &tab);
        }

        fclose(arq);

        Ptr_de_no_de_arvore_binaria raiz;
        if (!constroi_arvore_huffman(&tab, &raiz)) {
            printf("Erro ao construir a árvore de Huffman.\n");
            return 1;
        }

        if (compactar_arquivo(entrada, saida, raiz)) {
            printf("Arquivo compactado com sucesso!\n");
        } else {
            printf("Erro na compactação do arquivo.\n");
        }
    }

    else {
        printf("Opção inválida.\n");
    }

    return 0;
}

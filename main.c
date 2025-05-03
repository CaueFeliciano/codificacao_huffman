#include <stdio.h>
#include "meus_tipos.h"
#include "codigo.h"
#include "tabela_de_frequencias.h"

// Declaração da função de compactação
void compactar_arquivo(const char* nome_entrada, const char* nome_saida);

int main() {
    int escolha;
    printf("Escolha uma opção:\n");
    printf("1 - Rodar teste com vetor fixo (a, b, a, c...)\n");
    printf("2 - Compactar arquivo real\n");
    printf("Opção: ");
    scanf("%d", &escolha);

    if (escolha == 1) {
        // ---------- TESTE COM VETOR FIXO ----------
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
        printf("chamando constroi_arvore");
        if (constroi_arvore_huffman(&tab, &raiz)) {
            printf("\nÁrvore de Huffman construída com sucesso!\n");
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

        printf("\nArvore montada:\n");
        imprime_arvore(raiz, 0);

        free_codigo(&codigo);
    }

    else if (escolha == 2) {
        // ---------- COMPACTAÇÃO DE ARQUIVO ----------
        char entrada[100], saida[100];
        printf("Digite o nome do arquivo de entrada: ");
        scanf("%s", entrada);
        printf("Digite o nome do arquivo compactado de saída: ");
        scanf("%s", saida);

        compactar_arquivo(entrada, saida);
    }

    else {
        printf("Opção inválida.\n");
        return 1;
    }

    return 0;
}

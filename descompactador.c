#include <stdio.h>
#include <stdlib.h>
#include "descompactador.h"
#include "codigo.h"
#include "tabela_de_frequencias.h"

static int ler_bit(U8 byte, int posicao) {
    return (byte >> (7 - posicao)) & 1;
}

static boolean eh_folha(Ptr_de_no_de_arvore_binaria no) {
    return (no->esquerda == NULL && no->direita == NULL);
}

static Ptr_de_no_de_arvore_binaria carregar_arvore(FILE* arquivo) {
    if (arquivo == NULL) return NULL;
    
    U8 bit_atual;
    if (fread(&bit_atual, 1, 1, arquivo) != 1) {
        return NULL;
    }
    
    if (bit_atual == 1) {
        U8 valor;
        if (fread(&valor, 1, 1, arquivo) != 1) {
            return NULL;
        }
        
        Ptr_de_no_de_arvore_binaria no = (Ptr_de_no_de_arvore_binaria)malloc(sizeof(Struct_do_no_de_arvore_binaria));
        if (no == NULL) return NULL;
        
        no->informacao.byte = valor;
        no->informacao.frequencia = 0;  // A frequência não é necessária para a descompactação
        no->esquerda = NULL;
        no->direita = NULL;
        
        return no;
    } 
    // Se o bit atual é 0, nó é um nó interno
    else if (bit_atual == 0) {
        // Criar o nó interno
        Ptr_de_no_de_arvore_binaria no = (Ptr_de_no_de_arvore_binaria)malloc(sizeof(Struct_do_no_de_arvore_binaria));
        if (no == NULL) return NULL;
        
        no->informacao.frequencia = 0;  // A frequência não é necessária para a descompactação
        no->informacao.byte = 0;        // Para nós internos, o byte não importa
        
        no->esquerda = carregar_arvore(arquivo);
        no->direita = carregar_arvore(arquivo);
        
        if (no->esquerda == NULL || no->direita == NULL) {
            // Liberar memória em caso de erro
            if (no->esquerda != NULL) free(no->esquerda);
            if (no->direita != NULL) free(no->direita);
            free(no);
            return NULL;
        }
        
        return no;
    } else {
        return NULL;
    }
}

static void free_arvore_binaria(Ptr_de_no_de_arvore_binaria no) {
    if (no == NULL) return;
    
    // Liberar recursivamente os filhos
    free_arvore_binaria(no->esquerda);
    free_arvore_binaria(no->direita);
    
    // Liberar o nó atual
    free(no);
}

boolean descompactar_arquivo(const char* nome_entrada, const char* nome_saida) {
    FILE* entrada = fopen(nome_entrada, "rb");
    if (entrada == NULL) return false;
    
    FILE* saida = fopen(nome_saida, "wb");
    if (saida == NULL) {
        fclose(entrada);
        return false;
    }
    
    Ptr_de_no_de_arvore_binaria raiz = carregar_arvore(entrada);
    if (raiz == NULL) {
        fclose(entrada);
        fclose(saida);
        return false;
    }
    
    U8 byte_atual;
    int bit_pos = 0;
    boolean fim_arquivo = false;
    
    if (fread(&byte_atual, 1, 1, entrada) != 1) {
        free_arvore_binaria(raiz);
        fclose(entrada);
        fclose(saida);
        return false;
    }
    
    while (!fim_arquivo) {
        Ptr_de_no_de_arvore_binaria no_atual = raiz;
        
        // Navegar pela árvore até encontrar uma folha
        while (no_atual != NULL && !eh_folha(no_atual)) {
            // Ler o próximo bit
            int bit = ler_bit(byte_atual, bit_pos);
            
            // Avançar para o próximo bit
            bit_pos++;
            if (bit_pos == 8) {
                if (fread(&byte_atual, 1, 1, entrada) != 1) {
                    if (no_atual != raiz) {
                        // Se estivermos no meio da decodificação de um caractere, pode haver um problema no arquivo compactado
                        fim_arquivo = true;
                        break;
                    }
                    fim_arquivo = true;
                    break;
                }
                bit_pos = 0;
            }
            
            // Navega pela árvore
            if (bit == 0) {
                no_atual = no_atual->esquerda;
            } else {
                no_atual = no_atual->direita;
            }
        }
        
        // Se encontramos uma folha, escrevemos seu valor no arquivo de saída
        if (no_atual != NULL && eh_folha(no_atual) && !fim_arquivo) {
            U8 caractere = no_atual->informacao.byte;
            fwrite(&caractere, 1, 1, saida);
        }
    }
    free_arvore_binaria(raiz);
    fclose(entrada);
    fclose(saida);
    
    return true;
}
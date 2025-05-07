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

    U8 existe_no;
    if (fread(&existe_no, 1, 1, arquivo) != 1) {
        return NULL;
    }
    
    if (existe_no == 0) {
        return NULL;
    }
    
    U8 eh_folha;
    if (fread(&eh_folha, 1, 1, arquivo) != 1) {
        return NULL;
    }
    
    Ptr_de_no_de_arvore_binaria no = (Ptr_de_no_de_arvore_binaria)malloc(sizeof(Struct_do_no_de_arvore_binaria));
    if (no == NULL) return NULL;
    
    if (eh_folha == 1) {
        U8 valor;
        if (fread(&valor, 1, 1, arquivo) != 1) {
            free(no);
            return NULL;
        }
        
        U32 frequencia;
        if (fread(&frequencia, sizeof(U32), 1, arquivo) != 1) {
            free(no);
            return NULL;
        }
        
        no->informacao.byte = valor;
        no->informacao.frequencia = frequencia;
        no->esquerda = NULL;
        no->direita = NULL;
    } else {
        no->informacao.byte = 0; 
        no->informacao.frequencia = 0; 
        
        no->esquerda = carregar_arvore(arquivo);
        no->direita = carregar_arvore(arquivo);
        
        if (no->esquerda != NULL && no->direita != NULL) {
            no->informacao.frequencia = no->esquerda->informacao.frequencia + 
                                        no->direita->informacao.frequencia;
        }
    }
    
    return no;
}

static void free_arvore_binaria(Ptr_de_no_de_arvore_binaria no) {
    if (no == NULL) return;
    
    free_arvore_binaria(no->esquerda);
    free_arvore_binaria(no->direita);
    
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

    long tamanho_original;
    if (fread(&tamanho_original, sizeof(long), 1, entrada) != 1) {
        free_arvore_binaria(raiz);
        fclose(entrada);
        fclose(saida);
        return false;
    }

    long bytes_descompactados = 0;

    U8 byte_atual;
    int bit_pos = 0;
    
    if (fread(&byte_atual, 1, 1, entrada) != 1) {
        free_arvore_binaria(raiz);
        fclose(entrada);
        fclose(saida);
        return false;
    }

    while (bytes_descompactados < tamanho_original) {
        Ptr_de_no_de_arvore_binaria no_atual = raiz;

        while (no_atual != NULL && !(no_atual->esquerda == NULL && no_atual->direita == NULL)) {
            int bit = (byte_atual >> (7 - bit_pos)) & 1;

            bit_pos++;
            if (bit_pos == 8) {
                if (fread(&byte_atual, 1, 1, entrada) != 1) {
                    if (feof(entrada)) {
                        int bits_validos;
                        fseek(entrada, -sizeof(int), SEEK_END);
                        fread(&bits_validos, sizeof(int), 1, entrada);
                        
                        if (bit_pos >= bits_validos) {
                            break;
                        }
                    }
                    
                    free_arvore_binaria(raiz);
                    fclose(entrada);
                    fclose(saida);
                    return false;
                }
                bit_pos = 0;
            }

            if (bit == 0) {
                no_atual = no_atual->esquerda;
            } else {
                no_atual = no_atual->direita;
            }
        }

        if (no_atual != NULL && no_atual->esquerda == NULL && no_atual->direita == NULL) {
            U8 caractere = no_atual->informacao.byte;
            fwrite(&caractere, 1, 1, saida);
            bytes_descompactados++;
        } else {
            break;
        }
    }

    // Limpeza
    free_arvore_binaria(raiz);
    fclose(entrada);
    fclose(saida);

    return bytes_descompactados == tamanho_original;
}
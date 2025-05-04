#include <stdio.h>
#include "codigo.h"
#include "tabela_de_frequencias.h"

boolean compactar_arquivo(const char* nome_entrada,
                          const char* nome_saida,
                          Ptr_de_no_de_arvore_binaria raiz) {
    FILE* entrada = fopen(nome_entrada, "rb");
    if (entrada == NULL) return false;

    FILE* saida = fopen(nome_saida, "wb");
    if (saida == NULL) {
        fclose(entrada);
        return false;
    }

    Codigo codigos[256];
    for (int i = 0; i < 256; i++)
        novo_codigo(&codigos[i]);

    Codigo atual;
    novo_codigo(&atual);
    gerar_codigos_bit(raiz, codigos, atual, 0);
    free_codigo(&atual);

    salvar_arvore(raiz, saida); 

    U8 byte_lido;
    Codigo acumulador;
    novo_codigo(&acumulador);

    while (fread(&byte_lido, 1, 1, entrada) == 1) {
        Codigo* codigo = &codigos[byte_lido];

        for (int i = 0; i < codigo->tamanho; i++) {
            int byte_idx = i / 8;
            int bit_offset = 7 - (i % 8);
            U8 bit = (codigo->byte[byte_idx] >> bit_offset) & 1;
            adiciona_bit(&acumulador, bit);

            if (acumulador.tamanho == 8) {
                fwrite(acumulador.byte, 1, 1, saida);
                acumulador.tamanho = 0;
                acumulador.byte[0] = 0;
            }
        }
    }

    if (acumulador.tamanho > 0) {
        fwrite(acumulador.byte, 1, 1, saida);
    }

    fclose(entrada);
    fclose(saida);

    for (int i = 0; i < 256; i++)
        free_codigo(&codigos[i]);

    free_codigo(&acumulador);

    return true;
}


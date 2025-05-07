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

fseek(entrada, 0, SEEK_END);
long tamanho_original = ftell(entrada);
fseek(entrada, 0, SEEK_SET);

fwrite(&tamanho_original, sizeof(long), 1, saida);

U8 byte_lido;
U8 byte_saida = 0;
int bits_usados = 0;

while (fread(&byte_lido, 1, 1, entrada) == 1) {
Codigo* codigo = &codigos[byte_lido];

for (int i = 0; i < codigo->tamanho; i++) {
int byte_idx = i / 8;
int bit_offset = 7 - (i % 8);
U8 bit = (codigo->byte[byte_idx] >> bit_offset) & 1;

if (bit)
byte_saida |= (1 << (7 - bits_usados));

bits_usados++;

if (bits_usados == 8) {
fwrite(&byte_saida, 1, 1, saida);
byte_saida = 0;
bits_usados = 0;
}
}
}


if (bits_usados > 0) {
fwrite(&byte_saida, 1, 1, saida);
}

fwrite(&bits_usados, sizeof(int), 1, saida);

fclose(entrada);
fclose(saida);

for (int i = 0; i < 256; i++)
free_codigo(&codigos[i]);

return true;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pixel
{
    int r;
    int g;
    int b;
} Pixel;

typedef struct Imagem
{
    int largura;
    int altura;
    int valmax;
    Pixel *pixels;
} Imagem;

Imagem *criarImagem(int altura, int largura, int valmax);

Imagem *lerArquivoPpm(char *nome_arquivo);

Imagem *aplicarFiltroCinza(Imagem *imagem);

void criarArquivoPpm(char *nome_arquivo, Imagem *imagem);
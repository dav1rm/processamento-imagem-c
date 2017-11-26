#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

# define M_PI 3.14159265358979323846

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
    Pixel **pixels;
} Imagem;

Imagem *criarImagem(int altura, int largura, int valmax);

Imagem *lerArquivoPpm(char *nome_arquivo);

Pixel *verificarPixel(Imagem *imagem, int largura, int altura);

Imagem *aplicarFiltroCinza(Imagem *imagem);

Imagem *aplicarFiltroGaussiano(Imagem *imagem);

Imagem *aplicarFiltroSobel(Imagem *imagem);

Imagem * aplicarBinarizacao(Imagem *imagem);

void criarArquivoPpm(char *nome_arquivo, Imagem *imagem);

Imagem *aplicarTransformadaHough(Imagem *imagem, Imagem *original);
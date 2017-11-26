#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "imagem.h"

int main()
{
    /*
    Código para executar: 

    gcc -std=c99 -c imagem.c -lm
    gcc -std=c99 -o principal imagem.o principal.c -lm
    ./principal

    Ou..

    gcc -c imagem.c -lm
    gcc -o principal imagem.o principal.c -lm
    ./principal

    */

    Imagem *imagem;
    imagem = lerArquivoPpm("Normal.ppm");

    Imagem *novaImagem;
    novaImagem = aplicarFiltroCinza(imagem);


    criarArquivoPpm("imagem_cinza.ppm", novaImagem);

    Imagem *imagemGaus;
    imagemGaus = aplicarFiltroGaussiano(novaImagem);
    criarArquivoPpm("imagem_gaus.ppm", imagemGaus);

    Imagem *imagemSobel;
    imagemSobel = aplicarFiltroSobel(imagemGaus);

    criarArquivoPpm("imagem_sobel.ppm", imagemSobel);

    Imagem *imagemBinarizada;
    imagemBinarizada = aplicarBinarizacao(imagemSobel);

    criarArquivoPpm("imagem_binarizada.ppm", imagemBinarizada);

    Imagem *imagemHough;
    imagem = lerArquivoPpm("Normal.ppm");
    imagemHough = aplicarTransformadaHough(imagemBinarizada, imagem);

    criarArquivoPpm("imagem_hough.ppm", imagemHough);

    return 0;
}

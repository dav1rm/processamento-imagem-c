#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "imagem.h"

int main(int argc, char *argv[])
{
    char *nome_imagem = argv[2];
    char *nome_diagnostico = argv[6];

    Imagem *imagem;
    printf("Lendo a imagem...\n");
    imagem = lerArquivoPpm(nome_imagem);

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
    imagem = lerArquivoPpm(nome_imagem);
    imagemHough = aplicarTransformadaHough(imagemBinarizada, imagem);

    criarArquivoPpm("imagem_hough.ppm", imagemHough);

    diagnosticarPaciente(imagemHough, nome_diagnostico);

    printf("Finalizado!\n");

    return 0;
}

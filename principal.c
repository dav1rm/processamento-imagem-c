#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "imagem.h"

int main()
{
    Imagem *imagem;
    imagem = lerArquivoPpm("Catarata2.ppm");

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

    return 0;
}

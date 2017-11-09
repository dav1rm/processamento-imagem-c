#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imagem.h"

int main()
{
    Imagem *imagem;
    imagem = lerArquivoPpm("Normal2.ppm");

    Imagem *novaImagem;
    novaImagem = aplicarFiltroCinza(imagem);
    

    criarArquivoPpm("imagem_cinza.ppm", novaImagem);

    return 0;
}
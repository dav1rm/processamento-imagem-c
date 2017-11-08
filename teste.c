#include <stdio.h>
#include <stdlib.h>

typedef struct Pixel{
    int r;
    int g;
    int b;
} Pixel;

typedef struct Imagem{
    int largura;
    int altura;
    int valmax;
    Pixel *pixels;
} Imagem;

Imagem *carregarImagem(char *nome_img){
    FILE *imagem;
    imagem = fopen(nome_img, "r");

    if (imagem != NULL) //Verifica se o arquivo foi lido corretamente
    {
        char formato_img[4];
        int altura, largura, valmax;
        fgets(formato_img, 4, imagem);
        printf("%s",formato_img);
        
    }
    else
    {
        printf("O arquivo não pode ser aberto.\n");
    }
    fclose(imagem);
    return imagem;
}

int main()
{
    Imagem imagem = carregarImagem("Normal.ppm");

    return 0;
}
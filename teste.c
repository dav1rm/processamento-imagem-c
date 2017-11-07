#include <stdio.h>
#include <stdlib.h>
int main()
{
    FILE *imagem;
    imagem = fopen("Normal.ppm", "r");

    if (imagem != NULL)
    {

    }else{
        printf("O arquivo não pode ser aberto.\n");
        return 0;
    }

    fclose(imagem);
    return 0;
}
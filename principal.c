/*************************************/
/* le-pgm.c ASaa@20061129 */
/*************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NAME 256 /* tamanho maximo para nome de arquivo */
#define MAX 1020     /* tamanho maximo para a matriz \
    e, consequentemente, para a figura, em pixels)*/

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

/*LEITURA DE IMAGEM */

Imagem * lerImagem(char *fname){
    char key[128];
    char b[100];
    char *image;
    char c;
    int red, blue, green;
    int i, j, n, m, max, M[MAX][MAX];
   
    arq = fopen(fname, "r");
    if (arq == NULL)
    {
        printf("Erro na abertura do arquivo %s\n", fname);
        return 0;
    }
    /* le dados do cabecalho */
    fscanf(arq, "%s", key);
    if (strcmp(key, "P3") != 0)
    {
        printf("Arquivo nao e um PPM\n");
        fclose(arq);
        return 0;
    }

    c = getc(arq);
    if (c == '\n' || c == '\r') // Skip any line break and comments
    {
        c = getc(arq);
        while(c == '#') 
        {
            fscanf(arq, "%[^\n\r] ", b);
            printf("%s\n",b);
            c = getc(arq);
        }
        ungetc(c,arq); 
    }
    fscanf(arq, "%d %d %d", &n, &m, &max);
    
    printf("%d rows  %d columns  max value= %d\n",n,m,max);
    
    int numbytes = n * m * 3;
    image = (char *) malloc(numbytes);
    if (image == NULL)
    {
        printf("Memory allocation failed!\n"); 
    }

	for(i=m-1;i>=0;i--) for(j=0;j<n;j++) // Important bug fix here!
    { // i = row, j = column
        fscanf(arq,"%d %d %d",&red, &green, &blue );
        image[(i*n+j)*3]=red * 255 / max;
        image[(i*n+j)*3+1]=green * 255 / max;
        image[(i*n+j)*3+2]=blue * 255 / max;
    }

    printf("read image\n");

    return image;
}
int main()
{
    Imagem * imagem;
    imagem = lerImagem("Normal2.ppm");

    return 0;
}
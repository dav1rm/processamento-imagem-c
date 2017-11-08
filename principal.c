/*************************************/
/* le-pgm.c ASaa@20061129 */
/*************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NAME 256 /* tamanho maximo para nome de arquivo */
#define MAX 1020     /* tamanho maximo para a matriz \
    e, consequentemente, para a figura, em pixels)*/

/*-----------------------------------------------------------
 void skipComment(FILE *fin)
 PPM allows comment lines, this routine skips over comments
 fin points an already open (binary read) file
 ------------------------------------------------------------*/
void skipComment(FILE *fin)
{
    char buf[120]; // assumes at most 120 character comments

    while (1)
    {
        fscanf(fin, "%1s", &buf);
        if (buf[0] != '#')
        {
            ungetc(buf[0], fin);
            return;
        }
        else
            fgets(buf, 120, fin); /* skip comment line */
    }
}

int main()
{
    FILE *arq;
    char fname[MAX_NAME];
    char key[128];
    int i, j, n, m, max, M[MAX][MAX];
    printf("Digite o nome do arquivo PPM de entrada: ");
    scanf("%s", fname);
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

    char buf[120]; // assumes at most 120 character comments

    while (1)
    {
        fscanf(arq, "%1s", &buf);
        if (buf[0] != '#')
        {
            ungetc(buf[0], arq);
        }
        else
            fgets(buf, 120, arq); /* skip comment line */
    }

    fscanf(arq, "%d %d", &m, &n);
    printf("formato: %d\n", m);
    /* le os dados da imagem e armazena na matrix M */
    for (i = 0; i <= n - 1; i++)
        for (j = 0; j <= m - 1; j++)
            fscanf(arq, " %d ", &M[i][j]);

    for (i = 0; i <= n - 1; i++)
    {
        for (j = 0; j <= m - 1; j++)
        {
            printf(" %d ", M[i][j]);
        }
        printf("\n");
    }

    fclose(arq);
    return 0;
}
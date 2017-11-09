#include "imagem.h"

Imagem *criarImagem(int altura, int largura, int valmax)
{
    Imagem *imagem = (Imagem *)malloc(sizeof(Imagem));
    imagem->pixels = (Pixel *)malloc((largura * altura) * sizeof(Pixel));
    imagem->largura = largura;
    imagem->altura = altura;
    imagem->valmax = valmax;
    return imagem;
}

Imagem *lerArquivoPpm(char *nome_arquivo)
{
    int largura, altura, valmax;
    Imagem *imagem = NULL;
    char formato[128];
    char comentario[100];
    char c;

    FILE *arquivo = fopen(nome_arquivo, "r"); //Abrindo arquivo em modo leitura

    if (arquivo == NULL) //Verificando se o arquivo foi lido corretamente
    {
        printf("Erro na abertura do arquivo %s\n", nome_arquivo);
        return 0;
    }

    fscanf(arquivo, "%s", formato); //Salva o formato do arquivo ppm
    if (strcmp(formato, "P3") != 0) //Verifica se é do formatp P3
    {
        printf("A imagem não é do formato p3\n");
        fclose(arquivo);
        return 0;
    }

    c = getc(arquivo);
    if (c == '\n' || c == '\r') // Ignora quebra de linha ou espaçamento
    {
        c = getc(arquivo);
        while (c == '#')
        {
            fscanf(arquivo, "%[^\n\r] ", comentario); //Salva o comentario
            //printf("%s\n", b); //Imprime o comentário
            c = getc(arquivo);
        }
        ungetc(c, arquivo);

        fscanf(arquivo, "%d %d %d", &largura, &altura, &valmax); //Lendo largura, altura e valor máximo de cada pixel da imagem

        printf("%d linhas,  %d colunas e valor máximo de cor= %d\n", largura, altura, valmax);

        imagem = criarImagem(largura, altura, valmax); //Crinado a imagem

        for (int a = 0; a < altura; a++)
        {
            for (int l = 0; l < largura; l++)
            {
                Pixel *p = &(imagem->pixels[(a * largura) + l]); //Criando cada píxel da imagem
                fscanf(arquivo, "%d", &(p->r));                  //Definindo o valor de vermelho cada píxel da imagem
                fscanf(arquivo, "%d", &(p->g));                  //Definindo o valor de verde cada píxel da imagem
                fscanf(arquivo, "%d", &(p->b));                  //Definindo o valor de azul cada píxel da imagem
            }
        }

        fclose(arquivo);
        return imagem;
    }
}

Imagem *aplicarFiltroCinza(Imagem *imagem)
{
    int cinza;
    for (int i = 0; i < imagem->altura * imagem->largura; i++)
    {
        cinza = ((imagem->pixels[i].r)*0.3)+((imagem->pixels[i].g)*0.59)+((imagem->pixels[i].b)*0.11); //Calcula escala de cinza
        imagem->pixels[i].r = cinza;
        imagem->pixels[i].g = cinza;
        imagem->pixels[i].b = cinza;
    }

    return imagem;
}

void criarArquivoPpm(char *nome_arquivo, Imagem *imagem)
{
    FILE *arquivo = fopen(nome_arquivo, "w");                                           //Abrindo arquivo em modo escrita
    fprintf(arquivo, "P3\n%d %d\n%d", imagem->altura,imagem->largura, imagem->valmax); //Escrevendo o cabeçalho da imagem no arquivo ppm
    for (int a = 0; a < imagem->altura; a++)
    {
        for (int l = 0; l < imagem->largura; l++)
        {
            Pixel *p = &(imagem->pixels[(a * imagem->largura) + l]); //Criando cada píxel da imagem
            fprintf(arquivo, "\n%d\n%d\n%d", p->r, p->g, p->b);        //Escrevendo os píxels no arquivo ppm
        }
    }
    fclose(arquivo);
}
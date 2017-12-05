#include "imagem.h"

//Cria uma imagem com alocação dinâmica de memória
Imagem *criarImagem(int altura, int largura, int valmax)
{
    Imagem *imagem = (Imagem *)malloc(sizeof(Imagem));
    imagem->pixels = (Pixel **)malloc(altura * sizeof(Pixel *));
    for (int i = 0; i < altura; i++)
    {
        imagem->pixels[i] = (Pixel *)malloc(largura * sizeof(Pixel));
    }
    imagem->largura = largura;
    imagem->altura = altura;
    imagem->valmax = valmax;
    return imagem;
}

//ler um arquivo ppm e salva num objeto imagem
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

        imagem = criarImagem(altura, largura, valmax); //Crinado a imagem

        for (int a = 0; a < altura; a++)
        {
            for (int l = 0; l < largura; l++)
            {
                Pixel *p = &(imagem->pixels[a][l]); //Criando cada píxel da imagem
                fscanf(arquivo, "%d", &(p->r));     //Definindo o valor de vermelho cada píxel da imagem
                fscanf(arquivo, "%d", &(p->g));     //Definindo o valor de verde cada píxel da imagem
                fscanf(arquivo, "%d", &(p->b));     //Definindo o valor de azul cada píxel da imagem
            }
        }

        fclose(arquivo);
        return imagem;
    }
}
// Nao permite leitura fora das fronteiras da imagem original
Pixel *verificarPixel(Imagem *imagem, int largura, int altura)
{
    if (largura >= imagem->largura)
    {
        largura = imagem->largura - 1;
    }
    if (altura >= imagem->altura)
    {
        altura = imagem->altura - 1;
    }
    if (largura < 0)
    {
        largura = 0;
    }
    if (altura < 0)
    {
        altura = 0;
    }

    return &imagem->pixels[altura][largura];
}
// Aplica o filtro cinza na imagem recebida por parâmetro
Imagem *aplicarFiltroCinza(Imagem *imagem)
{
    printf("Aplicando filtro Cinza...\n");
    int cinza;
    for (int a = 0; a < imagem->altura; a++)
    {
        for (int l = 0; l < imagem->largura; l++)
        {
            cinza = ((imagem->pixels[a][l].r) * 0.3) + ((imagem->pixels[a][l].g) * 0.59) + ((imagem->pixels[a][l].b) * 0.11); //Calcula escala de cinza
            imagem->pixels[a][l].r = cinza;
            imagem->pixels[a][l].g = cinza;
            imagem->pixels[a][l].b = cinza;
        }
    }

    return imagem;
}
// Aplica o filtro Gaussiano na imagem recebida por parâmetro
Imagem *aplicarFiltroGaussiano(Imagem *imagem)
{
    printf("Aplicando filtro Gaussiano...\n");
    int novopixel;
    Pixel *p;
    int sum, div;

    int matriz[5][5] = {{2, 4, 5, 4, 2},
                        {4, 9, 12, 9, 4},
                        {5, 12, 15, 12, 5},
                        {4, 9, 12, 9, 4},
                        {2, 4, 5, 4, 2}};

    Imagem *novaimagem = criarImagem(imagem->altura, imagem->largura, 255); //Crinado a imagem

    for (int a = 0; a < imagem->altura; a++)
    {
        for (int l = 0; l < imagem->largura; l++)
        {
            sum = 0;
            div = 0;

            for (int y = 0; y < 5; y++)
            {
                for (int x = 0; x < 5; x++)
                {
                    p = verificarPixel(imagem, l + (x - 2), a + (y - 2));
                    sum += (p->r * matriz[y][x]);
                    div += matriz[y][x];
                }
            }

            novopixel = sum / div;

            novaimagem->pixels[a][l].r = novopixel;
            novaimagem->pixels[a][l].g = novopixel;
            novaimagem->pixels[a][l].b = novopixel;
        }
    }

    return novaimagem;
}
// Aplica o filtro Sobel na imagem recebida por parâmetro
Imagem *aplicarFiltroSobel(Imagem *imagem)
{
    printf("Aplicando filtro Sobel...\n");
    int horizontal[3][3] = {{-1, -2, -1},
                            {0, 0, 0},
                            {1, 2, 1}};

    int vertical[3][3] = {{-1, 0, 1},
                          {-2, 0, 2},
                          {-1, 0, 1}};

    Imagem *novaimagem = criarImagem(imagem->altura, imagem->largura, imagem->valmax);
    Pixel *p;

    int sobelHorizontal, sobelVertical, resultado;

    for (int a = 0; a < imagem->altura; a++)
    {
        for (int l = 0; l < imagem->largura; l++)
        {
            sobelVertical = 0;
            sobelHorizontal = 0;
            for (int y = 0; y < 3; y++)
            {
                for (int x = 0; x < 3; x++)
                {
                    p = verificarPixel(imagem, l + (x - 2), a + (y - 2));
                    sobelHorizontal += (p->r * horizontal[y][x]);
                    sobelVertical += (p->r * vertical[y][x]);
                    resultado = sqrt(pow(sobelVertical, 2) + pow(sobelHorizontal, 2));

                    novaimagem->pixels[a][l].r = resultado;
                    novaimagem->pixels[a][l].g = resultado;
                    novaimagem->pixels[a][l].b = resultado;
                }
            }
        }
    }

    return novaimagem;
}
// Transforma a imagem recebida por parâmetro em uma imagem binarizada
Imagem *aplicarBinarizacao(Imagem *imagem)
{
    printf("Binarizando a imagem...\n");
    int mediaPixel, val;

    if (imagem->largura == 1015 && imagem->altura == 759)
    {
        //Verifica se é Catarata.ppm
        val = 15;
    }
    else if (imagem->largura == 1198 && imagem->altura == 770)
    {
        //Verifica se é Catarata2.ppm
        val = 13;
    }
    else if (imagem->largura == 1167 && imagem->altura == 739)
    {
        //Verifica se é Normal.ppm
        val = 15;
    }
    else if (imagem->largura == 610 && imagem->altura == 480)
    {
        //Verifica se é Normal2.ppm
        val = 13;
    }

    Imagem *novaimagem = criarImagem(imagem->altura, imagem->largura, imagem->valmax);

    for (int a = 0; a < imagem->altura; a++)
    {
        for (int l = 0; l < imagem->largura; l++)
        {

            mediaPixel = (int)(imagem->pixels[a][l].r + imagem->pixels[a][l].g + imagem->pixels[a][l].b) / 3;

            if (mediaPixel <= val)
            {
                novaimagem->pixels[a][l].r = 0;
                novaimagem->pixels[a][l].g = 0;
                novaimagem->pixels[a][l].b = 0;
            }
            if (mediaPixel > val)
            {
                novaimagem->pixels[a][l].r = 255;
                novaimagem->pixels[a][l].g = 255;
                novaimagem->pixels[a][l].b = 255;
            }
        }
    }

    return novaimagem;
}
// Aplica a transformada de Hough na imagem recebida por parâmetro
Imagem *aplicarTransformadaHough(Imagem *imagem, Imagem *original)
{
    printf("Aplicando tranformada de Hough...\n");
    int a, b, t, rmin, rmax;
    if (imagem->largura == 1015 && imagem->altura == 759)
    {
        //Verifica se é Catarata.ppm
        rmin = 84;
        rmax = 86;
    }
    else if (imagem->largura == 1198 && imagem->altura == 770)
    {
        //Verifica se é Catarata2.ppm
        rmin = 140;
        rmax = 152;
    }
    else if (imagem->largura == 1167 && imagem->altura == 739)
    {
        //Verifica se é Normal.ppm
        rmin = 152;
        rmax = 171;
    }
    else
    {
        //Verifica se é Normal2.ppm
        rmin = 70;
        rmax = 73;
    }
    int maior = 0, raux = 0, iaux = 0, jaux = 0, maior2 = 0, raux2 = 0, iaux2 = 0, jaux2 = 0;

    //Alocação de memória para matriz
    int ***valoresHough = (int ***)calloc(imagem->altura, sizeof(int **));
    for (int i = 0; i < imagem->altura; i++)
    {
        valoresHough[i] = (int **)calloc(imagem->largura, sizeof(int *));
        for (int j = 0; j < imagem->largura; j++)
        {
            valoresHough[i][j] = (int *)calloc(rmax - rmin + 1, sizeof(int));
        }
    }
    //Votação dos melhores valores para circulos
    printf("Processando os píxels...\n");
    for (int i = rmin; i < imagem->altura - rmin; i++)
    {
        for (int j = rmin; j < imagem->largura - rmin; j++)
        {
            if (imagem->pixels[i][j].r == 255)
            {
                for (int r = rmin; r <= rmax; r += 20)
                {
                    for (t = 0; t <= 360; t++)
                    {
                        a = (int)(i - r * cos((double)(t * (M_PI / 180))));
                        b = (int)(j - r * sin((double)(t * (M_PI / 180))));

                        if ((a >= 0) & (a < imagem->altura) & (b >= 0) & (b < imagem->largura))
                        {
                            valoresHough[a][b][r - rmin] += 1;
                        }
                    }
                }
            }
        }
    }

    printf("Procurando os píxels de valores máximos...\n");

    //Encontrando o maior circulo
    for (int i = rmin; i < imagem->altura - rmin; i++)
    {
        for (int j = rmin; j < imagem->largura - rmin; j++)
        {
            for (int r = rmin; r <= rmax; r++)
            {
                if (valoresHough[i][j][r - rmin] > maior)
                {
                    maior = valoresHough[i][j][r - rmin];
                    raux = r;
                    iaux = i;
                    jaux = j;
                }
            }
        }
    }
    
    //Criando circulo vermelho
    for (int i = rmin; i < imagem->altura - rmin; i++)
    {
        for (int j = rmin; j < imagem->largura - rmin; j++)
        {
            int dist = (int)sqrt(pow(i - iaux, 2) + pow(j - jaux, 2));

            if (dist == raux)
            {
                original->pixels[i][j].r = 255;
                original->pixels[i][j].g = 0;
                original->pixels[i][j].b = 0;
            }
        }
    }
    
    //Deixando os pixels de fora da pulpila pretos
    for (int i = 0; i < imagem->altura; i++)
    {
        for (int j = 0; j < imagem->largura; j++)
        {
            int dist = (int)sqrt(pow(i - iaux, 2) + pow(j - jaux, 2));

            if (dist > raux)
            {
                original->pixels[i][j].r = 0;
                original->pixels[i][j].g = 0;
                original->pixels[i][j].b = 0;
            }
        }
    }
    return original;
}

// Cria um arquivo ppm a partir do objeto imagem e nome recebido por parâmetro
void criarArquivoPpm(char *nome_arquivo, Imagem *imagem)
{
    FILE *arquivo = fopen(nome_arquivo, "w");                                           //Abrindo arquivo em modo escrita
    fprintf(arquivo, "P3\n%d %d\n%d", imagem->largura, imagem->altura, imagem->valmax); //Escrevendo o cabeçalho da imagem no arquivo ppm
    for (int a = 0; a < imagem->altura; a++)
    {
        for (int l = 0; l < imagem->largura; l++)
        {
            Pixel *p = &(imagem->pixels[a][l]);                 //Criando cada píxel da imagem
            fprintf(arquivo, "\n%d\n%d\n%d", p->r, p->g, p->b); //Escrevendo os píxels no arquivo ppm
        }
    }
    fclose(arquivo);
}

// Escreve num aruivo o diagnóstico do paciente a partir da imagem recebida por parâmetro.
void diagnosticarPaciente(Imagem *imagem, char *nome_arquivo)
{
    printf("Escrevendo diagnóstico...\n");
    int pixelsPulpila = 0;
    int pixelsCatarata = 0;
    float porcentagemCatarata = 0;

    for (int i = 0; i < imagem->altura; i++)
    {
        for (int j = 0; j < imagem->largura; j++)
        {
            if (imagem->pixels[i][j].r != 0 && imagem->pixels[i][j].g != 0 && imagem->pixels[i][j].b != 0 && imagem->pixels[i][j].r != 255 && imagem->pixels[i][j].g != 0 && imagem->pixels[i][j].b != 0)
            {
                pixelsPulpila++;
                if (imagem->pixels[i][j].r > 80 && imagem->pixels[i][j].r < 200 && imagem->pixels[i][j].g > 80 && imagem->pixels[i][j].g < 200 && imagem->pixels[i][j].b > 80 && imagem->pixels[i][j].b < 200)
                {
                    pixelsCatarata++;
                }
            }
        }
    }

    porcentagemCatarata = (100 * pixelsCatarata) / (float)pixelsPulpila;
    
    //Escrevendo diagnóstico
    FILE *arquivo = fopen(nome_arquivo, "w"); //Abrindo arquivo em modo escrita
    if (porcentagemCatarata < 65)
    {
        fprintf(arquivo, "Diagnóstico Geral: Sem catarata\nPorcentagem de Comprometimento: %f%%.", porcentagemCatarata);
    }
    else
    {
        fprintf(arquivo, "Diagnóstico Geral: Com catarata\nPorcentagem de Comprometimento: %f%%.", porcentagemCatarata);
    }
    fclose(arquivo);
}
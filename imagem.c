#include "imagem.h"

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

        printf("Lendo a imagem de %d linhas e  %d colunas...\n", largura, altura);

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

Pixel *verificarPixel(Imagem *imagem, int largura, int altura)
{
    // Nao permite leitura fora das fronteiras da imagem original

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

Imagem *aplicarBinarizacao(Imagem *imagem)
{
    printf("Binarizando a imagem...\n");
    int mediaPixel;

    Imagem *novaimagem = criarImagem(imagem->altura, imagem->largura, imagem->valmax);

    for (int a = 0; a < imagem->altura; a++)
    {
        for (int l = 0; l < imagem->largura; l++)
        {

            mediaPixel = (int)(imagem->pixels[a][l].r + imagem->pixels[a][l].g + imagem->pixels[a][l].b) / 3;

            if (mediaPixel <= 23)
            {
                novaimagem->pixels[a][l].r = 0;
                novaimagem->pixels[a][l].g = 0;
                novaimagem->pixels[a][l].b = 0;
            }
            if (mediaPixel > 23)
            {
                novaimagem->pixels[a][l].r = 255;
                novaimagem->pixels[a][l].g = 255;
                novaimagem->pixels[a][l].b = 255;
            }
        }
    }

    return novaimagem;
}

Imagem *aplicarTransformadaHough(Imagem *imagem, Imagem *original)
{
    printf("Aplicando tranformada de Hough...\n");
	int a, b, t;
    int rmin = 145, rmax = 291;
    int maior = 0, raux = 0, iaux = 0, jaux = 0, maior2 = 0, raux2 = 0, iaux2 = 0, jaux2 = 0;

	int ***valoresHough = (int***)calloc(imagem->altura, sizeof(int**));
	
	for(int i = 0; i < imagem->altura; i++){
		valoresHough[i] = (int**)calloc(imagem->largura, sizeof(int*));
		for(int j = 0; j < imagem->largura; j++){
			valoresHough[i][j] = (int*)calloc(rmax - rmin + 1, sizeof(int));
		}
	}
	printf("Processando os píxels...\n");
	for(int i = rmin; i < imagem->altura - rmin; i++){
		for(int j = rmin; j < imagem->largura - rmin; j++){
			if(imagem->pixels[i][j].r == 255){
				for(int r = rmin; r <= rmax; r++){
					for(t = 0; t <= 360; t++){
						a = (int)(i - r * cos((double)(t * ( M_PI/180))));
						b = (int)(j - r * sin((double)(t * ( M_PI/180))));
                        
						if((a >= 0) & (a < imagem->altura) & (b >= 0) & (b < imagem->largura)){
							valoresHough[a][b][r - rmin] += 1;
						}
					}
				}
			}
		}
	}

	printf("Procurando os píxels de valores máximos...\n");
	
    //Encontrando o maior circulo
	for(int i = rmin; i < imagem->altura - rmin; i++){
		for(int j = rmin; j < imagem->largura - rmin; j++){
			for(int r = rmin; r <= rmax; r++){
				if(valoresHough[i][j][r - rmin] > maior){
					maior = valoresHough[i][j][r - rmin];
					raux = r;
					iaux = i;
					jaux = j;
				}
			}
		}	
	}

    //Encontrando o segundo maior circulo
    for(int i = rmin; i < imagem->altura - rmin; i++){
		for(int j = rmin; j < imagem->largura - rmin; j++){
			for(int r = rmin; r <= rmax; r++){
				if((valoresHough[i][j][r - rmin] > maior2) & (valoresHough[i][j][r - rmin] < maior)){
					maior2 = valoresHough[i][j][r - rmin];
					raux2 = r;
					iaux2 = i;
					jaux2 = j;
				}
			}
		}	
	}
    
    printf("Criando primeiro círculo vermelho...\n");
	for (int i = rmin; i < imagem->altura - rmin; i++) {
		for (int j = rmin; j < imagem->largura - rmin; j++) { 
    		int dist = (int) sqrt(pow(i-iaux, 2) + pow(j-jaux,2));

    		if(dist == raux) {
        		original->pixels[i][j].r = 255;
        		original->pixels[i][j].g = 0;
        		original->pixels[i][j].b = 0;
      		}
    	}
    }

    printf("Criando segundo círculo vermelho...\n");
	for (int i = rmin; i < imagem->altura - rmin; i++) {
		for (int j = rmin; j < imagem->largura - rmin; j++) { 
    		int dist = (int) sqrt(pow(i-iaux2, 2) + pow(j-jaux2,2));

    		if(dist == raux2) {
        		original->pixels[i][j].r = 255;
        		original->pixels[i][j].g = 0;
        		original->pixels[i][j].b = 0;
      		}
    	}
    }
    printf("Finalizando...\n");
	return original;
}

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

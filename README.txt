- O que foi feito:

Modularização do programa
Leitura de imagens PPM
Transformação das imagens em tons de cinza
Filtro Gaussiano
Binarização
Transformada de Hough
Filtro de Sobel
Segmentação da pupila
Diagnóstico da catarata
Comentários no código

- O que não foi feito:

As imagens Normal2.ppm e Catarata2.ppm não estão sendo reconhecidas como deveriam

- O que seria feito diferentemente:
A organização do projeto poderia ter sido feita de outra forma utilizando
pastas para separar codigo fonte e arquivos
Os códigos poderiam ter sidos escritos em inglês e utilizando boas práticas de programação
Os comentários no código poderiam ter sido mais detalhados
Algumas funções poderiam ter sido feitas de forma a deixar o tempo de execução menor
O projeto poderia ter sido feito utilizando imagens P6

- Como compilar o projeto:

gcc -c imagem.c 
gcc -o catarata imagem.o catarata.c -lm
./catarata -i Normal2.ppm -f ppm -o diagnostico.txt
//David Varão Lima Bentes Pessoa  10402647
//João Victor Dallapé Madeira 10400725
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "lexico.h"
#include "sintatico.h"

//Compilar o arquivo:
//gcc -g -Og -Wall compilador.c -o compilador

//Executar o arquivo:
//./compilador entrada.txt

int main(int argc, char *argv[])
{
    if(le_arquivo(argv[1])) //Lê arquivo de entrada
        printf("Erro ao ler o arquivo\n"); //Detecta erro ao ler arquivo
    
    if(argc < 2) //Detecta se algum arquivo de entrada foi fornecido
    {
        printf("Erro: Nenhum arquivo de entrada foi fornecido.\n");
        return 1;
    }

    programa(); //Chama analisador sintático

    return 0;
}
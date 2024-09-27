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

/* TODO:
    1) Descobrir um modo de fazer com que o código ignore os comentários ✅
    2) Comentar o código ✅
    3) Elaborar um README.md
    4) Criar outros casos teste
    5) Implementar parâmetro de controle i nas funções com recursividade do analisador sintático ✅
*/

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
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "lexico.h"
#include "sintatico.h"

//Só existem tipo bool e int
//Função obter_atomo() --> analisador léxico
//Funcão consome() --> analisdor sintático
//Não existe else nem while (pelo que entendi)

//consome() possui obter_atomo() em seu interior (faz chamadas)

//O Compilador deve ler o arquivo fonte com o nome informado por linha de comando
// Ex: gcc -g -Og -Wall compilador.c -o compilador

//O compilador deve informar na tela do computador cada átomo reconhecido no arquivo ✅

//AO DETECTAR UM ERRO LÉXICO OU SINTÁTICO DEVE-SE PARAR O PROGRAMA E INFORMAR O ERRO E SEU TIPO ✅
//Para erro sintático: deve-se informar o átomo esperado e qual foi o átomo encontrado

//palavras-chaves (int, float...), operadores, identificadores, constantes,
//cadeias de caracteres (strings), e símbolos de pontuação como vírgula,
//ponto e vírgula, dois pontos, e ponto, etc.


//FUNÇÕES DO LÉXICO:
//1) Extração e classificação de átomos ✅
//2) Eliminação de delimitadores e comentários ✅
//3) Conversão numérica ✅
//4) Identificação de palavras reservadas ✅
//5) Tratamento de identificadores ✅
//6) Recuperação de erros ✅
//7) Interação com o sistema de arquivos
//8) Controle da numeração de linhas do programa fonte ✅

//Para cada erro léxico do código, colocar uma mensagem explicando o erro ✅

int main(int argc, char *argv[])
{
    if(le_arquivo(argv[1]))
        printf("Erro ao ler o arquivo\n"); //executar como: ./compilador entrada.txt
    
    if(argc < 2)
    {
        printf("Erro: Nenhum arquivo de entrada foi fornecido.\n");
        return 1;
    }

    programa(); // simbolo inicial da gramatica

    // Se precisar, Libera memória 
    //free(buffer);
    

    return 0;
}
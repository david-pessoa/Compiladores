#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "lexico.h"


char *msgAtomo[] = {
    "IDENTIFICADOR",
    "NUMERO",
    "ERRO",
    "EOF_BUFFER",
    "COMENTARIO",

    "+", //Operadores matemáticos
    "*",
    "-",
    "/",

    ">", //Operadores relacionais
    "<",
    ">=",
    "<=",
    "=",
    "/=",

    "AND", //Palavras reservadas da linguagem
    "BEGIN",
    "BOOLEAN",
    "ELIF",
    "END",
    "FALSE",
    "FOR",
    "IF",
    "INTEGER",
    "NOT",
    "OF",
    "OR",
    "PROGRAM",
    "READ",
    "SET",
    "TO",
    "TRUE",
    "WRITE",

    ",", //Sinais de "pontuação"
    ";",
    ".",
    ":",
    ")",
    "("
};

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

Atomo lookahead;//posteriormente sera do tipo TAtomo, declarado no ASDR
InfoAtomo info_atomo;

//############################### ANALISADOR SINTÁTICO ###############################

void consome(Atomo atomo){
    if(lookahead==atomo){
        info_atomo = obter_atomo();
        lookahead=info_atomo.atomo;
    }
    else{
        printf("#%d:Erro sintático:esperado [%s] encontrado [%s] \n",info_atomo.linha,msgAtomo[atomo],msgAtomo[lookahead]);
        exit(0); // Encerra execução
    }
}
/*
<comando> ::= <comando_atribuicao> |
<comando_condicional> |
<comando_repeticao> |
<comando_entrada> |
<comando_saida> |
<comando_composto>
*/
void comando()
{
    switch (lookahead)
    {
    case SET:
        comando_atribuicao(); // Falta Implementar
        break;
    
    case IF:
        comando_condicional(); // Falta Implementar
        break;
    
    case FOR:
        comando_repeticao(); // Falta Implementar
        break;
    
    case READ:
        comando_entrada(); // Falta Implementar
        break;
    
    case WRITE:
        comando_saida(); // Falta Implementar
        break;
    
    case BEGIN:
        comando_composto(); // Falta Implementar
        break;

    default:
        break;
    }
}

//<comando_composto> ::= begin <comando> {“;”<comando>} end
void comando_composto()
{
    switch (lookahead)
    {
    case BEGIN:
        consome(BEGIN);
        comando();
        comando_composto();
        break;
    
    case PONTO_VIRGULA:
        consome(PONTO_VIRGULA);
        comando();
        comando_composto();
        break;
    
    case END:
        consome(END);
        break;
        
    default:
        break;
    }
}

//<lista_variavel> ::= identificador { “,” identificador }
void lista_variavel()
{
    switch (lookahead)
    {
    case IDENTIFICADOR:
        consome(IDENTIFICADOR);
        lista_variavel();
        break;
    
    case VIRGULA:
        consome(VIRGULA);
        consome(IDENTIFICADOR);
        lista_variavel();
    
    default:
        break;
    }
}

//<declaracao_de_variaveis> ::= {<tipo> <lista_variavel> “;”}
void declaracao_de_variaveis()
{
    switch (lookahead)
    {
    case INTEGER:
        lista_variavel();
        consome(PONTO_VIRGULA);
        break;
    
    case BOOLEAN:
        lista_variavel();
        consome(PONTO_VIRGULA);
        break;
    
    default:
        break;
    }
}

//<bloco>::= <declaracao_de_variaveis> <comando_composto>
void bloco()
{
    declaracao_de_variaveis();
    comando_composto();
}

//<programa> ::= program identificador “;” <bloco> “.”
void programa()
{
    switch (lookahead)
    {
    case PROGRAM:
        consome(PROGRAM);
        consome(IDENTIFICADOR);
        consome(PONTO_VIRGULA);
        bloco();
        consome(PONTO);
        break;
    
    default:
        consome(COMENTARIO);
        programa();
        break;
    }
}



int main(int argc, char *argv[])
{
    if(le_arquivo(argv[1]))
        printf("Erro ao ler o arquivo\n"); //executar como: ./compilador entrada.txt
    
    if(argc < 2)
    {
        printf("Erro: Nenhum arquivo de entrada foi fornecido.\n");
        return 1;
    }

    info_atomo = obter_atomo();
    lookahead=info_atomo.atomo;

    programa(); // simbolo inicial da gramatica

    consome(EOF_BUFFER); // se lookahead chegou ao final

    printf("%d linhas analisadas, programa sintaticamente correto\n", info_atomo.linha);


    // Se precisar, Libera memória 
    //free(buffer);
    

    return 0;
}
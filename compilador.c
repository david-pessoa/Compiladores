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
<fator> ::= identificador | 
numero        | 
true          | 
false         | 
not <fator>   | 
“(“ <expressao> “)”
*/
void fator()
{
    switch (lookahead)
    {
    case IDENTIFICADOR:
        consome(IDENTIFICADOR);
        break;
    
    case NUMERO:
        consome(NUMERO);
        break;
    
    case TRUE:
        consome(TRUE);
        break;
    
    case NOT:
        fator();
        consome(ABRE_PARENTESES);
        expressao();
        consome(FECHA_PARENTESES);
        break;
    
    default:
        break;
    }
}

//<termo> ::= <fator> { ( “*” | “/” ) <fator> } 
void termo()
{
    if(lookahead == IDENTIFICADOR || lookahead == NUMERO || lookahead == TRUE || lookahead == FALSE || lookahead == NOT || lookahead == ABRE_PARENTESES)
    {
        fator();
        termo();
    }
    else if(lookahead == OP_MULT)
    {
        consome(OP_MULT);
        fator();
        termo();
    }
    else if(lookahead == OP_DIV)
    {
        consome(OP_DIV);
        fator();
        termo();
    }
}

// <expressao_simples> ::= <termo> { (“+” | “−” ) <termo> }
void expressao_simples()
{
    if(lookahead == IDENTIFICADOR || lookahead == NUMERO || lookahead == TRUE || lookahead == FALSE || lookahead == NOT || lookahead == ABRE_PARENTESES)
    {
        termo();
        expressao_simples();
    }
    else if(lookahead == OP_SOMA)
    {
        consome(OP_SOMA);
        termo();
        expressao_simples();
    }
    else if(lookahead == OP_SUB)
    {
        consome(OP_SUB);
        termo();
        expressao_simples();
    }
}

//<op_relacional> ::= “<” | “<=” | “=” | “/=” | “>” | “>=”
void op_relacional()
{
    switch (lookahead)
    {
    case OP_MENOR:
        consome(OP_MENOR);
        break;
    
    case OP_MENOR_IGUAL:
        consome(OP_MENOR_IGUAL);
        break;
    
    case OP_IGUAL:
        consome(OP_IGUAL);
        break;
    
    case OP_DIV_IGUAL:
        consome(OP_DIV_IGUAL);
        break;
    
    case OP_MAIOR:
        consome(OP_MAIOR);
        break;
    
    case OP_MAIOR_IGUAL:
        consome(OP_MAIOR_IGUAL);
        break;
    
    default:
        break;
    }
}

//<expressão_relacional> ::= <expressao_simples> [<op_relacional> <expressao_simples>]
void expressao_relacional()
{
    if(lookahead == IDENTIFICADOR || lookahead == NUMERO || lookahead == TRUE || lookahead == FALSE || lookahead == NOT || lookahead == ABRE_PARENTESES)
    {
        expressao_simples();
    }
    // else -> erro
    if(lookahead == OP_MENOR || lookahead == OP_MENOR_IGUAL || lookahead == OP_IGUAL || lookahead == OP_DIV_IGUAL || lookahead == OP_MAIOR || lookahead == OP_MAIOR_IGUAL)
    {
        op_relacional();
        expressao_simples();
    }
}

//<expressao_logica>::= <expressao_relacional> { and <expressao_relacional> }
void expressao_logica()
{
    if(lookahead == IDENTIFICADOR || lookahead == NUMERO || lookahead == TRUE || lookahead == FALSE || lookahead == NOT || lookahead == ABRE_PARENTESES)
    {
        expressao_relacional();
        expressao_logica();
    }
    else if(lookahead == AND)
    {
        consome(AND);
        expressao_relacional();
        expressao_logica();
    }
}

//<expressao> ::= <expressao_logica> { or <expressao_logica> }
void expressao()
{
    if(lookahead == IDENTIFICADOR || lookahead == NUMERO || lookahead == TRUE || lookahead == FALSE || lookahead == NOT || lookahead == ABRE_PARENTESES)
    {
        expressao_logica();
        expressao();
    }
    else if(lookahead == OR)
    {
        consome(OR);
        expressao_logica();
        expressao();
    }

}

//<comando_repeticao> ::= for identificador of <expressão> to <expressão> “:” <comando>
void comando_repeticao()
{
    consome(FOR);
    consome(IDENTIFICADOR);
    consome(TO);
    expressao();
    consome(TO);
    expressao();
    consome(DOIS_PONTOS);
    comando();
}

//<comando_entrada> ::= read “(“ <lista_variavel> “)”
void comando_entrada()
{
    consome(READ);
    consome(ABRE_PARENTESES);
    lista_variavel();
    consome(FECHA_PARENTESES);
}

//<comando_saida> ::= write “(“ <expressao> { “,” <expressao> } “)”
void comando_saida()
{
    switch (lookahead)
    {
    case WRITE:
        consome(WRITE);
        consome(ABRE_PARENTESES);
        expressao();
        comando_saida();
        break;
    
    case VIRGULA:
        consome(VIRGULA);
        expressao();
        comando_saida();
        break;
    
    case FECHA_PARENTESES:
        consome(FECHA_PARENTESES);
        break;
    
    default:
        break;
    }
}

//<comando_condicional> ::= if <expressao> “:” <comando> [elif <comando>]
void comando_condicional()
{
    consome(IF);
    expressao();
    consome(DOIS_PONTOS);
    comando();

    if(lookahead == ELIF)
    {
        consome(ELIF);
        comando();
    }
}

//<comando_atribuicao> ::= set identificador to <expressao>
void comando_atribuicao()
{
    consome(SET);
    consome(IDENTIFICADOR);
    consome(TO);
    expressao();
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
        comando_atribuicao(); //Implementado
        break;
    
    case IF:
        comando_condicional(); // Implementado
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
        declaracao_de_variaveis();
        break;
    
    case BOOLEAN:
        lista_variavel();
        consome(PONTO_VIRGULA);
        declaracao_de_variaveis();
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
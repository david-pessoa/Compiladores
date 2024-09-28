//David Varão Lima Bentes Pessoa  10402647
//João Pedro de Souza Costa Ferreira  10400720

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "lexico.h"

#ifndef LOOKAHEAD_H
#define LOOKAHEAD_H
Atomo lookahead = INICIA_SINTATICO; //Inicia lookahead
InfoAtomo info_atomo;
//############################### ANALISADOR SINTÁTICO ###############################

void consome(Atomo atomo){ //Ver como posso fazer para consumir comentários
    if(lookahead==atomo || lookahead == COMENTARIO){
        info_atomo = obter_atomo();
        lookahead=info_atomo.atomo;
    }
    else{
        printf("#%d:Erro sintático: esperado [%s] encontrado [%s] \n", info_atomo.linha, msgAtomo[atomo], msgAtomo[lookahead]);
        exit(0); // Encerra execução
    }
}

//Funções do analisador sintático:
void programa(); 
void bloco();
void declaracao_de_variaveis();
void lista_variavel(int i); 
void comando_composto(); 
void comando();
void comando_atribuicao();
void comando_condicional();
void comando_repeticao();
void comando_entrada();
void expressao(int i); // O i é usado para obter um controle, a fim de evitar que a função seja chamada e não seja ignorada em caso de erro sintático
void expressao_logica(int i);
void expressao_relacional();
void op_relacional();
void expressao_simples(int i);
void termo(int i);
void fator();

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
    if(lookahead == IDENTIFICADOR) //Se lê identificador, consome identificador
        consome(IDENTIFICADOR);

    else if(lookahead == NUMERO) //Se lê número, consome número
        consome(NUMERO);
    
    else if(lookahead == TRUE) //Se lê true, consome true
        consome(TRUE);
    
    else if(lookahead == FALSE) //Se lê false, consome false
        consome(FALSE);
    
    else if(lookahead == NOT) //Se lê not:
    {
        fator(); //Chama fator
        consome(ABRE_PARENTESES); //Consome '('
        expressao(0); //Chama expressão
        consome(FECHA_PARENTESES); //Consome ')'
    }

    else //Se lê qualquer outro átomo, dá erro sintático e informa que era esperado um identificador
        consome(IDENTIFICADOR); 
}

//<termo> ::= <fator> { ( “*” | “/” ) <fator> } 
void termo(int i)
{
    // Chama a primeira aparição de fator
    if(i == 0)
    {
        fator();
        termo(i + 1); //Chama termo() recursivamente para verificar se há átomos de '*' ou '/'
    }
    else if(lookahead == OP_MULT && i > 0) //Se o próximo átomo é '*'
    {
        consome(OP_MULT); //Consome o átomo '*'
        fator(); // Chama fator()
        termo(i + 1); //Chama termo() recursivamente para verificar se há átomos de '*' ou '/'
    }
    else if(lookahead == OP_DIV && i > 0) //Se o próximo átomo é '/'
    {
        consome(OP_DIV); //Consome o átomo '/'
        fator();  // Chama fator()
        termo(i + 1); //Chama termo() recursivamente para verificar se há átomos de '*' ou '/'
    }
}

// <expressao_simples> ::= <termo> { (“+” | “−” ) <termo> }
void expressao_simples(int i)
{
    // Chama a primeira aparição de termo
    if(i == 0)
    {
        termo(0); //Chama termo()
        expressao_simples(i + 1); //Chama expressao_simples() recursivamente para verificar se há átomos de '+' ou '-'
    }
    else if(lookahead == OP_SOMA && i > 0) //Se o próximo átomo é '+'
    {
        consome(OP_SOMA); //Consome o átomo '+'
        termo(0); //Chama termo()
        expressao_simples(i + 1); //Chama expressao_simples() recursivamente para verificar se há átomos de '+' ou '-'
    }
    else if(lookahead == OP_SUB && i > 0) //Se o próximo átomo é '-'
    {
        consome(OP_SUB); //Consome o átomo '-'
        termo(0); //Chama termo()
        expressao_simples(i + 1); //Chama expressao_simples() recursivamente para verificar se há átomos de '+' ou '-'
    }
}

//<op_relacional> ::= “<” | “<=” | “=” | “/=” | “>” | “>=”
void op_relacional()
{
    if(lookahead == OP_MENOR) //Consome átomo '<'
        consome(OP_MENOR);
    
    else if(lookahead == OP_MENOR_IGUAL) //Consome átomo '<='
        consome(OP_MENOR_IGUAL);
    
    else if(lookahead == OP_IGUAL) //Consome átomo '='
        consome(OP_IGUAL);
    
    else if(lookahead == OP_DIV_IGUAL) //Consome átomo '/='
        consome(OP_DIV_IGUAL);
    
    else if(lookahead == OP_MAIOR) //Consome átomo '>'
        consome(OP_MAIOR);
    
    else if(lookahead == OP_MAIOR_IGUAL) //Consome átomo '>='
        consome(OP_MAIOR_IGUAL);
    
    else //Se lê qualquer outro átomo, dá erro sintático e informa que era esperado um operador de igualdade
        consome(OP_IGUAL); 
}

//<expressão_relacional> ::= <expressao_simples> [<op_relacional> <expressao_simples>]
void expressao_relacional()
{
     expressao_simples(0); //Chama expressao_simples()
    
    //Se o próximo átomo é algum operador relacional
    if(lookahead == OP_MENOR || lookahead == OP_MENOR_IGUAL || lookahead == OP_IGUAL || lookahead == OP_DIV_IGUAL || lookahead == OP_MAIOR || lookahead == OP_MAIOR_IGUAL)
    {
        op_relacional(); //Chama op_relacional()
        expressao_simples(0); //Chama expressao_simples()
    }
}

//<expressao_logica>::= <expressao_relacional> { and <expressao_relacional> }
void expressao_logica(int i)
{
    // Chama a primeira aparição de expressão relacional
    if(i == 0)
    {
        expressao_relacional(); //Chama expressao_relacional()
        expressao_logica(i + 1); //Chama expressao_logica recursivamente() para verificar existência de outra expressão relacional
    }
    else if(lookahead == AND && i > 0) // Se o próximo átomo é and:
    {
        consome(AND); //Consome and
        expressao_relacional(); //Chama expressao_relacional()
        expressao_logica(i + 1); //Chama expressao_logica() recursivamente para verificar existência de outra expressão relacional
    }
}

//<expressao> ::= <expressao_logica> { or <expressao_logica> }
void expressao(int i)
{
    // Chama a primeira aparição de expressão lógica
    if(i == 0)
    {
        expressao_logica(0); //Chama expressao_logica()
        expressao(i + 1); //Chama expressao() recursivamente para verificar existência de outra expressão lógica
    }
    else if(lookahead == OR && i > 0) // Se o próximo átomo é or:
    {
        consome(OR); //Consome or
        expressao_logica(0); //Chama expressao_logica()
        expressao(i + 1); //Chama expressao() recursivamente para verificar existência de outra expressão lógica
    }
}

//<comando_repeticao> ::= for identificador of <expressão> to <expressão> “:” <comando>
void comando_repeticao() //Comando de laço for
{
    consome(FOR);
    consome(IDENTIFICADOR); //Consome átomos
    consome(TO);
    expressao(0); //Chama expressao() para ler a expressão de condição do for
    consome(TO);
    expressao(0); //Chama expressao() para ler a expressão de condição do for
    consome(DOIS_PONTOS);
    comando(); //Chama expressao() para ler o comando aninhado ao for

    while(lookahead == COMENTARIO)
        consome(COMENTARIO);
}

//<comando_entrada> ::= read “(“ <lista_variavel> “)”
void comando_entrada() //Comando para ler listas de variáveis
{
    consome(READ);
    consome(ABRE_PARENTESES);
    lista_variavel(0); // Chama lista_variavel() para ler a lista de variáveis
    consome(FECHA_PARENTESES);

    while(lookahead == COMENTARIO)
        consome(COMENTARIO);
}

//<comando_saida> ::= write “(“ <expressao> { “,” <expressao> } “)”
void comando_saida()
{
    if (lookahead == WRITE) // Se o próximo átomo é write
    {
        consome(WRITE); //Consome átomos write e '('
        consome(ABRE_PARENTESES);
        expressao(0); //Chama expressao()
        comando_saida(); // Chama comando_saida() recursivamente para verificar se o comando de saída já encerrou ou não
    }
    
    else if(lookahead == VIRGULA) // Se o próximo átomo é vírgula
    {
        consome(VIRGULA); //Consome vírgula
        expressao(0); //Chama expressao()
        comando_saida(); // Chama comando_saida() recursivamente para verificar se o comando de saída já encerrou ou não
    }
    
    else //Se encontra qualquer outro átomo consome ')' (se o átomo não for ')', gera-se erro)
        consome(FECHA_PARENTESES);
    
    while(lookahead == COMENTARIO)
        consome(COMENTARIO);
}

//<comando_condicional> ::= if <expressao> “:” <comando> [elif <comando>]
void comando_condicional()
{
    if(lookahead == IF) //Se o próximo átomo é um if:
    {
        consome(IF); //Consome if, consome condição e dois pontos
        expressao(0);
        consome(DOIS_PONTOS);
        comando(); // Chama comando()
    }
    while(lookahead == COMENTARIO)
        consome(COMENTARIO);

    if(lookahead == ELIF) //Caso o if seja seguido de um elif:
    {
        consome(ELIF); // Consome elif e chama comando()
        comando();
    }
    while(lookahead == COMENTARIO)
        consome(COMENTARIO);
}

//<comando_atribuicao> ::= set identificador to <expressao>
void comando_atribuicao()
{
    consome(SET); //Consome átomos e lê expressão
    consome(IDENTIFICADOR);
    consome(TO);
    expressao(0);

    while(lookahead == COMENTARIO)
        consome(COMENTARIO);
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
    if(lookahead == SET) //Se o próximo átomo a ser consumido é set, é porque se trata de um comando de atribuição
        comando_atribuicao();
    
    else if(lookahead == IF) //Se o próximo átomo a ser consumido é if, é porque se trata de um comando condicional
        comando_condicional(); 
    
    else if(lookahead == FOR) //Se o próximo átomo a ser consumido é for, é porque se trata de um comando de repetição
        comando_repeticao(); 
    
    else if(lookahead == READ) //Se o próximo átomo a ser consumido é read, é porque se trata de um comando de entrada
        comando_entrada(); 
    
    else if(lookahead == WRITE) //Se o próximo átomo a ser consumido é write, é porque se trata de um comando de saída
        comando_saida();
    
    else if(lookahead == BEGIN) //Se o próximo átomo a ser consumido é begin, é porque se trata de um comando composto
        comando_composto(); 
    
    else if(lookahead == COMENTARIO)
    {
        consome(COMENTARIO);
        comando();
    }
    else //Se lê qualquer outro átomo, dá erro sintático e informa que era esperado o átomo begin
        consome(BEGIN); 
}

//<comando_composto> ::= begin <comando> {“;”<comando>} end
void comando_composto()
{
    if(lookahead == BEGIN) //Se o próximo átomo é begin:
    {
        consome(BEGIN); //Consome begin e chama comando() para ler o comando aninhado
        comando();
        comando_composto(); //Chama comando_composto() recursivamente para verificar se o comando composto acabou ou não
    }

    else if(lookahead == COMENTARIO)
    {
        consome(COMENTARIO);
        comando_composto();
    }
    else if(lookahead == PONTO_VIRGULA) //se o próximo átomo é ';', é porque ainda há mais um comando
    {
        consome(PONTO_VIRGULA); //Consome ';' e chama comando()
        comando();
        comando_composto(); //Chama comando_composto() recursivamente para verificar se o comando composto acabou ou não
    }
    
    else //Se qualquer outro átomo é lido, tenta consumir end (caso o átomo não seja end, gera-se erro)
        consome(END);

}

//<lista_variavel> ::= identificador { “,” identificador }
void lista_variavel(int i)
{
    if(i == 0)
    {
        consome(IDENTIFICADOR);
        lista_variavel(i + 1); //Chama lista_variavel() recursivamente para verificar se a lista de variáveis acabou ou não
    }
    
    else if(lookahead == VIRGULA && i > 0) //Se o próximo átomo é ',': consome a ',' e o identificador seguinte
    {
        consome(VIRGULA);
        consome(IDENTIFICADOR);
        lista_variavel(i + 1); //Chama lista_variavel() recursivamente para verificar se a lista de variáveis acabou ou não
    }

    while(lookahead == COMENTARIO)
        consome(COMENTARIO);

}

//<declaracao_de_variaveis> ::= {<tipo> <lista_variavel> “;”}
void declaracao_de_variaveis()
{
    switch (lookahead)
    {
    case INTEGER: //Se o próximo átomo é integer, então a lista contém variaveis do tipo integer
        consome(INTEGER); //consome integer
        lista_variavel(0); //Lê lista de variáveis
        consome(PONTO_VIRGULA); //Consome ';'
        declaracao_de_variaveis(); //Chama declaracao_de_variaveis() recursivamente para verificar se há mais declarações de variáveis a serem feitas
        break;
    
    case BOOLEAN: //Se o próximo átomo é boolean, então a lista contém variaveis do tipo boolean
        consome(BOOLEAN); //consome boolean
        lista_variavel(0); //Lê lista de variáveis
        consome(PONTO_VIRGULA); //Consome ';'
        declaracao_de_variaveis(); //Chama declaracao_de_variaveis() recursivamente para verificar se há mais declarações de variáveis a serem feitas
        break;

    default:
        break;
    }

    while(lookahead == COMENTARIO)
        consome(COMENTARIO);
}

//<bloco>::= <declaracao_de_variaveis> <comando_composto>
void bloco()
{
    while(lookahead == COMENTARIO)
        consome(COMENTARIO);

    declaracao_de_variaveis();

    while(lookahead == COMENTARIO)
        consome(COMENTARIO);

    comando_composto();

    while(lookahead == COMENTARIO)
        consome(COMENTARIO);
}

//<programa> ::= program identificador “;” <bloco> “.”
void programa()
{
    consome(INICIA_SINTATICO); //Obtém átomo inicial

    while(lookahead == COMENTARIO)
        consome(COMENTARIO);

        consome(PROGRAM); //inicia a leitura do programa consumindo os próximos átomos
        consome(IDENTIFICADOR);
        consome(PONTO_VIRGULA);

        while(lookahead == COMENTARIO)
            consome(COMENTARIO);

        bloco(); // Chama o bloco
        consome(PONTO); //Quando chega ao ponto final, consome ponto e o átomo EOF_BUFFER

        while(lookahead == COMENTARIO)
            consome(COMENTARIO);

        consome(EOF_BUFFER);
        printf("%d linhas analisadas, programa sintaticamente correto\n", info_atomo.linha); //Encerra análise sintática corretamente

}
#endif
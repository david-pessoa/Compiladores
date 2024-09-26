#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "lexico.h"

#ifndef LOOKAHEAD_H
#define LOOKAHEAD_H
Atomo lookahead;//posteriormente sera do tipo TAtomo, declarado no ASDR
InfoAtomo info_atomo;
//############################### ANALISADOR SINTÁTICO ###############################

void consome(Atomo atomo){ //Ver como posso fazer para consumir comentários
    if(lookahead==atomo){
        info_atomo = obter_atomo();
        lookahead=info_atomo.atomo;
    }
    else{
        printf("#%d:Erro sintático:esperado [%s] encontrado [%s] \n",info_atomo.linha,msgAtomo[atomo],msgAtomo[lookahead]);
        exit(0); // Encerra execução
    }
}

// TODO: Criar uma função para encontrar o erro gerado quando o código cai no else ou default

//Funções do analisador sintático:
void programa();
void bloco();
void declaracao_de_variaveis();
void lista_variavel();
void comando_composto();
void comando();
void comando_atribuicao();
void comando_condicional();
void comando_repeticao();
void comando_entrada();
void expressao();
void expressao_logica();
void expressao_relacional();
void op_relacional();
void expressao_simples();
void termo();
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
        expressao(); //Chama expressão
        consome(FECHA_PARENTESES); //Consome ')'
    }

    //else ERRO
}

//<termo> ::= <fator> { ( “*” | “/” ) <fator> } 
void termo()
{
    //Se o próximo átomo a ser lido é um: identificador, número, true, false, not ou abre parênteses, quer dizer que deve-se chamar a função fator()
    if(lookahead == IDENTIFICADOR || lookahead == NUMERO || lookahead == TRUE || lookahead == FALSE || lookahead == NOT || lookahead == ABRE_PARENTESES)
    {
        fator();
        termo(); //Chama termo() recursivamente para verificar se há átomos de '*' ou '/'
    }
    else if(lookahead == OP_MULT) //Se o próximo átomo é '*'
    {
        consome(OP_MULT); //Consome o átomo '*'
        fator(); // Chama fator()
        termo(); //Chama termo() recursivamente para verificar se há átomos de '*' ou '/'
    }
    else if(lookahead == OP_DIV) //Se o próximo átomo é '/'
    {
        consome(OP_DIV); //Consome o átomo '/'
        fator();  // Chama fator()
        termo(); //Chama termo() recursivamente para verificar se há átomos de '*' ou '/'
    }

    //else ERRO
}

// <expressao_simples> ::= <termo> { (“+” | “−” ) <termo> }
void expressao_simples()
{
    //Se o próximo átomo a ser lido é um: identificador, número, true, false, not ou abre parênteses, quer dizer que deve-se chamar a função termo()
    if(lookahead == IDENTIFICADOR || lookahead == NUMERO || lookahead == TRUE || lookahead == FALSE || lookahead == NOT || lookahead == ABRE_PARENTESES)
    {
        termo(); //Chama termo()
        expressao_simples(); //Chama expressao_simples() recursivamente para verificar se há átomos de '+' ou '-'
    }
    else if(lookahead == OP_SOMA) //Se o próximo átomo é '+'
    {
        consome(OP_SOMA); //Consome o átomo '+'
        termo(); //Chama termo()
        expressao_simples(); //Chama expressao_simples() recursivamente para verificar se há átomos de '+' ou '-'
    }
    else if(lookahead == OP_SUB) //Se o próximo átomo é '-'
    {
        consome(OP_SUB); //Consome o átomo '-'
        termo(); //Chama termo()
        expressao_simples(); //Chama expressao_simples() recursivamente para verificar se há átomos de '+' ou '-'
    }

    //else ERRO
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
    
    //else ERRO
}

//<expressão_relacional> ::= <expressao_simples> [<op_relacional> <expressao_simples>]
void expressao_relacional()
{
     expressao_simples(); //Chama expressao_simples()
    
    //Se o próximo átomo é algum operador relacional
    if(lookahead == OP_MENOR || lookahead == OP_MENOR_IGUAL || lookahead == OP_IGUAL || lookahead == OP_DIV_IGUAL || lookahead == OP_MAIOR || lookahead == OP_MAIOR_IGUAL)
    {
        op_relacional(); //Chama op_relacional()
        expressao_simples(); //Chama expressao_simples()
    }
}

//<expressao_logica>::= <expressao_relacional> { and <expressao_relacional> }
void expressao_logica()
{
    //Se o próximo átomo a ser lido é um: identificador, número, true, false, not ou abre parênteses, quer dizer que deve-se chamar a função expressao_relacional()
    if(lookahead == IDENTIFICADOR || lookahead == NUMERO || lookahead == TRUE || lookahead == FALSE || lookahead == NOT || lookahead == ABRE_PARENTESES)
    {
        expressao_relacional(); //Chama expressao_relacional()
        expressao_logica(); //Chama expressao_logica recursivamente() para verificar existência de outra expressão relacional
    }
    else if(lookahead == AND) // Se o próximo átomo é and:
    {
        consome(AND); //Consome and
        expressao_relacional(); //Chama expressao_relacional()
        expressao_logica(); //Chama expressao_logica() recursivamente para verificar existência de outra expressão relacional
    }

    //else ERRO
}

//<expressao> ::= <expressao_logica> { or <expressao_logica> }
void expressao()
{
    //Se o próximo átomo a ser lido é um: identificador, número, true, false, not ou abre parênteses, quer dizer que deve-se chamar a função expressao_logica()
    if(lookahead == IDENTIFICADOR || lookahead == NUMERO || lookahead == TRUE || lookahead == FALSE || lookahead == NOT || lookahead == ABRE_PARENTESES)
    {
        expressao_logica(); //Chama expressao_logica()
        expressao(); //Chama expressao() recursivamente para verificar existência de outra expressão lógica
    }
    else if(lookahead == OR) // Se o próximo átomo é or:
    {
        consome(OR); //Consome or
        expressao_logica(); //Chama expressao_logica()
        expressao(); //Chama expressao() recursivamente para verificar existência de outra expressão lógica
    }
    // else ERRO
}

//<comando_repeticao> ::= for identificador of <expressão> to <expressão> “:” <comando>
void comando_repeticao() //Comando de laço for
{
    consome(FOR);
    consome(IDENTIFICADOR); //Consome átomos
    consome(TO);
    expressao(); //Chama expressao() para ler a expressão de condição do for
    consome(TO);
    expressao(); //Chama expressao() para ler a expressão de condição do for
    consome(DOIS_PONTOS);
    comando(); //Chama expressao() para ler o comando aninhado ao for
}

//<comando_entrada> ::= read “(“ <lista_variavel> “)”
void comando_entrada() //Comando para ler listas de variáveis
{
    consome(READ);
    consome(ABRE_PARENTESES);
    lista_variavel(); // Chama lista_variavel() para ler a lista de variáveis
    consome(FECHA_PARENTESES);
}

//<comando_saida> ::= write “(“ <expressao> { “,” <expressao> } “)”
void comando_saida()
{
    if (lookahead == WRITE) // Se o próximo átomo é write
    {
        consome(WRITE); //Consome átomos write e '('
        consome(ABRE_PARENTESES);
        expressao(); //Chama expressao()
        comando_saida(); // Chama comando_saida() recursivamente para verificar se o comando de saída já encerrou ou não
    }
    
    else if(lookahead == VIRGULA) // Se o próximo átomo é vírgula
    {
        consome(VIRGULA); //Consome vírgula
        expressao(); //Chama expressao()
        comando_saida(); // Chama comando_saida() recursivamente para verificar se o comando de saída já encerrou ou não
    }
    
    else //Se encontra qualquer outro átomo consome ')' (se o átomo não for ')', gera-se erro)
        consome(FECHA_PARENTESES);

    
}

//<comando_condicional> ::= if <expressao> “:” <comando> [elif <comando>]
void comando_condicional()
{
    if(lookahead == IF) //Se o próximo átomo é um if:
    {
        consome(IF); //Consome if, consome condição e dois pontos
        expressao();
        consome(DOIS_PONTOS);
        comando(); // Chama comando()
    }
    if(lookahead == ELIF) //Caso o if seja seguido de um elif:
    {
        consome(ELIF); // Consome elif e chama comando()
        comando();
    }
}

//<comando_atribuicao> ::= set identificador to <expressao>
void comando_atribuicao()
{
    consome(SET); //Consome átomos e lê expressão
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
    
    //else ERRO
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
void lista_variavel()
{
    switch (lookahead)
    {
    case IDENTIFICADOR: //Se o próximo átomo é um identificador: consome identificador
        consome(IDENTIFICADOR);
        lista_variavel(); //Chama lista_variavel() recursivamente para verificar se a lista de variáveis acabou ou não
        break;
    
    case VIRGULA: //Se o próximo átomo é ',': consome a ',' e o identificador seguinte
        consome(VIRGULA);
        consome(IDENTIFICADOR);
        lista_variavel(); //Chama lista_variavel() recursivamente para verificar se a lista de variáveis acabou ou não
    
    default:
    //else ERRO
        break;
    }
}

//<declaracao_de_variaveis> ::= {<tipo> <lista_variavel> “;”}
void declaracao_de_variaveis()
{
    switch (lookahead)
    {
    case INTEGER: //Se o próximo átomo é integer, então a lista contém variaveis do tipo integer
        consome(INTEGER); //consome integer
        lista_variavel(); //Lê lista de variáveis
        consome(PONTO_VIRGULA); //Consome ';'
        declaracao_de_variaveis(); //Chama declaracao_de_variaveis() recursivamente para verificar se há mais declarações de variáveis a serem feitas
        break;
    
    case BOOLEAN: //Se o próximo átomo é boolean, então a lista contém variaveis do tipo boolean
        consome(INTEGER); //consome boolean
        lista_variavel(); //Lê lista de variáveis
        consome(PONTO_VIRGULA); //Consome ';'
        declaracao_de_variaveis(); //Chama declaracao_de_variaveis() recursivamente para verificar se há mais declarações de variáveis a serem feitas
        break;
    
    default:
        //Não precisa do else ERRO
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
    info_atomo = obter_atomo(); //Obtém átomo inicial
    lookahead = info_atomo.atomo;

    switch (lookahead)
    {
    case PROGRAM: //Se o átomo inicial for PROGRAM
        consome(PROGRAM); //inicia a leitura do programa consumindo os próximos átomos
        consome(IDENTIFICADOR);
        consome(PONTO_VIRGULA);
        bloco(); // Chama o bloco
        consome(PONTO); //Quando chega ao ponto final, consome ponto e o átomo EOF_BUFFER
        consome(EOF_BUFFER);
        printf("%d linhas analisadas, programa sintaticamente correto\n", info_atomo.linha); //Encerra análise sintática corretamente
        break;

    default: //Arrumar isto (para identificar átomos de comentário)
        //else ERRO
        break;
    }

}
#endif
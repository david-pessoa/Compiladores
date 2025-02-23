//David Varão Lima Bentes Pessoa  10402647
//João Victor Dallapé Madeira 10400725

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "lexico.h"
#include "semantico.h"
#include "atomo.h"

#ifndef LOOKAHEAD_H
#define LOOKAHEAD_H

Atomo lookahead = INICIA_SINTATICO; //Inicia lookahead
InfoAtomo info_atomo;

//############################### ANALISADOR SINTÁTICO ###############################
void consome(Atomo atomo){ //Função consome() verifica se o átomo esperado é o átomo obtido
    if(lookahead==atomo){
        info_atomo = obter_atomo(); //Caso o átomo esperado é o átomo obtido, obtém próximo átomo
        lookahead=info_atomo.atomo;
    }
    else{ //Senão, exibe erro sintático mostrando o átomo esperado e o obtido
        printf("#%d:Erro sintático: esperado [%s] encontrado [%s] \n", info_atomo.linha, msgAtomo[atomo], msgAtomo[lookahead]);
        exit(0); // Encerra execução
    }
}

//Funções do analisador sintático:
void programa(); 
void bloco();
void declaracao_de_variaveis();
void lista_variavel(int i, bool checa); 
void comando_composto(int i); 
void comando();
void comando_atribuicao();
void comando_condicional();
void comando_repeticao();
void comando_entrada();
void comando_saida(int i);
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
    {    
        int endereco = verifica_semantica_outros(info_atomo);
        printf("CRVL %d\n", endereco); //Imprime o endereço da variável que está sendo carregada
        consome(IDENTIFICADOR);
    }

    else if(lookahead == NUMERO) //Se lê número, consome número
    {   printf("CRCT %d\n", info_atomo.numero); //Imprime o valor do número carregado
        consome(NUMERO);
    }
    
    else if(lookahead == TRUE) //Se lê true, consome true
        consome(TRUE);
    
    else if(lookahead == FALSE) //Se lê false, consome false
        consome(FALSE);
    
    else if(lookahead == NOT) //Se lê not:
    {
        consome(NOT);
        fator(); //Chama fator
    }

    else if(lookahead == ABRE_PARENTESES) //Se lê abre parênteses
    {
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
        printf("MULT\n");
        termo(i + 1); //Chama termo() recursivamente para verificar se há átomos de '*' ou '/'
    }
    else if(lookahead == OP_DIV && i > 0) //Se o próximo átomo é '/'
    {
        consome(OP_DIV); //Consome o átomo '/'
        fator();  // Chama fator()
        printf("DIVI\n");
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
        printf("SOMA\n");
        expressao_simples(i + 1); //Chama expressao_simples() recursivamente para verificar se há átomos de '+' ou '-'
    }
    else if(lookahead == OP_SUB && i > 0) //Se o próximo átomo é '-'
    {
        consome(OP_SUB); //Consome o átomo '-'
        termo(0); //Chama termo()
        printf("SUBT\n");
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
        Atomo operador = lookahead;
        op_relacional(); //Chama op_relacional()
        expressao_simples(0); //Chama expressao_simples()

        //Indica que será feita a operação de...
        if(operador == OP_MENOR) 
            printf("CMME\n"); // menor (<)
        else if(operador == OP_MENOR_IGUAL) //menor ou igual (<=)
            printf("CMEG\n");
        else if(operador == OP_MAIOR_IGUAL) //maior ou igual (>=)
            printf("CMAG\n");
        else if(operador == OP_IGUAL) //igual (=)
            printf("CMIG\n");
        else if(operador == OP_DIV_IGUAL) // (/=)
            printf("CMDG\n");
        else if(operador == OP_MAIOR) //maior (>)
            printf("CMMA\n");
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
    int endereco = verifica_semantica_outros(info_atomo); //Verifica se a variável já foi declarada

    consome(IDENTIFICADOR); //Consome variável que será usado dentro do for (i)
    consome(OF);
    expressao(0); //Chama expressao() para retornar o valor de início da iteração
    printf("ARMZ %d\n", endereco); //armazena o valor de início na variável i
    consome(TO);

    int L1 = proximo_rotulo();
    int L2 = proximo_rotulo();
    printf("L%d: NADA\n", L1); //Inicia loop
    printf("CRVL %d\n", endereco); //Carrega valor da variável i
    expressao(0); //Carrega valor de parada do for

    consome(DOIS_PONTOS);
    
    printf("CMEG\n"); //Compara se a variável i é menor ou igual ao valor de parada
    printf("DSVF L%d\n", L2); //Se não for, desvia

    comando(); //Código aninhado ao for

    printf("CRVL %d\n", endereco); //Fim da iteração, incrementa i em um e salva novo valor
    printf("CRCT 1\n");
    printf("SOMA\n");
    printf("ARMZ %d\n", endereco);
    printf("DSVS L%d\n", L1); //Volta ao início
    printf("L%d: NADA\n", L2); //Sai do loop

    while(lookahead == COMENTARIO) //Verifica se há comentários
        consome(COMENTARIO); //Consome comentário
}

//<comando_entrada> ::= read “(“ <lista_variavel> “)”
void comando_entrada() //Comando para ler listas de variáveis
{
    consome(READ);
    consome(ABRE_PARENTESES);
    lista_variavel(0, false); // Chama lista_variavel() para ler a lista de variáveis
    consome(FECHA_PARENTESES);

    while(lookahead == COMENTARIO) //Verifica se há comentários
        consome(COMENTARIO); //Consome comentário
}

//<comando_saida> ::= write “(“ <expressao> { “,” <expressao> } “)”
void comando_saida(int i)
{
    if (i == 0) // Se o próximo átomo é write
    {
        consome(WRITE); //Consome átomos write e '('
        consome(ABRE_PARENTESES);
        expressao(0); //Carrega valor a ser impresso
        printf("IMPR\n");
        comando_saida(i + 1); // Chama comando_saida() recursivamente para verificar se o comando de saída já encerrou ou não
    }
    
    else if(lookahead == VIRGULA && i > 0) // Se o próximo átomo é vírgula
    {
        consome(VIRGULA); //Consome vírgula
        expressao(0); //Carrega valor a ser impresso
        printf("IMPR\n");
        comando_saida(i + 1); // Chama comando_saida() recursivamente para verificar se o comando de saída já encerrou ou não
    }
    
    else //Se encontra qualquer outro átomo consome ')' (se o átomo não for ')', gera-se erro)
        consome(FECHA_PARENTESES);
    
    while(lookahead == COMENTARIO) //Verifica se há comentários
        consome(COMENTARIO); //Consome comentário
}

//<comando_condicional> ::= if <expressao> “:” <comando> [elif <comando>]
void comando_condicional()
{
    int L1 = proximo_rotulo();
    if(lookahead == IF) //Se o próximo átomo é um if:
    {
        consome(IF); //Consome if, consome condição e dois pontos
        expressao(0); //Mostra condição do loop
        printf("DSVF L%d\n", L1); //Se condição for falsa, não executa o if
        consome(DOIS_PONTOS);
        comando(); // Chama comando()
    }
    while(lookahead == COMENTARIO) //Verifica se há comentários
        consome(COMENTARIO); //Consome comentário

    if(lookahead == ELIF) //Caso o if seja seguido de um elif:
    {
        int L2 = proximo_rotulo();
        printf("DSVS L%d\n", L2); //Adiciona fim ao bloco if, para não invadir o elif
        printf("L%d:  NADA\n", L1); //Inicia elif
        consome(ELIF); // Consome elif e chama comando()
        comando();
        printf("L%d:  NADA\n", L2); // Sai do elif
    }
    else
        printf("L%d:  NADA\n", L1); //Caso não haja um ELIF

    while(lookahead == COMENTARIO) //Verifica se há comentários
        consome(COMENTARIO); //Consome comentário
}

//<comando_atribuicao> ::= set identificador to <expressao>
void comando_atribuicao()
{
    consome(SET); //Consome átomos e lê expressão
    int endereco = verifica_semantica_outros(info_atomo);
    consome(IDENTIFICADOR);
    consome(TO);
    expressao(0); //Carrega valor a ser atribuído a variável
    printf("ARMZ %d\n", endereco);

    while(lookahead == COMENTARIO) //Verifica se há comentários
        consome(COMENTARIO); //Consome comentário
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
        comando_saida(0);
    
    else if(lookahead == BEGIN) //Se o próximo átomo a ser consumido é begin, é porque se trata de um comando composto
        comando_composto(0); 
    
    else if(lookahead == COMENTARIO)
    {
        consome(COMENTARIO);
        comando();
    }
    else //Se lê qualquer outro átomo, dá erro sintático e informa que era esperado o átomo begin
        consome(BEGIN); 
}

//<comando_composto> ::= begin <comando> {“;”<comando>} end
void comando_composto(int i)
{
    if(i == 0) //Se o próximo átomo é begin:
    {
        consome(BEGIN); //Consome begin e chama comando() para ler o comando aninhado
        comando();
        comando_composto(i + 1); //Chama comando_composto() recursivamente para verificar se o comando composto acabou ou não
    }
    else if(lookahead == PONTO_VIRGULA && i > 0) //se o próximo átomo é ';', é porque ainda há mais um comando
    {
        consome(PONTO_VIRGULA); //Consome ';' e chama comando()
        comando();
        comando_composto(i + 1); //Chama comando_composto() recursivamente para verificar se o comando composto acabou ou não
    }
    
    else //Se qualquer outro átomo é lido, tenta consumir end (caso o átomo não seja end, gera-se erro)
        consome(END);

}

//<lista_variavel> ::= identificador { “,” identificador }
void lista_variavel(int i, bool checa) //checa == true se está em <declaracao_de_variaveis>, checa == false caso contrário
{
    if(i == 0)
    {
        verifica_semantica(checa, info_atomo); //Realiza análise semântica
        consome(IDENTIFICADOR);
        lista_variavel(i + 1, checa); //Chama lista_variavel() recursivamente para verificar se a lista de variáveis acabou ou não
    }
    
    else if(lookahead == VIRGULA && i > 0) //Se o próximo átomo é ',': consome a ',' e o identificador seguinte
    {
        consome(VIRGULA);
        verifica_semantica(checa, info_atomo); //Realiza análise semântica
        consome(IDENTIFICADOR);
        lista_variavel(i + 1, checa); //Chama lista_variavel() recursivamente para verificar se a lista de variáveis acabou ou não
    }

    while(lookahead == COMENTARIO) //Verifica se há comentários
        consome(COMENTARIO); //Consome comentário

}

//<declaracao_de_variaveis> ::= {<tipo> <lista_variavel> “;”}
void declaracao_de_variaveis()
{
    while(lookahead == COMENTARIO) //Verifica se há comentários
        consome(COMENTARIO); //Consome comentário

    switch (lookahead)
    {
    case INTEGER: //Se o próximo átomo é integer, então a lista contém variaveis do tipo integer
        consome(INTEGER); //consome integer
        lista_variavel(0, true); //Lê lista de variáveis
        consome(PONTO_VIRGULA); //Consome ';'
        declaracao_de_variaveis(); //Chama declaracao_de_variaveis() recursivamente para verificar se há mais declarações de variáveis a serem feitas
        break;
    
    case BOOLEAN: //Se o próximo átomo é boolean, então a lista contém variaveis do tipo boolean
        consome(BOOLEAN); //consome boolean
        lista_variavel(0, true); //Lê lista de variáveis
        consome(PONTO_VIRGULA); //Consome ';'
        declaracao_de_variaveis(); //Chama declaracao_de_variaveis() recursivamente para verificar se há mais declarações de variáveis a serem feitas
        break;

    default:
        break;
    }

}

//<bloco>::= <declaracao_de_variaveis> <comando_composto>
void bloco()
{
    while(lookahead == COMENTARIO) //Verifica se há comentários
        consome(COMENTARIO); //Consome comentário

    declaracao_de_variaveis();
    printf("AMEM %d\n", get_tabela_size()); //Aloca memória proporcional a quantidade de variáveis a serem usadas no programa

    while(lookahead == COMENTARIO) //Verifica se há comentários
        consome(COMENTARIO); //Consome comentário

    comando_composto(0);

    while(lookahead == COMENTARIO) //Verifica se há comentários
        consome(COMENTARIO); //Consome comentário
}

//<programa> ::= program identificador “;” <bloco> “.”
void programa()
{
    consome(INICIA_SINTATICO); //Obtém átomo inicial
    inicia_tabela(); //Cria a tabela de símbolos
    printf("INPP\n"); //Inicia programa

    while(lookahead == COMENTARIO) //Verifica se há comentários
        consome(COMENTARIO); //Consome comentário

        consome(PROGRAM); //inicia a leitura do programa consumindo os próximos átomos
        consome(IDENTIFICADOR);
        consome(PONTO_VIRGULA);

        while(lookahead == COMENTARIO) //Verifica se há comentários
            consome(COMENTARIO); //Consome comentário

        bloco(); // Chama o bloco
        consome(PONTO); //Quando chega ao ponto final, consome ponto

        while(lookahead == COMENTARIO) //Verifica se há comentários
            consome(COMENTARIO); //Consome comentário

    printf("PARA\n"); //Exibe encerramento do programa
    show_tabela(); //Exibe tabela de símbolos
}
#endif
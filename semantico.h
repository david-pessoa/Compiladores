//David Varão Lima Bentes Pessoa  10402647
//João Victor Dallapé Madeira 10400725

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "atomo.h"

#ifndef SEMANTICO_H
#define SEMANTICO_H

#define MAX 20 //Máximo de variáveis suportado pela tabela de símbolos

char *tabela_de_simbolos[MAX]; //Cria tabela de símbolos
int index_tabela = 0; //Inicializa indíce da tabela em 0

//############################### ANALISADOR SEMÂNTICO ###############################
int busca_tabela_simbolos(char var[]) //Retorna o índice da variável na tabela, se ela está na tabela e -1 caso contrário
{
    for (int i = 0; i < MAX; i++) 
    {
        if(strcmp(tabela_de_simbolos[i], var) == 0)
            return i;
    }
    return -1;
}


bool add_na_tabela(char var[]) // Chama a função confere_tabela() para conferir se a variável já foi declarada para verificar se é possível adicioná-la na tabela
{   
    int result = busca_tabela_simbolos(var); 
    if(result < 0)
    {
        strcpy(tabela_de_simbolos[index_tabela++], var);
        return false; //Retorna false se consegue adicionar na tabela
    }
    else
        return true; //Retorna true caso contrário 
    
}

void inicia_tabela() //Inicia a tabela de símbolos colocando cadeia vazia '\0' em todos os elementos do vetor
{
    for (int i = 0; i < MAX; i++) {
        tabela_de_simbolos[i] = malloc(15 * sizeof(char)); // Cada string pode ter até 15 caracteres + '\0'
        if (tabela_de_simbolos[i] != NULL) {
            memset(tabela_de_simbolos[i], '\0', 15); // Inicializa com '\0'
        }
    }
}

int verifica_semantica_outros(InfoAtomo info_atomo) //Verifica se a variável em info_atomo.atributo_ID está presente na tabela
{
    int result = busca_tabela_simbolos(info_atomo.atributo_ID); 
    if(result < 0) //Se o resultado for -1, não está, então retorna erro.
    {
        printf("#%d Erro semântico: variável '%s' não foi declarada anteriormente!\n", info_atomo.linha, info_atomo.atributo_ID);
        exit(0);
    }
    return result; //Caso contrário retorna o endereço obtido.
}

void verifica_semantica(bool checa, InfoAtomo info_atomo) //Realiza a análise semântica dentro da função <lista_variavel> ou <comando_entrada>
{
    int result;
    if(checa) // Se está dentro de <declaracao_de_variaveis>:
    {
        result = add_na_tabela(info_atomo.atributo_ID); //Tenta adicionar variável na tabela
        if(result) //Se ela já existir:
        {
            printf("#%d Erro semântico: variável '%s' já foi declarada anteriormente!\n", info_atomo.linha, info_atomo.atributo_ID);
            exit(0);
        }
    }
    else //Se está dentro do <comando_entrada>:
    {
        result = verifica_semantica_outros(info_atomo); //Verifica se a variável já foi declarada
        printf("LEIT\n");
        printf("ARMZ %d\n", result);
    }
}

void show_tabela() //Exibe tabela de símbolos
{
    printf("\n\tTABELA DE SIMBOLOS\n");
    for (int i = 0; i < MAX; i++) 
    {
        if(strcmp(tabela_de_simbolos[i], "\0")  == 0)
            break;
        printf("%-15s | Endereço: %d\n", tabela_de_simbolos[i], i);
    }
}

int rotulo = 1;

int proximo_rotulo() //Função retorna o valor do rótulo a ser colocado no desvio
{
    return rotulo++;
}

int get_tabela_size() //Função retorna o tamanho da tabela de símbolos
{
    return index_tabela;
}

#endif
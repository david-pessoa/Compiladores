#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "atomo.h"

//############################### ANALISADOR LÉXICO ###############################
#ifndef VAR_H
#define VAR_H
int conta_linha = 1; // conta as linhas do arquivo
char *buffer;

InfoAtomo reconhece_numero()
{
    InfoAtomo info_atomo;
    buffer += 2;
    info_atomo.linha = conta_linha;
    char string_num[20] = "";
    int num = 0;

    while(*buffer == '0' || *buffer == '1')
    {    
        strncat(string_num, buffer, 1);
        buffer++;
    }

    if(*buffer != '\n' && *buffer != '\0' && *buffer != ' ' && *buffer != ',' && *buffer != ';')
    {
        info_atomo.atomo = ERRO;
        strcpy(info_atomo.mensagem_erro, "Erro léxico: número inválido");
        return info_atomo;
    }

    int tamanho = strlen(string_num);

    // Percorre cada caractere da string de binário
    for (int i = 0; i < tamanho; i++) {
        if (string_num[i] == '1') {
            // Calcula 2 elevado à posição (da direita para a esquerda)
            num += pow(2, tamanho - 1 - i);
        }
    }
    info_atomo.numero = num;
    info_atomo.atomo = NUMERO;
    return info_atomo;
}

InfoAtomo ignora_comentario_multiplas_linhas() //Ignora múltiplas linhas de comentário
{
    InfoAtomo info_atomo;
    buffer += 2;
    info_atomo.linha = conta_linha;
    while(!(*buffer == '-' && *(buffer + 1) == '}'))
    {
        buffer++;
        if(*buffer =='\n')
            conta_linha++;
        
        if(*buffer == '\0' || *buffer == 0)
        {
            info_atomo.atomo = ERRO;
            info_atomo.linha = conta_linha;
            strcpy(info_atomo.mensagem_erro, "Erro léxico: comentário finalizado incorretamente");
            return info_atomo;
        }
    }

    if (*(buffer + 2) == '\n') {
       conta_linha++;
    }

    info_atomo.atomo = COMENTARIO;
    buffer += 2;
    return info_atomo;
}

// Função para ignorar uma linha de comentário #
InfoAtomo ignora_comentario()
{
    InfoAtomo info_atomo;
    buffer++;
    info_atomo.linha = conta_linha;
    while(*buffer != '\n')
    {
        buffer++;
        
        if(*buffer == '\0' || *buffer == 0)
        {
            info_atomo.atomo = EOF_BUFFER;
            info_atomo.linha = conta_linha;
            return info_atomo;
        }
    }
    conta_linha++;
    info_atomo.atomo = COMENTARIO;
    buffer++;
    return info_atomo;
}

// IDENTIFICADOR -> LETRA_MINUSCULA (LETRA_MINUSCULA | DIGITO | _ )*
InfoAtomo reconhece_identificador(){
    int tamanho = 1;
    InfoAtomo info_atomo;
    info_atomo.atomo = ERRO;
    char string[100] = ""; //armazena palavra para verificar se é palavra reservada
    
    strncat(string, buffer, 1);
    // ja temos uma letra minuscula
    buffer++;

    //reconhecer as palavras reservadas


    while((islower(*buffer) || isdigit(*buffer) || *buffer == '_') && tamanho <= 15){
        strncat(string, buffer, 1);
        tamanho++;
        buffer++;
    }
    if(!islower(*buffer) && !isdigit(*buffer) && *buffer != '_' && *buffer != ' ' && *buffer != '\n' && *buffer != ',' && *buffer != ';' && *buffer != '(' && *buffer != ')' && *buffer != ':' && *buffer != '.')
    {
        strcpy(info_atomo.mensagem_erro, "Erro léxico: identificador inválido!");
        buffer++;
        return info_atomo;
    }
    else if(tamanho > 15) // se for maior eh erro lexico
    {
        buffer++;
        strcpy(info_atomo.mensagem_erro, "Erro léxico: identificador possui mais de 15 caracteres");
        return info_atomo;
    }
    
    else if(strcmp("and", string) == 0)
    {
        strcpy(info_atomo.atributo_ID, string);
        info_atomo.atomo = AND;
        return info_atomo;
    }

    else if(strcmp("begin", string) == 0)
    {
        strcpy(info_atomo.atributo_ID, string);
        info_atomo.atomo = BEGIN;
        return info_atomo;
    }

    else if(strcmp("boolean", string) == 0)
    {
        strcpy(info_atomo.atributo_ID, string);
        info_atomo.atomo = BOOLEAN;
        return info_atomo;
    }

    else if(strcmp("elif", string) == 0)
    {
        strcpy(info_atomo.atributo_ID, string);
        info_atomo.atomo = ELIF;
        return info_atomo;
    }

    else if(strcmp("end", string) == 0)
    {
        strcpy(info_atomo.atributo_ID, string);
        info_atomo.atomo = END;
        return info_atomo;
    }

    else if(strcmp("false", string) == 0)
    {
        strcpy(info_atomo.atributo_ID, string);
        info_atomo.atomo = FALSE;
        return info_atomo;
    }

    else if(strcmp("for", string) == 0)
    {
        strcpy(info_atomo.atributo_ID, string);
        info_atomo.atomo = FOR;
        return info_atomo;
    }

    else if(strcmp("if", string) == 0)
    {
        strcpy(info_atomo.atributo_ID, string);
        info_atomo.atomo = IF;
        return info_atomo;
    }

    else if(strcmp("integer", string) == 0)
    {
        strcpy(info_atomo.atributo_ID, string);
        info_atomo.atomo = INTEGER;
        return info_atomo;
    }

    else if(strcmp("not", string) == 0)
    {
        strcpy(info_atomo.atributo_ID, string);
        info_atomo.atomo = NOT;
        return info_atomo;
    }

    else if(strcmp("of", string) == 0)
    {
        strcpy(info_atomo.atributo_ID, string);
        info_atomo.atomo = OF;
        return info_atomo;
    }

    else if(strcmp("or", string) == 0)
    {
        strcpy(info_atomo.atributo_ID, string);
        info_atomo.atomo = OR;
        return info_atomo;
    }

    else if(strcmp("program", string) == 0)
    {
        strcpy(info_atomo.atributo_ID, string);
        info_atomo.atomo = PROGRAM;
        return info_atomo;
    }

    else if(strcmp("read", string) == 0)
    {
        strcpy(info_atomo.atributo_ID, string);
        info_atomo.atomo = READ;
        return info_atomo;
    }

    else if(strcmp("set", string) == 0)
    {
        strcpy(info_atomo.atributo_ID, string);
        info_atomo.atomo = SET;
        return info_atomo;
    }

    else if(strcmp("to", string) == 0)
    {
        strcpy(info_atomo.atributo_ID, string);
        info_atomo.atomo = TO;
        return info_atomo;
    }

    else if(strcmp("true", string) == 0)
    {
        strcpy(info_atomo.atributo_ID, string);
        info_atomo.atomo = TRUE;
        return info_atomo;
    }

    else if(strcmp("write", string) == 0)
    {
        strcpy(info_atomo.atributo_ID, string);
        info_atomo.atomo = WRITE;
        return info_atomo;
    }

    else
    {
        strcpy(info_atomo.atributo_ID, string);
        info_atomo.atomo = IDENTIFICADOR;
        return info_atomo;
    }
}

InfoAtomo obter_atomo(){
    InfoAtomo info_atomo;

    // consome espaços em branco quebra de linhas tabulação e retorno de carro
    while(*buffer == ' ' || *buffer == '\n' || *buffer == '\t' ||*buffer == '\r'){
        if(*buffer =='\n')
            conta_linha++;
        buffer++;
    }
    info_atomo.linha = conta_linha;

    // reconhece identificador
    if( islower(*buffer)){ // ser for letra mininuscula
        info_atomo = reconhece_identificador();
    }else if(*buffer == 0){ // Se chegar ao fim do arquivo
        info_atomo.atomo = EOF_BUFFER;
    }
    else if(*buffer == '0' && *(buffer + 1) == 'b') //Reconhece número
    {
        info_atomo = reconhece_numero();
    }
    else if(*buffer == ',')
    {
        info_atomo.atomo = VIRGULA; //Reconhece vírgula
        buffer++;
    }
    else if(*buffer == ';')
    {
        info_atomo.atomo = PONTO_VIRGULA; //Reconhece vírgula
        buffer++;
    }
    else if (*buffer == '.')
    {
        info_atomo.atomo = PONTO;
        buffer++;
    }
    else if (*buffer == ':')
    {
        info_atomo.atomo = DOIS_PONTOS;
        buffer++;
    }
    else if (*buffer == '(')
    {
        info_atomo.atomo = ABRE_PARENTESES;
        buffer++;
    }
    else if (*buffer == ')')
    {
        info_atomo.atomo = FECHA_PARENTESES;
        buffer++;
    }
    else if (*buffer == '+')
    {
        info_atomo.atomo = OP_SOMA;
        buffer++;
    }
    else if (*buffer == '-')
    {
        info_atomo.atomo = OP_SUB;
        buffer++;
    }
    else if (*buffer == '*')
    {
        info_atomo.atomo = OP_MULT;
        buffer++;
    }
    else if (*buffer == '/')
    {
        info_atomo.atomo = OP_DIV;
        buffer++;
    }
    else if (*buffer == '>')
    {
        info_atomo.atomo = OP_MAIOR;
        buffer++;
    }
    else if (*buffer == '<')
    {
        info_atomo.atomo = OP_MENOR;
        buffer++;
    }
    else if (*buffer == '>' && *(buffer + 1) == '=')
    {
        info_atomo.atomo = OP_MAIOR_IGUAL;
        buffer++;
    }
    else if (*buffer == '<' && *(buffer + 1) == '=')
    {
        info_atomo.atomo = OP_MENOR_IGUAL;
        buffer++;
    }
    else if (*buffer == '=')
    {
        info_atomo.atomo = OP_IGUAL;
        buffer++;
    }
    else if (*buffer == '/' && *(buffer + 1) == '=')
    {
        info_atomo.atomo = OP_DIV_IGUAL;
        buffer++;
    }
    else if(*buffer == '{' && *(buffer + 1) == '-') //Se encontra comentário de múltiplas linhas
    {
        info_atomo = ignora_comentario_multiplas_linhas();
    }

    else if(*buffer == '#')
        info_atomo = ignora_comentario();
    
    else
    {
        info_atomo.atomo = ERRO;
        strcpy(info_atomo.mensagem_erro, "Erro léxico: não foi possível identificar o átomo");
    }

    if( info_atomo.atomo == IDENTIFICADOR)
            printf("%03d# %s | %s\n",info_atomo.linha,msgAtomo[info_atomo.atomo], info_atomo.atributo_ID);
        
        else if (info_atomo.atomo == NUMERO) 
            printf("%03d# %s | %d\n", info_atomo.linha, msgAtomo[info_atomo.atomo], info_atomo.numero);

        else if (info_atomo.atomo == ERRO)
        {
            printf("%03d# %s\n",info_atomo.linha, info_atomo.mensagem_erro);
            exit(0);
        }
        
        else
            printf("%03d# %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo]);

    return info_atomo;

}

int le_arquivo(char* nome_arq)
{
    FILE *file;
    long file_size;

    // Abra o arquivo em modo de leitura
    file = fopen(nome_arq, "r");

    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Mova o ponteiro para o final do arquivo e obtenha o tamanho
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);  // Volta para o início do arquivo

    // Aloca memória suficiente para armazenar o conteúdo
    buffer = (char *)malloc(file_size + 1); // +1 para o terminador nulo '\0'
    
    if (buffer == NULL) {
        perror("Erro ao alocar memória");
        fclose(file);
        return 1;
    }

    // Lê o conteúdo do arquivo e coloca na string `buffer`
    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0'; // Adiciona o terminador nulo ao final da string
    fclose(file);
    return 0;
}
#endif
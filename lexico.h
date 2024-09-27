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
char *buffer; //Armazena buffer de entrada com conteúdo do arquivo

InfoAtomo reconhece_numero()
{
    InfoAtomo info_atomo;
    buffer += 2; // Como o buffer já leu '0b', incrementa mais dois
    info_atomo.linha = conta_linha; // Obtém linha do átomo
    char string_num[20] = ""; // Inicia string que guardará o número
    int num = 0; //Inicia a variável que guardará o valor do número em decimal

    while(*buffer == '0' || *buffer == '1') //Enquanto estiver lendo o número em binário:
    {    
        strncat(string_num, buffer, 1); //Guarda os caracteres na string
        buffer++;
    }

    if(*buffer != '\n' && *buffer != '\0' && *buffer != ' ' && *buffer != ',' && *buffer != ';') //Se após o número vier algum caractere inesperado:
    {
        info_atomo.atomo = ERRO; // Gera erro léxico
        strcpy(info_atomo.mensagem_erro, "Erro léxico: número inválido");
        return info_atomo;
    }

    int tamanho = strlen(string_num); // Obtém comprimento do número em binário

    // Percorre cada caractere da string de binário para calcular o valor do número em decimal
    for (int i = 0; i < tamanho; i++) {
        if (string_num[i] == '1') {
            // Calcula 2 elevado à posição (da direita para a esquerda)
            num += pow(2, tamanho - 1 - i);
        }
    }
    info_atomo.numero = num; // Retorna átomo número
    info_atomo.atomo = NUMERO;
    return info_atomo;
}

InfoAtomo ignora_comentario_multiplas_linhas() //Ignora múltiplas linhas de comentário
{
    InfoAtomo info_atomo;
    buffer += 2; // Como o buffer já leu '{-', incrementa mais dois
    info_atomo.linha = conta_linha; //Obtém linha de início do comentário
    while(!(*buffer == '-' && *(buffer + 1) == '}')) //Enquanto o comentário não chega ao fim
    {
        buffer++; //Percorre buffer ignorando o conteúdo
        if(*buffer =='\n') // Se encontrar quebra de linha, conta mais uma linha
            conta_linha++;
        
        if(*buffer == '\0' || *buffer == 0) //Se o conteúdo do buffer terminar sem o fechamento do comentário
        {
            info_atomo.atomo = ERRO; // Retorna erro léxico
            info_atomo.linha = conta_linha;
            strcpy(info_atomo.mensagem_erro, "Erro léxico: comentário finalizado incorretamente");
            return info_atomo;
        }
    }

    if (*(buffer + 2) == '\n') { //Se a última linha do comentário só conter os caracteres '-}\n'
       conta_linha++; //Conta mais uma linha
    }

    info_atomo.atomo = COMENTARIO; //Retorna átomo comentário
    buffer += 2; //Como o buffer já leu '-}', incrementa mais dois
    return info_atomo;
}

// Função para ignorar uma linha de comentário #
InfoAtomo ignora_comentario()
{
    InfoAtomo info_atomo;
    buffer++; //Como o buffer já leu o caractere '#', incrementa um
    info_atomo.linha = conta_linha; //Obtém linha do comentário
    while(*buffer != '\n') //Enquanto não chega ao fim da linha:
    {
        buffer++; //Consome buffer
        
        if(*buffer == '\0' || *buffer == 0) //Se chegar ao fim do buffer
        {
            info_atomo.atomo = EOF_BUFFER; //Retorna átomo indicando fim do buffer
            info_atomo.linha = conta_linha;
            return info_atomo;
        }
    }
    conta_linha++; //Conta mais uma linha
    info_atomo.atomo = COMENTARIO; //Retorna átomo comentário
    buffer++;
    return info_atomo;
}

// Função para reconhecer identificadores e palavras reservadas
InfoAtomo reconhece_palavra(){
    int tamanho = 1; //Armazena tamanho do identificador ou da palavra reservada
    InfoAtomo info_atomo; 
    char string[100] = ""; //a string armazena os caracteres que serão lidos em seguida para verificar se é palavra reservada
    strncat(string, buffer, 1); //armazena caractere já lido à palavra
    buffer++;

    while((islower(*buffer) || isdigit(*buffer) || *buffer == '_') && tamanho <= 15){
        strncat(string, buffer, 1); // Lê os caracteres seguintes, os armazena na string e obtém tamanho da string
        tamanho++;
        buffer++;
    }
    if(!islower(*buffer) && !isdigit(*buffer) && *buffer != '_' && *buffer != ' ' && *buffer != '\n' && *buffer != ',' && *buffer != ';' && *buffer != '(' && *buffer != ')' && *buffer != ':' && *buffer != '.')
    {
        info_atomo.atomo = ERRO; //Caso apareça algum caractere inesperado, gera erro léxico
        strcpy(info_atomo.mensagem_erro, "Erro léxico: identificador inválido!");
        buffer++;
        return info_atomo;
    }
    else if(tamanho > 15) //Se o tamanho do identificador for maior que 15, gera erro léxico
    {
        info_atomo.atomo = ERRO;
        buffer++;
        strcpy(info_atomo.mensagem_erro, "Erro léxico: identificador possui mais de 15 caracteres");
        return info_atomo;
    }
    
    else if(strcmp("and", string) == 0) //Identifica a palavra reservada and
    {
        info_atomo.atomo = AND;
        return info_atomo;
    }

    else if(strcmp("begin", string) == 0) //Identifica a palavra reservada begin
    {
        info_atomo.atomo = BEGIN;
        return info_atomo;
    }

    else if(strcmp("boolean", string) == 0) //Identifica a palavra reservada boolean
    {
        info_atomo.atomo = BOOLEAN;
        return info_atomo;
    }

    else if(strcmp("elif", string) == 0) //Identifica a palavra reservada elif
    {
    
        info_atomo.atomo = ELIF;
        return info_atomo;
    }

    else if(strcmp("end", string) == 0) //Identifica a palavra reservada end
    {
        info_atomo.atomo = END;
        return info_atomo;
    }

    else if(strcmp("false", string) == 0) //Identifica a palavra reservada false
    {
        info_atomo.atomo = FALSE;
        return info_atomo;
    }

    else if(strcmp("for", string) == 0) //Identifica a palavra reservada for
    {
        info_atomo.atomo = FOR;
        return info_atomo;
    }

    else if(strcmp("if", string) == 0) //Identifica a palavra reservada if
    {
        info_atomo.atomo = IF;
        return info_atomo;
    }

    else if(strcmp("integer", string) == 0) //Identifica a palavra reservada integer
    {
        info_atomo.atomo = INTEGER;
        return info_atomo;
    }

    else if(strcmp("not", string) == 0) //Identifica a palavra reservada not
    {
        info_atomo.atomo = NOT;
        return info_atomo;
    }

    else if(strcmp("of", string) == 0) //Identifica a palavra reservada of
    {
        info_atomo.atomo = OF;
        return info_atomo;
    }

    else if(strcmp("or", string) == 0) //Identifica a palavra reservada or
    {
        info_atomo.atomo = OR;
        return info_atomo;
    }

    else if(strcmp("program", string) == 0) //Identifica a palavra reservada program
    {
        info_atomo.atomo = PROGRAM;
        return info_atomo;
    }

    else if(strcmp("read", string) == 0) //Identifica a palavra reservada read
    {
        info_atomo.atomo = READ;
        return info_atomo;
    }

    else if(strcmp("set", string) == 0) //Identifica a palavra reservada set
    {
        info_atomo.atomo = SET;
        return info_atomo;
    }

    else if(strcmp("to", string) == 0) //Identifica a palavra reservada to
    {
        info_atomo.atomo = TO;
        return info_atomo;
    }

    else if(strcmp("true", string) == 0) //Identifica a palavra reservada true
    {
        info_atomo.atomo = TRUE;
        return info_atomo;
    }

    else if(strcmp("write", string) == 0) //Identifica a palavra reservada write
    {
        info_atomo.atomo = WRITE;
        return info_atomo;
    }

    else //Se não identifica que a string lida é uma palavra reservada, é porque ela é um identificador
    {
        info_atomo.atomo = IDENTIFICADOR;
        strcpy(info_atomo.atributo_ID, string);
        return info_atomo;
    }
}

InfoAtomo obter_atomo(){
    InfoAtomo info_atomo;

    // consome espaços em branco quebra de linhas tabulação e retorno de carro
    while(*buffer == ' ' || *buffer == '\n' || *buffer == '\t' ||*buffer == '\r'){
        if(*buffer =='\n') //Se é uma quebra de linha, incrementa o número de linhas
            conta_linha++;
        buffer++;
    }
    info_atomo.linha = conta_linha;

    // reconhece identificador ou uma palavra reservada
    if( islower(*buffer)){ // ser for letra mininuscula
        info_atomo = reconhece_palavra();
    }else if(*buffer == 0){ // Se chegar ao fim do arquivo
        info_atomo.atomo = EOF_BUFFER;
    }
    else if(*buffer == '0' && *(buffer + 1) == 'b') //Reconhece número
    {
        info_atomo = reconhece_numero();
    }
    else if(*buffer == ',') // Se for uma vírgula
    {
        info_atomo.atomo = VIRGULA; //Reconhece vírgula
        buffer++;
    }
    else if(*buffer == ';') // Se for um ponto-vírgula
    {
        info_atomo.atomo = PONTO_VIRGULA;
        buffer++;
    }
    else if (*buffer == '.') // Se for um ponto
    {
        info_atomo.atomo = PONTO;
        buffer++;
    }
    else if (*buffer == ':') // Se for dois pontos
    {
        info_atomo.atomo = DOIS_PONTOS;
        buffer++;
    }
    else if (*buffer == '(') // Se for um abre parênteses
    {
        info_atomo.atomo = ABRE_PARENTESES;
        buffer++;
    }
    else if (*buffer == ')') // Se for um fecha parênteses
    {
        info_atomo.atomo = FECHA_PARENTESES;
        buffer++;
    }
    else if (*buffer == '+') // Se for um mais
    {
        info_atomo.atomo = OP_SOMA;
        buffer++;
    }
    else if (*buffer == '-') // Se for um menos
    {
        info_atomo.atomo = OP_SUB;
        buffer++;
    }
    else if (*buffer == '*') //Se for um operador de multiplicação
    {
        info_atomo.atomo = OP_MULT;
        buffer++;
    }
    else if (*buffer == '/') //Se for um operador de divisão
    {
        info_atomo.atomo = OP_DIV;
        buffer++;
    }
    else if (*buffer == '>') //Se for um operador maior
    {
        info_atomo.atomo = OP_MAIOR;
        buffer++;
    }
    else if (*buffer == '<') //Se for um operador menor
    {
        info_atomo.atomo = OP_MENOR;
        buffer++;
    }
    else if (*buffer == '>' && *(buffer + 1) == '=') //Se for um operador maior igual (>=)
    {
        info_atomo.atomo = OP_MAIOR_IGUAL;
        buffer++;
    }
    else if (*buffer == '<' && *(buffer + 1) == '=') //Se for um operador menor igual (<=)
    {
        info_atomo.atomo = OP_MENOR_IGUAL;
        buffer++;
    }
    else if (*buffer == '=') //Se for um operador de igualdade (=)
    {
        info_atomo.atomo = OP_IGUAL;
        buffer++;
    }
    else if (*buffer == '/' && *(buffer + 1) == '=') //Se for um operador de atribuição e divisão (/=)
    {
        info_atomo.atomo = OP_DIV_IGUAL;
        buffer++;
    }
    else if(*buffer == '{' && *(buffer + 1) == '-') //Se encontra comentário de múltiplas linhas
    {
        info_atomo = ignora_comentario_multiplas_linhas();
    }

    else if(*buffer == '#') //Se encontra comentário de uma linha
        info_atomo = ignora_comentario();
    
    else // Se encontra qualquer outra coisa gera erro léxico:
    {
        info_atomo.atomo = ERRO;
        strcpy(info_atomo.mensagem_erro, "Erro léxico: não foi possível identificar o átomo");
    }

    if( info_atomo.atomo == IDENTIFICADOR) //Imprime mensagem ao identificar átomo do tipo identificador
            printf("%03d# %s | %s\n",info_atomo.linha,msgAtomo[info_atomo.atomo], info_atomo.atributo_ID);
        
        else if (info_atomo.atomo == NUMERO) //Imprime mensagem ao identificar átomo do tipo NUMERO
            printf("%03d# %s | %d\n", info_atomo.linha, msgAtomo[info_atomo.atomo], info_atomo.numero);

        else if (info_atomo.atomo == ERRO) //Imprime mensagem quando encontra erro léxico
        {
            printf("%03d# %s\n",info_atomo.linha, info_atomo.mensagem_erro);
            exit(0); // Finaliza execução do programa logo após imprimir a mensagem de erro
        }
        
        else // Quando identifica qualquer outro tipo de átomo
            printf("%03d# %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo]);

    return info_atomo; // Retorna informações do átomo para o analisador sintático

}

// Função para ler o arquivo de entrada
int le_arquivo(char* nome_arq) //Entra nome do arquivo
{
    FILE *file;
    long file_size;

    // Abra o arquivo em modo de leitura
    file = fopen(nome_arq, "r");

    if (file == NULL) { //Se não consegue abrir o arquivo, retorna erro
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Mova o ponteiro para o final do arquivo e obtenha o tamanho
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);  // Volta para o início do arquivo

    // Aloca memória suficiente para armazenar o conteúdo
    buffer = (char *)malloc(file_size + 1); // +1 para o terminador nulo '\0'
    
    if (buffer == NULL) { //Se não consegue alocar memória, retorna erro
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
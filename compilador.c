#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef enum{ //tipo dos átomos
    IDENTIFICADOR,
    NUMERO,
    ERRO,
    EOF_BUFFER,
    COMENTARIO
}Atomo;

typedef struct{
    Atomo atomo;
    int linha;
    float atributo_numero;
    char atributo_ID[16];
}InfoAtomo;

char *msgAtomo[] = {
    "IDENTIFICADOR",
    "NUMERO",
    "ERRO",
    "EOF_BUFFER",
    "COMENTARIO"
};

//Só existem tipo bool e int
//Função obter_atomo() --> analisador léxico
//Funcão consome() --> analisdor sintático
//Não existe else nem while (pelo que entendi)

//consome() possui obter_atomo() em seu interior (faz chamadas)
//Ignorar caracteres (‘ ’, ‘\n’, ‘\t, ‘\r’) OBS: usar '\n' para fazer contagem de linhas 

//Comentários de uma linha: do '#' até o '\n'
//Comentários de mais de uma linha: do '{-' até o '-}'

//Iddentificadores: letra(letra|digito|_)* (OS IDENTIFICADORES SEMPRE LETRAS minúsculas!)
//OBS: O máximo de caracteres para o identificador é 15, se tem mais tem que dar ERRO

//palavras reservadas = [and, begin, boolean, elif, end, false, for, if, integer, not, of, or, program, read, set, to, true, write]
//são sempre em minúsculo
//Importante: Uma sugestão é que as palavras reservadas sejam reconhecidas na mesma função quereconhece os identificadores e deve ser retornado um átomo específico para cada palavrareservada reconhecida

//Números: somente binários no formato: 0b(0|1)+
//OBS: O número deve ser transformado em decimal

//O Compilador deve ler o arquivo fonte com o nome informado por linha de comando
// Ex: gcc -g -Og -Wall compilador.c -o compilador

//O compilador deve informar na tela do computador cada átomo reconhecido no arquivo

//AO DETECTAR UM ERRO LÉXICO OU SINTÁTICO DEVE-SE PARAR O PROGRAMA E INFORMAR O ERRO E SEU TIPO
//Para erro sintático: deve-se informar o átomo esperado e qual foi o átomo encontrado

//palavras-chaves (int, float...), operadores, identificadores, constantes,
//cadeias de caracteres (strings), e símbolos de pontuação como vírgula,
//ponto e vírgula, dois pontos, e ponto, etc.


//FUNÇÕES DO LÉXICO:
//1) Extração e classificação de átomos
//2) Eliminação de delimitadores e comentários
//3) Conversão numérica
//4) Identificação de palavras reservadas
//5) Tratamento de identificadores
//6) Recuperação de erros
//7) Interação com o sistema de arquivos
//8) Controle da numeração de linhas do programa fonte

int conta_linha = 1; // conta as linhas do arquivo

InfoAtomo obter_atomo(char *buffer){
    InfoAtomo info_atomo;

    // consome espaços em branco quebra de linhas tabulação e retorno de carro
    while(*buffer == ' ' || *buffer == '\n' || *buffer == '\t' ||*buffer == '\r'){
        if( *buffer =='\n')
            conta_linha++;
        buffer++;
    }
    // reconhece identificador
    if( islower(*buffer)){ // ser for letra mininuscula
        info_atomo = reconhece_identificador(buffer);
    }else if(*buffer == 0){
        info_atomo.atomo = EOF_BUFFER;
    }else{
        info_atomo.atomo = ERRO;
    }
    info_atomo.linha = conta_linha;
    return info_atomo;

}
// IDENTIFICADOR -> LETRA_MINUSCULA (LETRA_MINUSCULA | DIGITO )*
InfoAtomo reconhece_identificador(char *buffer){
    InfoAtomo info_atomo;
    info_atomo.atomo = ERRO;
    char *iniID = buffer;
    // ja temos uma letra minuscula
    buffer++;

q1:
    if( islower(*buffer) || isdigit(*buffer)){
        buffer++;
        goto q1;
    }
    if( isupper(*buffer))
        return info_atomo;

    //"  var1@"
    // falta testar o tamanho do identificador que dever ser menor que 15,
    // se for maior eh erro lexico
    strncpy(info_atomo.atributo_ID,iniID,buffer-iniID);
    info_atomo.atributo_ID[buffer-iniID] = 0; // finaliza a string
    info_atomo.atomo = IDENTIFICADOR;
    return info_atomo;
}

char* le_arquivo(char* nome_arq)
{
    FILE *file;
    char *buffer;
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

    return buffer;
}



int main(int argc, char *argv[])
{
    char* nome_arq = argv[1]; //executar como: ./compilador entrada.txt
    char* buffer = le_arquivo(nome_arq);
    

    obter_atomo(buffer);

    InfoAtomo info_atomo;
    do{
        // falta implementar o reconhecimento do atomo NUMERO
        info_atomo = obter_atomo(buffer);
        if( info_atomo.atomo == IDENTIFICADOR)
            printf("%03d# %s | %s\n",info_atomo.linha,msgAtomo[info_atomo.atomo], info_atomo.atributo_ID);
        else
            printf("%03d# %s\n",info_atomo.linha,msgAtomo[info_atomo.atomo]);

    }while(info_atomo.atomo != EOF_BUFFER && info_atomo.atomo != ERRO);
    printf("fim da analise lexica\n");
    return 0;
    





    // Libera memória e fecha o arquivo

    

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


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

int main(int argc, char *argv[])
{
    char* nome_arq = argv[1]; //executar como: ./compilador entrada.txt

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

    

    // Libera memória e fecha o arquivo
    free(buffer);
    fclose(file);

    return 0;
}

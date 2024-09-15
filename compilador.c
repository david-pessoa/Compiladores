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


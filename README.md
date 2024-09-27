# Projeto Compiladores

## Introdução
Para este projeto, resolvemos modularizar o código em 4 arquivos diferentes, a fim de tornar o código mais limpo e organizado. `compilador.c` é responsável por ler o arquivo `entrada.txt`, contendo o programa na linguagem Pascal+, por meio da função `le_arquivo()` pertencente ao módulo `lexico.h` e chamar o analisador sintático por meio da função `programa()` do  módulo `sintatico.h`.  
Já o arquivo `atomo.h` define os tipos que um `Atomo` pode assumir (Identificador, número, palavra reservada, etc.) e também o tipo `InfoAtomo`, que contém atributos do `Atomo` como: seu tipo, a linha onde foi encontrado, string para armazená-lo, caso seja um identificador, uma string para guardar uma mensagem de erro, caso seja gerado um erro, e um inteiro, caso o átomo seja um número.  Além disso, o módulo também conta com um vetor de strings, em que cada string corresponde a um `Atomo`.
Como já é possível notar, os módulos `lexico.h` e `sintatico.h`, carregam as funções do analisador léxico e sintático respectivamente. Eles serão melhor apresentados nas seções seguintes.

## Analisador Léxico

## Analisador Sintático

## Conclusão (?)
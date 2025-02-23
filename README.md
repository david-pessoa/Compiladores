# Integrantes

## David Varão Lima Bentes Pessoa   RA: 10402647
## João Victor Dallapé Madeira RA: 10400725

# Projeto Compiladores

## Parte I

### Introdução
Para este projeto, resolvemos modularizar o código em 4 arquivos diferentes, a fim de tornar o código mais limpo e organizado. `compilador.c` é responsável por ler o arquivo `entrada.txt`, contendo o programa na linguagem Pascal+, por meio da função `le_arquivo()` pertencente ao módulo `lexico.h` e chamar o analisador sintático por meio da função `programa()` do  módulo `sintatico.h`.  
Já o arquivo `atomo.h` define os tipos que um `Atomo` pode assumir (Identificador, número, palavra reservada, etc.) e também o tipo `InfoAtomo`, que contém atributos do `Atomo` como: seu tipo, a linha onde foi encontrado, string para armazená-lo, caso seja um identificador, uma string para guardar uma mensagem de erro, caso seja gerado um erro, e um inteiro, caso o átomo seja um número.  Além disso, o módulo também conta com um vetor de strings, em que cada string corresponde a um `Atomo`.
Como já é possível notar, os módulos `lexico.h` e `sintatico.h`, carregam as funções do analisador léxico e sintático respectivamente. Eles serão melhor apresentados nas seções seguintes.

### Analisador Léxico (lexico.h)
É no analisador léxico que será lido o programa armazenado na string `buffer`, a contagem de linhas e a identificação de átomos do programa. Ele é chamado pelo analisador sintático para obter os átomos do programa. Para isso, foi criada a função `obter_atomo()` que retorna o próximo átomo obtido do programa no tipo `InfoAtomo`, para armazenar as informações do `Atomo`.    

A fim de identificar o `Atomo`, a função ignora caracteres como: ' ', '\t', '\r' e '\n' (quando passa por este último é feita a contagem de uma linha). Depois, verifica-se se o que está sendo lido é um identificador ou palavra reservada (caso seja identificada uma letra minúscula ao início do átomo) por meio da função `reconhece_palavra()`. Caso o átomo comece com `0b`, provavelmente se trata de um número, então é chamada a função `reconhece_numero()` para reconhecê-lo e realizar sua conversão para o formato decimal. Há também a possibilidade de que venha em seguida um comentário de uma linha (inciando com '#') ou de múltiplas linhas (iniciando com '{-' e finalindo com '-}'). Para essas situações, criamos as funções `ignora_comentario()` e `ignora_comentario_multiplas_linhas()` respectivamente. Todas estas funções retornam um struct do tipo `InfoAtomo`, sendo que `reconhece_numero()`utiliza o atributo `numero` de `InfoAtomo` para guardar o número obtido (em caso de êxito) e `reconhece_palavra()`, caso reconheça um identificador, guarda o identificador lido em `atributo_ID`. Além disso, há também a identificação de outros átomos que são operadores matemáticos, relacionais entre outros como: (',', ';', '.', ':', etc.).    

Se a mensagem chega ao fim, é identificado o `Atomo` `EOF_BUFFER`, caso não seja identificado nenhum dos átomos mencionados anteriormente durante a leitura do arquivo, o programa retorna uma mensagem indicando um erro léxico e encerra imediatamente. Após a identificação do `Atomo`, é feita a sua impressão, exibindo a linha em que foi identificado, ao final da função `obter_atomo()` que será então retornado pela função, para que o analisador léxico possa análisá-lo.

### Analisador Sintático (sintatico.h)
É no analisador sintático que se verifica se os átomos gerados pelo analisador léxico seguem a estrutura sintática definida pela gramática da linguagem. Para isso, criaram-se as variáveis `lookahead` do tipo `Atomo`, que inicia com valor `INICIA_SINTATICO` e `info_atomo`. Essa variável será responsável por guardar as informações do próximo átomo, enquanto `lookahead` guarda o valor do `Atomo` de `info_atomo`. Já a função `consome()` é responsável por comparar o valor do átomo esperado, passado por parâmetro, com `lookahead`. Caso os valores sejam iguais, ou se trate apenas de um comentário, é obtido o próximo átomo pela função `obter_atomo()` e os valores de `lookahead` e `info_atomo` são atualizados. Caso contrário, é exibida uma mensagem de erro sintático informando o átomo esperado e o obtido e o programa encerra imediatamente  
  
Para cada não-terminal da gramática fornecida, criamos uma função. Por exemplo, na gramática de Pascal+, `<programa>` é denotado por: `<programa> ::= program identificador “;” <bloco> “.”`. Sendo assim, foi criada a função `programa()` que inicia realizando uma chamada da função `consome(INICIA_SINTATICO)` para iniciar a análise sintática. Depois, é feita uma verificação para conferir se há comentários no início do código (isso também foi feito nas demais funções, respeitando a gramática). A seguir, é feito o consumo dos átomos `program` `identificador` e `;`. Em seguida, analisa-se novamente a existência de um comentário e chama-se a função bloco() e, por fim se consome o átomo final `.`.  
  
É importante ressaltar que foi utilizada a recursão em funções que possuem alguma repetição de cadeia. Por isso, quando uma função chama um terminal que é seguido de um loop, como em: `<lista_variavel> ::= identificador { “,” identificador }`, é feita a contagem do número de recursões para conferir se está na primeira chamada (nível 0) ou num nível mais profundo na recursão (nível > 0). Assim é possível saber se o átomo esperado é o `identifcador` ou `,` no caso de `<lista_variavel>`

## Parte II

Para a parte II desse projeto, criou-se um novo módulo: `semantico.h` para conter funções relacionadas à análise semântica do código. Além disso, modificou-se o arquivo já existente `sintatico.h` a fim de que fosse possível gerar o código MEPA.

### Analisador Semântico (semantico.h)
Para o analisador semântico, criamos 8 funções: `confere_tabela()`, `add_na_tabela()`, `inicia_tabela()`, `verifica_sematica()`, `verifica_semantica_outros()`, `show_tabela()`, `proximo_rotulo()` e `get_index_tabela()`.

* `busca_tabela_simbolos(char var[])`: recebe por parâmetro o nome de uma variável e confere se ela está presente na tabela de símbolos. Se sim, retorna seu respectivo índice, senão, retorna -1;
* `add_na_tabela(char var[])`: também recebe por parâmetro o nome de uma variável para conferir se é possível adicioná-la na tabela de símbolos. Retorna `false` se consegue adicionar na tabela e `true` caso contrário;
* `inicia_tabela()`: Essa função é chamada ao início do programa para criar a tabela de símbolos;
* `verifica_sematica(bool checa, InfoAtomo info_atomo)`: A função recebe por parâmetro um valor booleano para verificar se ela está sendo chamada de dentro de `declaracao_de_variaveis()` ou `comando_entrada()` e o valor da variável `info_atomo` do módulo `sintatico.h`. Quando a função está sendo chamada de `declaracao_de_variaveis()`, tenta-se adicionar o valor de `info_atomo.atributo_ID`. Caso a variável já exista retorna erro. Se a função estiver sendo chamada de `comando_entrada()`, é conferido se `info_atomo.atributo_ID` está presente na tabela. Se estiver, gera o código intermediário respectivo ao comando de entrada. Caso contrário, retorna erro;
* `verifica_semantica_outros(InfoAtomo info_atomo)`: Verifica a semântica para outros pontos no código após a declaração de variáveis, sem ser dentro de `comando_entrada()`. Portanto a função verifica se a variável já existe na tabela, se existir retorna erro, senão, retorna o endereço da variável na tabela de símbolos.
* `show_tabela()`: Imprime a tabela de símbolos;
* `proximo_rotulo()`: Retorna o próximo rótulo;
* `get_tabela_size()`: Retorna o tamanho da tabela de símbolos.
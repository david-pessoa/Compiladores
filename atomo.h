//David Varão Lima Bentes Pessoa  10402647
//João Victor Dallapé Madeira 10400725

#ifndef ATOMO_H    // Verifica se ATOMO_H ainda não foi definido
#define ATOMO_H    // Define ATOMO_H para evitar múltiplas inclusões

typedef enum{ //tipo dos átomos
    IDENTIFICADOR,
    NUMERO,
    ERRO,
    EOF_BUFFER, // Átomo para indicar fim do BUFFER
    COMENTARIO,

    OP_SOMA, //Operadores matemáticos
    OP_MULT,
    OP_SUB,
    OP_DIV,

    OP_MAIOR, //Operadores relacionais
    OP_MENOR,
    OP_MAIOR_IGUAL,
    OP_MENOR_IGUAL,
    OP_IGUAL,
    OP_DIV_IGUAL, //'/='

    AND, //Palavras reservadas da linguagem
    BEGIN,
    BOOLEAN,
    ELIF,
    END,
    FALSE,
    FOR,
    IF,
    INTEGER,
    NOT,
    OF,
    OR,
    PROGRAM,
    READ,
    SET,
    TO,
    TRUE,
    WRITE,

    VIRGULA, //Sinais de "pontuação"
    PONTO_VIRGULA,
    PONTO,
    DOIS_PONTOS,
    ABRE_PARENTESES,
    FECHA_PARENTESES,

    INICIA_SINTATICO // Indica o início do funcionamento do analisador sintático
}Atomo;

char *msgAtomo[] = {
    "IDENTIFICADOR",
    "NUMERO",
    "ERRO",
    "EOF_BUFFER",
    "COMENTARIO",

    "+", //Operadores matemáticos
    "*",
    "-",
    "/",

    ">", //Operadores relacionais
    "<",
    ">=",
    "<=",
    "=",
    "/=",

    "AND", //Palavras reservadas da linguagem
    "BEGIN",
    "BOOLEAN",
    "ELIF",
    "END",
    "FALSE",
    "FOR",
    "IF",
    "INTEGER",
    "NOT",
    "OF",
    "OR",
    "PROGRAM",
    "READ",
    "SET",
    "TO",
    "TRUE",
    "WRITE",

    "VÍRGULA", //Sinais de "pontuação"
    "PONTO VÍRGULA",
    "PONTO",
    "DOIS PONTOS",
    "ABRE PARÊNTESES",
    "FECHA PARÊNTESES"
};

typedef struct InfoAtomo{ //atributos do átomo
    Atomo atomo; // Tipo do átomo
    int linha; // Linha em que o átomo foi encontrado
    char atributo_ID[100]; //Armazena identificador, se o átomo for um identificador
    char mensagem_erro[100]; //string para armazenar possível mensagem de erro gerada
    int numero; //Armazena o número, se o átomo for um número
}InfoAtomo;

#endif  // Finaliza o bloqueio de redefinição

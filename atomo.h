#ifndef ATOMO_H    // Verifica se ATOMO_H ainda não foi definido
#define ATOMO_H    // Define ATOMO_H para evitar múltiplas inclusões

typedef enum{ //tipo dos átomos
    IDENTIFICADOR,
    NUMERO,
    ERRO,
    EOF_BUFFER,
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
    FECHA_PARENTESES
}Atomo;

char *msgAtomo[] = { //Colocar no sintatico.h mais tarde
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

    ",", //Sinais de "pontuação"
    ";",
    ".",
    ":",
    ")",
    "("
};

typedef struct InfoAtomo{
    Atomo atomo;
    int linha;
    char atributo_ID[100];
    char mensagem_erro[100];
    int numero;
}InfoAtomo;

#endif  // Finaliza o bloqueio de redefinição

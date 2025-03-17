%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int yylex(void);
extern int yyerror(const char *s);
%}

%union {
    char *s;
}

/* Declare that the IDENTIFIER token carries a string */
%token <s> IDENTIFIER

/* Declare that the non-terminal variable also carries a string */
%type <s> variable

%%
input: variable '\n' { printf("Valid variable: %s\n", $1); }
     | error '\n'    { yyerror("Not a valid variable"); yyerrok; }
     ;

variable: IDENTIFIER { $$ = $1; }
        ;
%%

int main(void) {
    printf("Enter a variable name followed by Enter:\n");
    yyparse();
    return 0;
}

int yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
    return 0;
}

%{
#include <stdio.h>
#include <stdlib.h>
%}

%token IDENTIFIER

%%
variable: IDENTIFIER { printf("Valid variable: %s\n", $1); }
        ;
%%

int main(void) {
    yyparse();
    return 0;
}

int yyerror(char *s) {
    fprintf(stderr, "Error: %s\n", s);
    return 0;
}

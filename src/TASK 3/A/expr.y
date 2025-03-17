%{
#include <stdio.h>
#include <stdlib.h>
extern int yylex(void);
extern int yyerror(const char *s);
%}

%token NUMBER

%left '+' '-'
%left '*' '/'

%%
input: expr '\n' { printf("Expression parsed successfully!\n"); }
     ;

expr: expr '+' expr   { /* action */ }
    | expr '-' expr   { /* action */ }
    | expr '*' expr   { /* action */ }
    | expr '/' expr   { /* action */ }
    | '(' expr ')'    { /* action */ }
    | NUMBER          { /* action */ }
    ;
%%
int main(void) {
    printf("Enter an arithmetic expression followed by Enter:\n");
    yyparse();
    return 0;
}

int yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
    return 0;
}

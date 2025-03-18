%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
AST *root;  // Global variable to hold the AST root
int yylex(void);
int yyerror(char *s);
%}

%code requires {
  #include "ast.h"
}

%union {
    AST *node;  // For AST pointers
    int num;    // For numeric values (from NUMBER)
}

%token <num> NUMBER
%type <node> expr
%type <node> input
%type <node> newline

%left '+' '-'
%left '*' '/'

%%
input:
      expr newline { root = $1; }
    ;

newline:
      '\n'
    | newline '\n'
    ;

expr: expr '+' expr   { $$ = newast('+', $1, $3); }
    | expr '-' expr   { $$ = newast('-', $1, $3); }
    | expr '*' expr   { $$ = newast('*', $1, $3); }
    | expr '/' expr   { $$ = newast('/', $1, $3); }
    | '(' expr ')'    { $$ = $2; }
    | NUMBER          { $$ = newnum($1); }
    ;
%%

int main(void) {
    printf("Enter an arithmetic expression followed by newline:\n");
    yyparse();
    printf("The Abstract Syntax Tree is:\n");
    print_ast(root);
    printf("\n");
    return 0;
}

int yyerror(char *s) {
    fprintf(stderr, "Error: %s\n", s);
    return 0;
}

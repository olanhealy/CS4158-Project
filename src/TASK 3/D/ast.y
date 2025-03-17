%{
#include <stdio.h>
#include <stdlib.h>

typedef struct AST {
    int nodetype;          // '+', '-', '*', '/', 'N' for number, etc.
    struct AST *left;
    struct AST *right;
    int value;             // valid if nodetype == 'N'
} AST;

AST *newast(int nodetype, AST *left, AST *right);
AST *newnum(int value);
void print_ast(AST *node);
%}

%token NUMBER
%left '+' '-'
%left '*' '/'

%%
expr: expr '+' expr   { $$ = newast('+', $1, $3); }
    | expr '-' expr   { $$ = newast('-', $1, $3); }
    | expr '*' expr   { $$ = newast('*', $1, $3); }
    | expr '/' expr   { $$ = newast('/', $1, $3); }
    | '(' expr ')'    { $$ = $2; }
    | NUMBER          { $$ = newnum($1); }
    ;
%%

#include "ast.c"   // Contains the definitions of newast, newnum, print_ast

int main(void) {
    AST *tree;
    printf("Enter an arithmetic expression:\n");
    yyparse();
    // Suppose the final result of the parse is stored in a global variable (or returned)
    // Here, you would call print_ast(tree); to display the AST
    return 0;
}

int yyerror(char *s) {
    fprintf(stderr, "Error: %s\n", s);
    return 0;
}

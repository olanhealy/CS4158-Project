%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
int yylex(void);
void yyerror(const char *s);
%}

%union {
    char* string;
    ast_node* node;
}

%token MAIN INT
%token <string> ID NUM

%type <node> program declarations id_list statements statement expr term factor

%%

program: MAIN '(' ')' '{' declarations statements '}'
    {
        /* Build the AST with a "program" node having children for declarations and statements */
        ast_root = create_node("program", $5, $6);
    }
    ;

declarations:
      INT id_list ';' { $$ = $2; }
    | /* empty */ { $$ = NULL; }
    ;

id_list:
      id_list ',' ID { $$ = create_node("id_list", $1, create_leaf($3)); }
    | ID { $$ = create_leaf($1); }
    ;

statements:
      statements statement { $$ = create_node("stmt_list", $1, $2); }
    | statement { $$ = $1; }
    ;

statement:
      ID '=' expr ';' { $$ = create_assign_node(create_leaf($1), $3); }
    ;

expr:
      expr '+' term { $$ = create_operator_node("+", $1, $3); }
    | expr '-' term { $$ = create_operator_node("-", $1, $3); }
    | term { $$ = $1; }
    ;

term:
      term '*' factor { $$ = create_operator_node("*", $1, $3); }
    | term '/' factor { $$ = create_operator_node("/", $1, $3); }
    | factor { $$ = $1; }
    ;

factor:
      '(' expr ')' { $$ = $2; }
    | ID { $$ = create_leaf($1); }
    | NUM { $$ = create_leaf($1); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

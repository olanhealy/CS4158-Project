%{
#include <stdio.h>
%}
%option noyywrap
%%
[ \t\n]+                    ;
"int"                       { printf("KEYWORD: %s\n", yytext); }
"String"                    { printf("KEYWORD: %s\n", yytext); }
"long"                      { printf("KEYWORD: %s\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]*       { printf("IDENTIFIER: %s\n", yytext); }
[0-9]+                      { printf("NUMBER: %s\n", yytext); }
[+\-*/=]                   { printf("OPERATOR: %s\n", yytext); }
[,\.;"]                     { printf("PUNCTUATOR: %s\n", yytext); }
.                          { printf("SYMBOL: %s\n", yytext); }
%%
int main(void) {
    printf("Enter a line of code:\n");
    yylex();
    return 0;
}

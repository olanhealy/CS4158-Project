%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int commentCount = 0;
%}

%option noyywrap

%%

\/\/[^\n]*                 { commentCount++; }
[ \t\n]+                   ; // Skip whitespace
"int"|"String"|"long"      { printf("%s Reserved Keyword\n", yytext); }
"main"|"void"              { printf("%s Reserved Keyword\n", yytext); }
[0-9]+\.[0-9]+             { printf("%s Floating Number\n", yytext); }
[0-9]+                     { printf("%s Constant\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]*     { printf("%s Identifier\n", yytext); }
"="|"+"|"-"|"*"|"/"        { printf("%s Arithmetic Operator\n", yytext); }
";"|","|"("|")"|"{"|"}"    { printf("%s Special Operator\n", yytext); }
.                          { printf("%s Unknown Symbol\n", yytext); }

%%

int main() {
    char input[2048] = "";
    char line[256];

    printf("Enter a program (end with $$ on a new line):\n");

    while (fgets(line, sizeof(line), stdin)) {
        if (strcmp(line, "$$\n") == 0 || strcmp(line, "$$\r\n") == 0) {
            break;
        }
        strcat(input, line);
    }

    YY_BUFFER_STATE buffer = yy_scan_string(input);
    yylex();
    yy_delete_buffer(buffer);

    if (commentCount > 0) {
        printf("%d Comments Ignored\n", commentCount);
    }

    return 0;
}

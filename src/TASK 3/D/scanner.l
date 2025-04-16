%{
#include "ast.h"
#include "parser.tab.h"
#include <stdlib.h>
#include <string.h>
%}

%%

"main"            { return MAIN; }
"int"             { return INT; }
[0-9]+            { yylval.string = strdup(yytext); return NUM; }
[a-zA-Z][a-zA-Z0-9]*  { yylval.string = strdup(yytext); return ID; }
"("               { return '('; }
")"               { return ')'; }
"{"               { return '{'; }
"}"               { return '}'; }
";"               { return ';'; }
","               { return ','; }
"="               { return '='; }
"\+"              { return '+'; }
"-"               { return '-'; }
"\*"              { return '*'; }
"/"               { return '/'; }
[ \t\n]+          { /* Skip whitespace */ }
.                 { printf("Unknown character: %s\n", yytext); }
%%

int yywrap(void) {
    return 1;
}

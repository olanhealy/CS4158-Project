%{
#include "var.tab.h"
#include <stdlib.h>
#include <string.h>
%}
%option noyywrap
%%
[a-zA-Z][a-zA-Z0-9]*   { yylval.s = strdup(yytext); return IDENTIFIER; }
[ \t]+                ;
\n                    { return '\n'; }
.                     { return yytext[0]; }
%%

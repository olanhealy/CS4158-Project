%{
#include "ast.tab.h"
#include <stdlib.h>
%}
%option noyywrap
%%
[ \t]+             ;   /* ignore spaces and tabs */
\n                 { return '\n'; }
[0-9]+             { yylval.num = atoi(yytext); return NUMBER; }
"("                { return '('; }
")"                { return ')'; }
"+"                { return '+'; }
"-"                { return '-'; }
"*"                { return '*'; }
"/"                { return '/'; }
.                  { return yytext[0]; }
%%

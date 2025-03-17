%{
#include "calc.tab.h"
%}

%%
[ \t]+             ;   /* ignore spaces and tabs */
\n                 { return '\n'; }
[0-9]+             { yylval = atoi(yytext); return NUMBER; }
"+"                { return '+'; }
"-"                { return '-'; }
"*"                { return '*'; }
"/"                { return '/'; }
"("                { return '('; }
")"                { return ')'; }
.                  { return yytext[0]; }
%%
int yywrap(void) { return 1; }

%{
#include "expr.tab.h"
#include <stdlib.h>
%}

/* 
   Match spaces and tabs only and ignore them.
   Then match newline separately.
*/
%%
[ \t]+             ;  /* Ignore spaces and tabs */
\n                 { return '\n'; }
[0-9]+            { yylval = atoi(yytext); return NUMBER; }
[+\-*/()]         { return yytext[0]; }
.                 { return yytext[0]; }
%%
int yywrap(void) { return 1; }

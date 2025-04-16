#include <stdio.h>
#include "ast.h"

extern int yyparse(void);
extern ast_node *ast_root;

int main(void) {
    // Parse the input from standard input
    if(yyparse() == 0) {
        // If parsing is successful, generate code.
        generate_code(ast_root);
        // Optionally, free the AST once done.
        free_ast(ast_root);
    }
    return 0;
}

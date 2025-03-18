#ifndef AST_H
#define AST_H

typedef struct AST {
    int nodetype;          
    struct AST *left;
    struct AST *right;
    int value;             
} AST;

AST *newast(int nodetype, AST *left, AST *right);
AST *newnum(int value);
void print_ast(AST *node);

#endif

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"  // if you create a header; otherwise define struct in this file

AST *newast(int nodetype, AST *left, AST *right) {
    AST *node = (AST *)malloc(sizeof(AST));
    if (!node) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    node->nodetype = nodetype;
    node->left = left;
    node->right = right;
    return node;
}

AST *newnum(int value) {
    AST *node = (AST *)malloc(sizeof(AST));
    if (!node) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    node->nodetype = 'N';
    node->value = value;
    node->left = node->right = NULL;
    return node;
}

void print_ast(AST *node) {
    if (node == NULL)
        return;
    if (node->nodetype == 'N') {
        printf("%d", node->value);
        return;
    }
    printf("(");
    print_ast(node->left);
    printf(" %c ", node->nodetype);
    print_ast(node->right);
    printf(")");
}

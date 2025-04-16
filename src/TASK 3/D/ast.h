#ifndef AST_H
#define AST_H

#include <stdio.h>

typedef struct ast_node {
    char *op;                  // Operator or token value
    struct ast_node *left;     // Left child
    struct ast_node *right;    // Right child
} ast_node;

extern ast_node *ast_root;     // Global pointer to the AST root

// Creation functions for AST nodes
ast_node* create_node(char* op, ast_node* left, ast_node* right);
ast_node* create_leaf(char* value);
ast_node* create_assign_node(ast_node* id, ast_node* expr);
ast_node* create_operator_node(char* operator, ast_node* left, ast_node* right);

// Free the AST
void free_ast(ast_node* root);

// Code generation functions
char* codegen(ast_node* root);
void generate_code(ast_node *root);

#endif

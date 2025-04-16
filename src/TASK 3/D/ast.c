#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ast_node *ast_root = NULL;
int temp_count = 0;   // For generating temporary names 
int instr_count = 0;  // For numbering instructions

// Create a generic AST node with operator 'op' and two children.
ast_node* create_node(char* op, ast_node* left, ast_node* right) {
    ast_node* node = (ast_node*) malloc(sizeof(ast_node));
    node->op = strdup(op);
    node->left = left;
    node->right = right;
    return node;
}

// Create a leaf node (identifier or number)
ast_node* create_leaf(char* value) {
    return create_node(value, NULL, NULL);
}

// Create assignment node: LHS (identifier) RHS (expression)
ast_node* create_assign_node(ast_node* id, ast_node* expr) {
    return create_node("=", id, expr);
}

// Create an operator node for arithmetic operations 
ast_node* create_operator_node(char* operator, ast_node* left, ast_node* right) {
    return create_node(operator, left, right);
}

// Free the AST (postorder traversal)
void free_ast(ast_node* root) {
    if(root == NULL)
        return;
    free_ast(root->left);
    free_ast(root->right);
    free(root->op);
    free(root);
}

// Recursively generate three-address code from the AST and return a pointer to a temporary name if needed.
char* codegen(ast_node* root) {
    if (root == NULL)
        return NULL;

    // For nodes that serve as a container just process children.
    if (strcmp(root->op, "program") == 0 || strcmp(root->op, "stmt_list") == 0 ||
        strcmp(root->op, "id_list") == 0) {
        if (root->left)
            codegen(root->left);
        if (root->right)
            codegen(root->right);
        return NULL;
    }

    // For assignment nodes: operator "="
    if (strcmp(root->op, "=") == 0) {
        // RHS expression first
        char* expr_temp = codegen(root->right);
        // LHS should be an identifier (leaf)
        char* var = codegen(root->left);
        printf("%d = %s %s\n", instr_count, expr_temp, var);
        instr_count++;
        return var;  // Return variable name 
    }

    // For leaf nodes: identifiers or numbers
    if (root->left == NULL && root->right == NULL) {
        return root->op;
    }

    // For arithmetic operator nodes
    char* left_temp = codegen(root->left);
    char* right_temp = codegen(root->right);
    char* temp = (char*) malloc(10);
    sprintf(temp, "t%d", temp_count++);
    printf("%d %s %s %s %s\n", instr_count, root->op, left_temp, right_temp, temp);
    instr_count++;
    return temp;
}

// Wrapper to print table headers and generate code.
void generate_code(ast_node *root) {
    // Reset counters for each run
    instr_count = 0;
    temp_count = 0;
    printf("-----------------------------------\n");
    printf("Pos Operator Arg1 Arg2 Result\n");
    printf("-----------------------------------\n");
    codegen(root);
    printf("-----------------------------------\n");
}

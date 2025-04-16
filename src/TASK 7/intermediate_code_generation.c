#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int tempCounter = 0;  // Global counter for generating temporary variable names

// Return the precedence of an operator (higher number = higher precedence)
int precedenceValue(char op) {
    switch (op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
    }
    return 0;
}

// Generate a new temporary variable name (T0, T1, T2, ... Tn)
void getTempName(char *nameBuffer) {
    sprintf(nameBuffer, "T%d", tempCounter++);
}

// Convert an infix expression to postfix notation
// The result is stored in the 'postfix' array
void infixToPostfix(const char *infix, char *postfix) {
    char opStack[100];
    int stackTop = -1, pos = 0;
    int len = strlen(infix);
    
    for (int i = 0; i < len; i++) {
        char ch = infix[i];
        if (isspace(ch))
            continue;
        
        if (isalnum(ch)) {
            // Append operands directly to the postfix expression
            postfix[pos++] = ch;
        } else if (ch == '(') {
            opStack[++stackTop] = ch;
        } else if (ch == ')') {
            // Pop until the matching '(' is found
            while (stackTop >= 0 && opStack[stackTop] != '(') {
                postfix[pos++] = opStack[stackTop--];
            }
            if (stackTop >= 0)
                stackTop--;  // remove '('
        } else {
            // Operator encountered
            while (stackTop >= 0 && opStack[stackTop] != '(' &&
                   precedenceValue(opStack[stackTop]) >= precedenceValue(ch)) {
                postfix[pos++] = opStack[stackTop--];
            }
            opStack[++stackTop] = ch;
        }
    }
    // Pop any remaining operators
    while (stackTop >= 0)
        postfix[pos++] = opStack[stackTop--];
    
    postfix[pos] = '\0';
}

// Generate three-address code from a postfix expression
void generateThreeAddressCode(const char *postfix, char lhs) {
    char resultStack[100][20];
    int top = -1;
    int len = strlen(postfix);
    char tempVar[20];
    
    for (int i = 0; i < len; i++) {
        char token = postfix[i];
        if (isalnum(token)) {
            // Convert a single-character operand to a string and push it
            char operand[2] = { token, '\0' };
            strcpy(resultStack[++top], operand);
        } else {
            // Operator encountered. Pop two operands
            char operand2[20], operand1[20];
            strcpy(operand2, resultStack[top--]);
            strcpy(operand1, resultStack[top--]);
            getTempName(tempVar);
            // Print the three-address code instruction
            printf("%s = %s %c %s\n", tempVar, operand1, token, operand2);
            // Push the temporary result onto the stack.
            strcpy(resultStack[++top], tempVar);
        }
    }
    // Assign the top of the result stack to lhs
    printf("%c = %s\n", lhs, resultStack[top]);
}

int main(void) {
    char inputLine[100];
    char infixExpr[100];
    
    printf("Enter an expression:\n");
    fgets(inputLine, sizeof(inputLine), stdin);
    inputLine[strcspn(inputLine, "\n")] = '\0';  // Remove newline character
    char lhs;
    char *rhs = strchr(inputLine, '=');
    if (rhs != NULL) {
        lhs = inputLine[0];  // The target variable (e.g., 'x')
        rhs++;               // Skip the '='
        strcpy(infixExpr, rhs);
    } else {
        printf("Invalid format: No '=' found.\n");
        return 1;
    }
    
    // Convert the RHS infix expression to postfix.
    char postfixExpr[100];
    infixToPostfix(infixExpr, postfixExpr);
    
    printf("Intermediate code:\n");
    generateThreeAddressCode(postfixExpr, lhs);
    
    return 0;
}

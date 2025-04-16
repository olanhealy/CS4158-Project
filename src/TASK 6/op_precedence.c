#include <stdio.h>
#include <string.h>

#define STACK_SIZE 50
#define TABLE_SIZE 10

// Global arrays for terminal symbols and the precedence table
char termSymbols[TABLE_SIZE];
char opTable[TABLE_SIZE][TABLE_SIZE];  
int termCount; 

// Returns the index of a symbol in termSymbols
int findSymbolIndex(char sym) {
    for (int i = 0; i < termCount; i++) {
        if (termSymbols[i] == sym)
            return i;
    }
    return -1;
}

// Returns the precedence relation between the stack-top symbol and input symbol
char lookupRelation(char stackChar, char inputChar) {
    int row = findSymbolIndex(stackChar);
    int col = findSymbolIndex(inputChar);
    if (row == -1 || col == -1)
        return 'X';  // Error code in case a symbol isn’t found
    return opTable[row][col];
}

int main(void) {
    char parseStack[STACK_SIZE] = "$";  // Initialise stack with bottom marker
    char inStr[STACK_SIZE];
    int stackPtr = 1;   // Points just past the '$' in parseStack
    int inputPtr = 0;   // Current index in the input string
    
    // Input the number of terminal symbols
    printf("Enter the number of terminals: ");
    scanf("%d", &termCount);
    
    // Input the terminal symbols.
    printf("Enter the terminal symbols (e.g. + * i $): ");
    for (int i = 0; i < termCount; i++) {
        scanf(" %c", &termSymbols[i]);
    }
    termSymbols[termCount] = '\0';
    
    // Input the operator-precedence table
    printf("\nEnter the operator–precedence table values:\n");
    for (int r = 0; r < termCount; r++) {
        for (int c = 0; c < termCount; c++) {
            printf("Enter relation for %c vs %c: ", termSymbols[r], termSymbols[c]);
            scanf(" %c", &opTable[r][c]);
        }
    }
    
    // Display the table
    printf("\n**** OPERATOR PRECEDENCE TABLE ****\n   ");
    for (int i = 0; i < termCount; i++) {
        printf("  %c", termSymbols[i]);
    }
    printf("\n");
    for (int r = 0; r < termCount; r++) {
        printf(" %c ", termSymbols[r]);
        for (int c = 0; c < termCount; c++) {
            printf("  %c", opTable[r][c]);
        }
        printf("\n");
    }
    
    // Input the string to parse
    printf("\nEnter the input string: ");
    scanf("%s", inStr);
    // Append '$' if not present
    if (inStr[strlen(inStr)-1] != '$') {
        strcat(inStr, "$");
    }
    
    // Header for the trace output
    printf("\n%-15s %-15s %-20s\n", "STACK", "INPUT STRING", "ACTION");
    printf("-------------------------------------------------------------\n");
    
    // shift and reduce actions
    while (1) {
        // Get the symbol at the top of the stack
        char topSym = parseStack[stackPtr - 1];
        // Current input symbol
        char current = inStr[inputPtr];
        
        // Display the current configuration
        printf("%-15s %-15s ", parseStack, inStr + inputPtr);
        
        // Acceptance condition: if both stack and input have only the end-marker
        if (topSym == '$' && current == '$') {
            printf("String Accepted\n");
            break;
        }
        
        // Get precedence relation
        char relation = lookupRelation(topSym, current);
        
        if (relation == '<' || relation == '=') {  
            // SHIFT: Push current input symbol on the stack and move input pointer.
            parseStack[stackPtr++] = current;
            parseStack[stackPtr] = '\0';
            inputPtr++;
            printf("Shift %c\n", current);
        }
        else if (relation == '>') {  
            // REDUCE:  remove one symbol from stack.
            stackPtr--;  
            parseStack[stackPtr] = '\0';
            printf("Reduce\n");
        }
        else if (relation == 'A') {  
            // If the relation is 'A', accept
            printf("String Accepted\n");
            break;
        }
        else {  
            // Unrecognised relation.
            printf("Error: Invalid precedence or symbol\n");
            break;
        }
    }
    
    return 0;
}

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_STATEMENTS 100
#define MAX_WIDTH 50

typedef struct {
    char id[10];
    char constantVal[10];
} ConstantEntry;

int main(void) {
    char oper[6], opnd1[25], opnd2[25], lhs[25];
    char sourceLines[MAX_STATEMENTS][MAX_WIDTH];
    ConstantEntry constants[MAX_STATEMENTS];
    int numConstants = 0, numLines = 0;
    
    printf("Enter 3-address code statements (end input with an empty line):\n");
    // Read each line until an empty line is encountered.
    while(fgets(sourceLines[numLines], MAX_WIDTH, stdin)) {
        if (sourceLines[numLines][0] == '\n') break;
        numLines++;
    }
    
    printf("\nOptimised Code (after Constant Propagation):\n");
    // Process each statement
    for (int i = 0; i < numLines; i++) {
        // Expect each line to be formatted as: operator operand1 operand2 LHS
        sscanf(sourceLines[i], "%s %s %s %s", oper, opnd1, opnd2, lhs);
        
        // If the operator is an assignment ("="), record it as a constant assignment.
        if (strcmp(oper, "=") == 0) {
            int found = 0;
            // Update the constant pool if the variable was seen before.
            for (int j = 0; j < numConstants; j++) {
                if (strcmp(constants[j].id, lhs) == 0) {
                    strcpy(constants[j].constantVal, opnd1);
                    found = 1;
                    break;
                }
            }
            // Otherwise, add it as a new constant.
            if (!found) {
                strcpy(constants[numConstants].id, lhs);
                strcpy(constants[numConstants].constantVal, opnd1);
                numConstants++;
            }
        } else {
            // For non-assignment operations, substitute operands with constants if available.
            for (int j = 0; j < numConstants; j++) {
                if (strcmp(opnd1, constants[j].id) == 0)
                    strcpy(opnd1, constants[j].constantVal);
                if (strcmp(opnd2, constants[j].id) == 0)
                    strcpy(opnd2, constants[j].constantVal);
            }
            // Print the optimised statement.
            printf("%s %s %s %s\n", oper, opnd1, opnd2, lhs);
        }
    }
    
    return 0;
}

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TOKEN_SIZE 100
#define MAX_TOKENS 100

// helper function to print tokens with their types 
bool isKeyword(const char *token) {
    // List of Java keywords, just took these
    const char *keywords[] = {"int", "String", "long", "double", "float", "boolean", "char"};
    int numKeywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < numKeywords; i++) {
        if (strcmp(token, keywords[i]) == 0)
            return true;
    }
    return false;
}

int main() {
    char input[1024];
    printf("Enter a line(s) of code (end input with '$$' on a new line):\n");

    char line[256];
    input[0] = '\0';

    while (fgets(line, sizeof(line), stdin)) {
        if (strcmp(line, "$$\n") == 0) break;  
        strcat(input, line); // Append to total input
    }

    char keywords[MAX_TOKENS][MAX_TOKEN_SIZE];
    char identifiers[MAX_TOKENS][MAX_TOKEN_SIZE];
    char constants[MAX_TOKENS][MAX_TOKEN_SIZE];
    char operators[MAX_TOKENS][MAX_TOKEN_SIZE];
    char symbols[MAX_TOKENS][MAX_TOKEN_SIZE];

    int kwCount = 0, idCount = 0, constCount = 0, opCount = 0, symCount = 0;

    int i = 0;
    while (input[i] != '\0') {
        // Skip whitespace
        if (isspace(input[i])) {
            i++;
            continue;
        }

        // Skip comments
        if (input[i] == '/' && input[i+1] == '/') {
            while (input[i] != '\0' && input[i] != '\n') i++;
            continue;
        }

        char token[MAX_TOKEN_SIZE] = {0};
        int j = 0;

        // Identifiers / Keywords
        if (isalpha(input[i])) {
            while (isalnum(input[i]) || input[i] == '_') {
                token[j++] = input[i++];
            }
            token[j] = '\0';

            if (isKeyword(token)) {
                strcpy(keywords[kwCount++], token);
            } else {
                strcpy(identifiers[idCount++], token);
            }
        }
        // Constants
        else if (isdigit(input[i])) {
            while (isdigit(input[i])) {
                token[j++] = input[i++];
            }
            token[j] = '\0';
            strcpy(constants[constCount++], token);
        }
        // Operators
        else if (strchr("+-*/=", input[i]) != NULL) {
            token[j++] = input[i++];
            token[j] = '\0';
            strcpy(operators[opCount++], token);
        }
        // Symbols
        else if (strchr(",;.", input[i]) != NULL) {
            token[j++] = input[i++];
            token[j] = '\0';
            strcpy(symbols[symCount++], token);
        }
        // Skip anything else
        else {
            i++;
        }
    }

    // Print results
    printf("\nConstants:   [");
    for (int i = 0; i < constCount; i++) {
        printf("'%s'%s", constants[i], (i < constCount - 1) ? ", " : "");
    }
    printf("]\n");

    printf("KeyWord:     [");
    for (int i = 0; i < kwCount; i++) {
        printf("'%s'%s", keywords[i], (i < kwCount - 1) ? ", " : "");
    }
    printf("]\n");

    printf("Identifier:  [");
    for (int i = 0; i < idCount; i++) {
        printf("'%s'%s", identifiers[i], (i < idCount - 1) ? ", " : "");
    }
    printf("]\n");

    printf("Operators:   [");
    for (int i = 0; i < opCount; i++) {
        printf("'%s'%s", operators[i], (i < opCount - 1) ? ", " : "");
    }
    printf("]\n");

    printf("Symbols:     [");
    for (int i = 0; i < symCount; i++) {
        printf("'%s'%s", symbols[i], (i < symCount - 1) ? ", " : "");
    }
    printf("]\n");

    return 0;
}

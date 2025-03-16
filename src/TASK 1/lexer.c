#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TOKEN_SIZE 100

// enum of the types of tokens
typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_LITERAL,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION,
    TOKEN_UNKNOWN
} TokenType;

// helper function to print tokens with their types 
void printToken(TokenType type, const char *lexeme) {
    switch(type) {
        case TOKEN_KEYWORD:
            printf("Keyword: %s\n", lexeme);
            break;
        case TOKEN_IDENTIFIER:
            printf("Identifier: %s\n", lexeme);
            break;
        case TOKEN_LITERAL:
            printf("Literal: %s\n", lexeme);
            break;
        case TOKEN_OPERATOR:
            printf("Operator: %s\n", lexeme);
            break;
        case TOKEN_PUNCTUATION:
            printf("Punctuation: %s\n", lexeme);
            break;
        default:
            printf("Unknown: %s\n", lexeme);
            break;
    }
}

//function check if something is keyword
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
    char input[256];

    //User to input a LOC
    printf("Enter a line of code: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }

    int i = 0;
    while (input[i] != '\0') {
        // Skip all whitespace: spaces, tabs, new lines, etc. 
        if (isspace((unsigned char)input[i])) {
            i++;
            continue;
        }

        char token[MAX_TOKEN_SIZE] = {0};
        int j = 0;
        TokenType type = TOKEN_UNKNOWN;

        // identifiers
        if (isalpha((unsigned char)input[i])) {
            while (i < (int)strlen(input) && (isalnum((unsigned char)input[i]) || input[i]=='_') && j < MAX_TOKEN_SIZE - 1) {
                token[j++] = input[i++];
            }
            token[j] = '\0';
            if (isKeyword(token))
                type = TOKEN_KEYWORD;
            else
                type = TOKEN_IDENTIFIER;
        }
        // literals
        else if (isdigit((unsigned char)input[i])) {
            while (i < (int)strlen(input) && isdigit((unsigned char)input[i]) && j < MAX_TOKEN_SIZE - 1) {
                token[j++] = input[i++];
            }
            token[j] = '\0';
            type = TOKEN_LITERAL;
        }
        // operators
        else if (strchr("+-*/=", input[i]) != NULL) {
            token[j++] = input[i++];
            token[j] = '\0';
            type = TOKEN_OPERATOR;
        }
        // puncuations
        else if (strchr(",;.", input[i]) != NULL) {
            token[j++] = input[i++];
            token[j] = '\0';
            type = TOKEN_PUNCTUATION;
        }
        // everything else is unknow
        else {
            token[j++] = input[i++];
            token[j] = '\0';
            type = TOKEN_UNKNOWN;
        }
        
        printToken(type, token);
    }

    return 0;
}

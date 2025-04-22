#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Functions
void add_symbol(char *arr, char ch);
void FIND_FIRST(char *arr, char ch);
void FIND_FOLLOW(char *arr, char ch);
void FIRST_SHOW();
void FOLLOW_SHOW();
int  CREATE_LL1_TABLE();
void PARSING_TABLE_SHOW(int flag);
void LL1_PARSER(char *str);
void trim(char *str);

// Global variables
int top = 0;           // Stack pointer for parsing
int t = 0, nt = 0, cr = 0, count = 0;
char FIRST[100][100] = {0}, FOLLOW[100][100] = {0};
char T[100] = {0};     // Terminals array
char NT[100] = {0};    // Non-terminals array
char G[100][100] = {0}; // Productions array 
char STACK[100] = {0};  // Parsing stack
int LL1[100][100] = {0}; // LL(1) parsing table

// Add a symbol to an array if not already present.
void add_symbol(char *arr, char ch) {
    int i, flag = 0;
    for(i = 0; arr[i] != '\0'; i++){
        if(arr[i] == ch) {
            flag = 1;
            break;
        }
    }
    if(!flag) {
        int len = (int)strlen(arr);
        arr[len] = ch;
        arr[len+1] = '\0';
    }
}

// Remove leading and trailing whitespace from a string
void trim(char *str) {
    while(isspace((unsigned char)*str)) str++;
    char *end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    *(end+1) = '\0';
}

// For symbol ch, recursively compute its FIRST set
// For terminal symbols, FIRST is the symbol itself
void FIND_FIRST(char *arr, char ch) {
    if (!isupper(ch)) {
        add_symbol(arr, ch);
        return;
    }
    int i;
    for (i = 0; i < cr; i++) {
        if (G[i][0] == ch) {
            // Production is of the form eg "A->..."
            char prodRHS[100];
            strcpy(prodRHS, G[i] + 3); 
            char *token = strtok(prodRHS, "|");
            while(token != NULL) {
                trim(token);
                if(token[0] != '\0') {
                    if (!isupper(token[0]))
                        add_symbol(arr, token[0]);
                    else
                        FIND_FIRST(arr, token[0]);
                }
                token = strtok(NULL, "|");
            }
        }
    }
}

// FIND_FOLLOW: for non-terminal ch, compute FOLLOW set.
void FIND_FOLLOW(char arr[], char ch) {
    int i, j;
    if(ch == G[0][0])
        add_symbol(arr, '$');
    for(i = 0; i < cr; i++) {
        char *rhs = G[i] + 3;
        char copy[100];
        strcpy(copy, rhs);
        char *token = strtok(copy, "|");
        while(token != NULL) {
            int len = strlen(token);
            for(j = 0; j < len; j++) {
                if(token[j] == ch) {
                    if(j + 1 < len) {
                        char next = token[j+1];
                        if(!isupper(next))
                            add_symbol(arr, next);
                        else {
                            char tmp[100] = "";
                            FIND_FIRST(tmp, next);
                            int k;
                            for(k = 0; k < (int)strlen(tmp); k++) {
                                if(tmp[k] != '#' && tmp[k] != '^')
                                    add_symbol(arr, tmp[k]);
                            }
                        }
                    } else {
                        if(G[i][0] != ch)
                            FIND_FOLLOW(arr, G[i][0]);
                    }
                }
            }
            token = strtok(NULL, "|");
        }
    }
}

// Print FIRST sets for each non-terminal.
void FIRST_SHOW() {
    int i, j;
    char arr[100];
    for(i = 0; i < nt; i++) {
        arr[0] = '\0';
        count = 0;
        FIND_FIRST(arr, NT[i]);
        strcpy(FIRST[i], arr);
    }
    printf("\n");
    for(i = 0; i < nt; i++) {
        printf("FIRST( %c ): { ", NT[i]);
        for(j = 0; j < (int)strlen(FIRST[i]); j++) {
            printf("%c", FIRST[i][j]);
            if(j < (int)strlen(FIRST[i]) - 1)
                printf(", ");
            else
                printf(" ");
        }
        printf("}\n");
    }
}

// Print FOLLOW sets for each non-terminal
void FOLLOW_SHOW() {
    int i, j;
    char arr[100];
    for(i = 0; i < nt; i++) {
        arr[0] = '\0';
        count = 0;
        FIND_FOLLOW(arr, NT[i]);
        strcpy(FOLLOW[i], arr);
    }
    printf("\n");
    for(i = 0; i < nt; i++) {
        printf("FOLLOW( %c ): { ", NT[i]);
        for(j = 0; j < (int)strlen(FOLLOW[i]); j++) {
            printf("%c", FOLLOW[i][j]);
            if(j < (int)strlen(FOLLOW[i]) - 1)
                printf(", ");
            else
                printf(" ");
        }
        printf("}\n");
    }
}

// Builds the predictive parsing table
// Returns 0 if no conflicts, 1 otherwise
int CREATE_LL1_TABLE() {
    int i, j, k, pos, flag = 0;
    char arr[100];
    for(i = 0; i < cr; i++) {
        arr[0] = '\0';
        count = 0;
        // Compute FIRST for the RHS (starting at index 3)
        FIND_FIRST(arr, G[i][3]);
        // Find row index corresponding to LHS of production
        for(k = 0; k < nt; k++) {
            if(NT[k] == G[i][0]) { pos = k; break; }
        }
        int len = strlen(arr);
        for(j = 0; j < len; j++) {
            if(isprint(arr[j]) && arr[j] != '#' && arr[j] != '^') {
                int t_index;
                for(t_index = 0; t_index < t; t_index++) {
                    if(T[t_index] == arr[j]) {
                        if(LL1[pos][t_index] != 0) {
                            printf("\nConflict between %s and %s rules!", G[LL1[pos][t_index]-1], G[i]);
                            printf("\nGiven grammar is not LL(1)!\n");
                            flag = 1;
                            return flag;
                        }
                        else {
                            LL1[pos][t_index] = i + 1;
                        }
                        break;
                    }
                }
            } else {
                // If epsilon is in FIRST, then add entries for all symbols in FOLLOW of LHS
                char fol[100] = "";
                FIND_FOLLOW(fol, G[i][0]);
                int fol_len = strlen(fol);
                for(int m = 0; m < fol_len; m++) {
                    int t_index;
                    for(t_index = 0; t_index < t; t_index++) {
                        if(T[t_index] == fol[m]) {
                            if(LL1[pos][t_index] != 0) {
                                printf("\nConflict between %s and %s rules!", G[LL1[pos][t_index]-1], G[i]);
                                printf("\nGiven grammar is not LL(1)!\n");
                                flag = 1;
                                return flag;
                            }
                            else
                                LL1[pos][t_index] = i + 1;
                            break;
                        }
                    }
                }
            }
        }
    }
    return flag;
}

// Display the Predictive Parsing Table.
void PARSING_TABLE_SHOW(int flag) {
    int i, j;
    if(flag == 0) {
        printf("\nPredictive Parsing Table:\n\n\t");
        for(j = 0; j < t; j++) {
            printf("\t%c", T[j]);
        }
        printf("\n----------------------------------------------------\n");
        for(i = 0; i < nt; i++) {
            printf("%c\t|", NT[i]);
            for(j = 0; j < t; j++) {
                if(LL1[i][j] != 0)
                    printf("\t%s", G[LL1[i][j] - 1]);
                else
                    printf("\t_");
            }
            printf("\n");
        }
    }
}

// Uses the predictive parsing table to parse the input string.
void LL1_PARSER(char *str) {
    int i = 0, j, pos, pos1, n;
    int len = strlen(str);
    str[len] = '$';
    str[len+1] = '\0';
    top = 0;
    STACK[top++] = '$';
    STACK[top++] = G[0][0]; // push starting symbol (from the first production's LHS)
    STACK[top] = '\0';
    printf("\n\nParsing sequence and actions\n");
    printf("STACK\t\t\tINPUT\t\t\tACTION\n");
    printf("--------------------------------------------------------------------------\n");
    while(STACK[top-1] != '$') {
        printf("%-24s%-24s", STACK, str + i);
        char X = STACK[top-1];
        if(X == str[i]) {
            printf("Matched: %c", X);
            top--;  // pop terminal from stack
            STACK[top] = '\0';
            i++;    // consume input symbol
        } else if(!isupper(X)) {
            printf("Error: terminal mismatch (expected %c, got %c)", X, str[i]);
            return;
        } else {
            // X is a non-terminal
            for(j = 0; j < nt; j++) {
                if(NT[j] == X) { pos = j; break; }
            }
            for(j = 0; j < t; j++) {
                if(T[j] == str[i]) { pos1 = j; break; }
            }
            n = LL1[pos][pos1];  // production number from the table
            if(n == 0) {
                printf("Error: no rule for %c on input %c", X, str[i]);
                return;
            }
            printf("Shift: %s", G[n-1]);
            top--; // pop non-terminal X
            STACK[top] = '\0';
            char *prod = G[n-1] + 3;// production's RHS
            trim(prod); 
            // For epsilon production do not push any symbol.
            if(prod[0] != '#' && prod[0] != '^' && prod[0] != '\0') {
                int l = strlen(prod);
                // Push production symbols in reverse order.
                for(j = l - 1; j >= 0; j--) {
                    // Do not push the alternative separator if any.
                    if(prod[j] != '|')
                        STACK[top++] = prod[j];
                }
                STACK[top] = '\0';
            }
        }
        printf("\n");
    }
    printf("%-24s%-24s\n", STACK, str + i);
    if(STACK[top-1] == '$' && str[i] == '$')
        printf("Parsed successfully.\n");
    else
        printf("Parsing failed.\n");
}

int main() {
    int numProds, i;
    char inputString[100];
    char prodLine[100];
    
    // Initialise counters and arrays.
    top = 0; t = 0; nt = 0; cr = 0; count = 0;
    memset(FIRST, 0, sizeof(FIRST));
    memset(FOLLOW, 0, sizeof(FOLLOW));
    memset(T, 0, sizeof(T));
    memset(NT, 0, sizeof(NT));
    memset(G, 0, sizeof(G));
    memset(STACK, 0, sizeof(STACK));
    memset(LL1, 0, sizeof(LL1));
    
    // Read grammar productions
    printf("Enter number of production rules: ");
    scanf("%d", &numProds);
    getchar(); 
    
    // For each production rule, split alternatives if any
    for(i = 0; i < numProds; i++) {
        printf("Enter production %d: ", i+1);
        fgets(prodLine, 100, stdin);
        prodLine[strcspn(prodLine, "\n")] = '\0'; // remove newline
        char left = prodLine[0];
        // Record left-hand non-terminal.
        int found = 0, j;
        for(j = 0; j < nt; j++) {
            if(NT[j] == left) { found = 1; break; }
        }
        if(!found) {
            NT[nt++] = left;
        }
        // Get pointer to right-hand side 
        char *rhs = prodLine + 3;
        // Make a copy so strtok does not affect original line
        char rhsCopy[100];
        strcpy(rhsCopy, rhs);
        // Split by '|'
        char *token = strtok(rhsCopy, "|");
        while(token != NULL) {
            trim(token);
            // Store production as "left->token"
            sprintf(G[cr], "%c->%s", left, token);
            cr++;
            // Add terminals from token.
            for(j = 0; token[j] != '\0'; j++) {
                if(!isupper(token[j]) && token[j] != '$' && !isspace((unsigned char)token[j])) {
                    int flag = 0, k;
                    for(k = 0; k < t; k++) {
                        if(T[k] == token[j]) { flag = 1; break; }
                    }
                    if(!flag)
                        T[t++] = token[j];
                }
            }
            token = strtok(NULL, "|");
        }
    }
    // Add '$' to the set of terminals
    T[t++] = '$';
    T[t] = '\0';
    
    // Display grammar
    printf("\nGrammar\n");
    printf("Starting symbol is: %c\n", G[0][0]);
    printf("Non-Terminal symbols: ");
    for(i = 0; i < nt; i++)
        printf("%c ", NT[i]);
    printf("\nTerminal symbols: ");
    for(i = 0; i < t; i++)
        printf("%c ", T[i]);
    printf("\nProduction rules:");
    for(i = 0; i < cr; i++)
        printf("\n%s", G[i]);
    printf("\n");
    
    // Compute and display FIRST and FOLLOW sets
    FIRST_SHOW();
    FOLLOW_SHOW();
    
    // Create and display the LL(1) parsing table
    int flag = CREATE_LL1_TABLE();
    PARSING_TABLE_SHOW(flag);
    
    // Parse the input string.
    if(flag == 0) {
        printf("\nEnter string for parsing: ");
        scanf("%s", inputString);
        LL1_PARSER(inputString);
    }
    
    return 0;
}

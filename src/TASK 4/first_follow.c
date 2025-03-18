#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SYMBOLS 50
#define MAX_PRODUCTIONS 50
#define MAX_RHS_LENGTH 100

// Structure to store a production rule
typedef struct {
    char left;                 // Left-hand side (non-terminal, e 'S')
    char right[MAX_RHS_LENGTH]; // Right-hand side (e.g., "daT|Rf")
} Production;

// Global arrays for FIRST and FOLLOW sets.
char FIRST[128][MAX_SYMBOLS];
char FOLLOW[128][MAX_SYMBOLS];

// List of non-terminals encountered
char nonTerminals[MAX_SYMBOLS] = "";
int nonTerminalCount = 0;


// For epison input we treat '^' or '#' as epsilon
int isEpsilonChar(char c) {
    return (c == '#' || c == '^');
}

// Check if a character is an uppercase letter => non-terminal
int isNonTerminal(char c) {
    return (c >= 'A' && c <= 'Z');
}

// Add a symbol to a set if it's not already present
void addToSet(char *set, char c) {
    if (!strchr(set, c)) {
        int len = (int)strlen(set);
        set[len] = c;
        set[len+1] = '\0';
    }
}

// Merge symbols from src into dest (ignoring epsilon chars when merging)
// Returns 1 if new symbol(s) were added, 0 otherwise
int mergeSet(char *dest, const char *src) {
    int changed = 0;
    for (int i = 0; i < (int)strlen(src); i++) {
        char c = src[i];
        if (isEpsilonChar(c))
            continue; 
        if (!strchr(dest, c)) {
            addToSet(dest, c);
            changed = 1;
        }
    }
    return changed;
}

// Initialise FIRST sets for all terminal symbols
// For any terminal, FIRST(terminal) = { terminal }
void initTerminalFIRST() {
    for (int c = 0; c < 128; c++) {
        if (!isNonTerminal(c) && !isspace(c) && c != '\0') {
            FIRST[c][0] = (char)c;
            FIRST[c][1] = '\0';
        }
    }
}

// Compute FIRST for a given non-terminal symbol by scanning productions
void computeFIRSTForSymbol(char symbol, Production *prods, int prodCount) {
    for (int i = 0; i < prodCount; i++) {
        if (prods[i].left == symbol) {
            // Copy right side so strtok doesn't modify original
            char rhsCopy[MAX_RHS_LENGTH];
            strcpy(rhsCopy, prods[i].right);
            // Split the right-hand side by '|'
            char *alt = strtok(rhsCopy, "|");
            while (alt != NULL) {
                int addEpsilon = 1;  
                int len = (int)strlen(alt);
                for (int j = 0; j < len && addEpsilon; j++) {
                    char s = alt[j];
                    // If we see an epsilon char, add it (store as '^') and stop
                    if (isEpsilonChar(s)) {
                        addToSet(FIRST[symbol], '^');
                        addEpsilon = 0;
                        break;
                    }
                    if (isNonTerminal(s)) {
                        // Merge FIRST(s) into FIRST(symbol)
                        mergeSet(FIRST[symbol], FIRST[s]);
                        // If FIRST(s) does not contain epsilon, stop
                        if (!strchr(FIRST[s], '^'))
                            addEpsilon = 0;
                    } else {
                        // merge its FIRST set (which is just itself) into FIRST(symbol)
                        mergeSet(FIRST[symbol], FIRST[s]);
                        addEpsilon = 0;
                    }
                }
                if (addEpsilon) {
                    addToSet(FIRST[symbol], '^');
                }
                alt = strtok(NULL, "|");
            }
        }
    }
}

// Iteratively compute FIRST sets for all non-terminals
void computeAllFIRST(Production *prods, int prodCount) {
    initTerminalFIRST();
    int changed = 1;
    while (changed) {
        changed = 0;
        for (int i = 0; i < nonTerminalCount; i++) {
            char nt = nonTerminals[i];
            int oldSize = (int)strlen(FIRST[nt]);
            computeFIRSTForSymbol(nt, prods, prodCount);
            if ((int)strlen(FIRST[nt]) > oldSize)
                changed = 1;
        }
    }
}

// Compute FOLLOW sets 
void computeAllFOLLOW(Production *prods, int prodCount, char startSymbol) {
    // Add '$' to FOLLOW(startSymbol)
    addToSet(FOLLOW[startSymbol], '$');
    int changed = 1;
    while (changed) {
        changed = 0;
        for (int i = 0; i < prodCount; i++) {
            char A = prods[i].left;
            char rhsCopy[MAX_RHS_LENGTH];
            strcpy(rhsCopy, prods[i].right);
            char *alt = strtok(rhsCopy, "|");
            while (alt != NULL) {
                int len = (int)strlen(alt);
                for (int j = 0; j < len; j++) {
                    char B = alt[j];
                    if (isNonTerminal(B)) {
                        int k = j + 1;
                        int canEpsilon = 1;
                        char betaFIRST[MAX_SYMBOLS] = "";
                        // Collect FIRST of the symbols following B
                        while (k < len && canEpsilon) {
                            char X = alt[k];
                            mergeSet(betaFIRST, FIRST[X]);
                            if (!strchr(FIRST[X], '^'))
                                canEpsilon = 0;
                            k++;
                        }
                        // Remove epsilon marker from betaFIRST if present.
                        char *ep = strchr(betaFIRST, '^');
                        if (ep) *ep = '\0';
                        int oldSize = (int)strlen(FOLLOW[B]);
                        mergeSet(FOLLOW[B], betaFIRST);
                        // If beta can produce epsilon, add FOLLOW(A) to FOLLOW(B)
                        if (canEpsilon) {
                            mergeSet(FOLLOW[B], FOLLOW[A]);
                        }
                        if ((int)strlen(FOLLOW[B]) > oldSize)
                            changed = 1;
                    }
                }
                alt = strtok(NULL, "|");
            }
        }
    }
}

int main() {
    Production prods[MAX_PRODUCTIONS];
    int prodCount;

    printf("Enter number of productions: ");
    scanf("%d", &prodCount);
    getchar(); 

    // Read productions in the format: S->AB|eDa
    char startSymbol = 0;
    for (int i = 0; i < prodCount; i++) {
        char line[200];
        printf("Enter production %d: ", i + 1);
        fgets(line, sizeof(line), stdin);
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) break;

        char left = line[0];
        if (startSymbol == 0)
            startSymbol = left;
        char *arrow = strstr(line, "->");
        if (!arrow) {
            printf("Invalid production format (missing '->'). Try again.\n");
            i--;
            continue;
        }
        char rightSide[200];
        strcpy(rightSide, arrow + 2); 

        // Remove spaces from rightSide
        int pos = 0;
        for (int j = 0; j < (int)strlen(rightSide); j++) {
            if (!isspace((unsigned char)rightSide[j])) {
                rightSide[pos++] = rightSide[j];
            }
        }
        rightSide[pos] = '\0';

        prods[i].left = left;
        strcpy(prods[i].right, rightSide);

        // Add left-hand side non-terminal if not already recorded
        if (!strchr(nonTerminals, left)) {
            int len = (int)strlen(nonTerminals);
            nonTerminals[len] = left;
            nonTerminals[len + 1] = '\0';
            nonTerminalCount++;
        }
    }

    // Initialise FIRST and FOLLOW sets to empty
    for (int i = 0; i < 128; i++) {
        FIRST[i][0] = '\0';
        FOLLOW[i][0] = '\0';
    }

    computeAllFIRST(prods, prodCount);
    computeAllFOLLOW(prods, prodCount, startSymbol);

    // Print FIRST sets.
    printf("\n=== FIRST Sets ===\n");
    for (int i = 0; i < nonTerminalCount; i++) {
        char nt = nonTerminals[i];
        printf("FIRST(%c) = { ", nt);
        for (int j = 0; j < (int)strlen(FIRST[nt]); j++) {
            char c = FIRST[nt][j];
            if (isEpsilonChar(c)) c = '^';
            printf("%c ", c);
        }
        printf("}\n");
    }

    // Print FOLLOW sets.
    printf("\n=== FOLLOW Sets ===\n");
    for (int i = 0; i < nonTerminalCount; i++) {
        char nt = nonTerminals[i];
        printf("FOLLOW(%c) = { ", nt);
        for (int j = 0; j < (int)strlen(FOLLOW[nt]); j++) {
            printf("%c ", FOLLOW[nt][j]);
        }
        printf("}\n");
    }

    return 0;
}

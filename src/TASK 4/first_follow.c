#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_RULES 20
#define MAX_RHS 50
#define MAX_INPUT 200
#define EPSILON '#'

// Global arrays to store grammar productions, nonterminals, and terminals.
char rules[MAX_RULES][MAX_RHS];
char NT_list[MAX_RULES];
char T_list[MAX_RULES];
int ruleCount = 0, NT_count = 0, T_count = 0;

// FIRST and FOLLOW sets are stored indexed by ASCII values.
char FIRST_set[128][MAX_RULES];
char FOLLOW_set[128][MAX_RULES];

// Returns true if symbol is considered a terminal: not uppercase and not epsilon.
int isTerminal(char sym) {
    return (!isupper(sym) && sym != EPSILON);
}

// Returns true if symbol is a nonterminal (uppercase letter).
int isNonTerminal(char sym) {
    return (isupper(sym));
}

// Searches for a nonterminal in list.
int indexOfNT(char sym) {
    for (int i = 0; i < NT_count; i++) {
        if (NT_list[i] == sym)
            return i;
    }
    return -1;
}

// Adds a symbol to a set (string) if it isn’t already present.
void setInsert(char *setStr, char sym) {
    if (!strchr(setStr, sym)) {
        int len = strlen(setStr);
        setStr[len] = sym;
        setStr[len+1] = '\0';
    }
}

// Compute FIRST set for a given symbol (recursively)
// The result is accumulated in 'firstResult'.
void computeFirst(char sym, char *firstResult) {
    // If it is a terminal, FIRST is itself.
    if (isTerminal(sym)) {
        setInsert(firstResult, sym);
        return;
    }
    // For each production whose LHS equals sym:
    for (int i = 0; i < ruleCount; i++) {
        if (rules[i][0] == sym) {
            char *rhsPart = rules[i] + 2;  
            if (rhsPart[0] == EPSILON) {
                setInsert(firstResult, EPSILON);
            } else {
                int epsilonFlag = 1;
                int len = strlen(rhsPart);
                for (int j = 0; j < len && epsilonFlag; j++) {
                    char tempFirst[MAX_RULES] = "";
                    computeFirst(rhsPart[j], tempFirst);
                    for (int k = 0; k < strlen(tempFirst); k++) {
                        if (tempFirst[k] != EPSILON)
                            setInsert(firstResult, tempFirst[k]);
                    }
                    if (!strchr(tempFirst, EPSILON))
                        epsilonFlag = 0;
                    if (j == len - 1 && epsilonFlag)
                        setInsert(firstResult, EPSILON);
                }
            }
        }
    }
}

// Iteratively update FIRST sets for all nonterminals.
void updateAllFirsts() {
    for (int i = 0; i < NT_count; i++) {
        computeFirst(NT_list[i], FIRST_set[(int)NT_list[i]]);
    }
}

// Compute FOLLOW set for a given nonterminal
// The result is accumulated in 'followResult'
void computeFollow(char sym, char *followResult) {
    // Add '$' for the start symbol
    if (sym == rules[0][0])
        setInsert(followResult, '$');
    
    // For each production, check the RHS for occurrences of sym
    for (int i = 0; i < ruleCount; i++) {
        char *rhsPart = rules[i] + 2;
        int len = strlen(rhsPart);
        for (int j = 0; j < len; j++) {
            if (rhsPart[j] == sym) {
                // If not the last symbol in the RHS
                if (rhsPart[j+1] != '\0') {
                    char tempRes[MAX_RULES] = "";
                    computeFirst(rhsPart[j+1], tempRes);
                    for (int k = 0; k < strlen(tempRes); k++) {
                        if (tempRes[k] != EPSILON)
                            setInsert(followResult, tempRes[k]);
                    }
                    if (strchr(tempRes, EPSILON)) {
                        char tempFollow[MAX_RULES] = "";
                        computeFollow(rules[i][0], tempFollow);
                        for (int k = 0; k < strlen(tempFollow); k++)
                            setInsert(followResult, tempFollow[k]);
                    }
                } else {
                    // If sym is at the end of the production and LHS is not sym.
                    if (rules[i][0] != sym) {
                        char tempFollow[MAX_RULES] = "";
                        computeFollow(rules[i][0], tempFollow);
                        for (int k = 0; k < strlen(tempFollow); k++)
                            setInsert(followResult, tempFollow[k]);
                    }
                }
            }
        }
    }
}

// Compute FOLLOW sets for all nonterminals.
void updateAllFollows() {
    for (int i = 0; i < NT_count; i++) {
        FOLLOW_set[(int)NT_list[i]][0] = '\0';
    }
    for (int i = 0; i < NT_count; i++) {
        computeFollow(NT_list[i], FOLLOW_set[(int)NT_list[i]]);
    }
}

// Populate NT_list and T_list using the productions
void discoverSymbols() {
    for (int i = 0; i < ruleCount; i++) {
        char lhs = rules[i][0];
        if (!strchr(NT_list, lhs)) {
            int len = strlen(NT_list);
            NT_list[len] = lhs;
            NT_list[len+1] = '\0';
            NT_count++;
        }
        // Scan the RHS for terminals
        for (int j = 2; rules[i][j] != '\0'; j++) {
            char sym = rules[i][j];
            if (isTerminal(sym) && sym != EPSILON && !isspace(sym)) {
                if (!strchr(T_list, sym)) {
                    int len = strlen(T_list);
                    T_list[len] = sym;
                    T_list[len+1] = '\0';
                    T_count++;
                }
            }
        }
    }
}

int main() {
    char inputLine[MAX_INPUT];
    
    printf("Enter grammar productions (one per line). End input with an empty line:\n");
    while (fgets(inputLine, sizeof(inputLine), stdin)) {
        if (inputLine[0] == '\n') break;
        inputLine[strcspn(inputLine, "\n")] = '\0';
        if (strchr(inputLine, '=')) {
            // The production is expected in format: X=...
            char LHS = inputLine[0];
            char *rhsFragment = strtok(inputLine + 2, "|");
            while (rhsFragment != NULL) {
                sprintf(rules[ruleCount], "%c=%s", LHS, rhsFragment);
                ruleCount++;
                rhsFragment = strtok(NULL, "|");
            }
        }
    }
    
    printf("Transitions Read: (%d)\n", ruleCount);
    for (int i = 0; i < ruleCount; i++) {
        printf(" %s\n", rules[i]);
    }
    
    discoverSymbols();
    
    printf("Non-Terminals Encountered: (%d)\n", NT_count);
    for (int i = 0; i < NT_count; i++) {
        printf("%c ", NT_list[i]);
    }
    printf("\nTerminals Encountered: (%d)\n", T_count);
    for (int i = 0; i < T_count; i++) {
        printf("%c ", T_list[i]);
    }
    printf("\n");
    
    updateAllFirsts();
    updateAllFollows();
    
    for (int i = 0; i < NT_count; i++) {
        char nt = NT_list[i];
        printf("First(%c) = { ", nt);
        printf("%s", FIRST_set[(int)nt]);
        printf(" }\n");
    }
    
    for (int i = 0; i < NT_count; i++) {
        char nt = NT_list[i];
        printf("Follow(%c) = { ", nt);
        printf("%s", FOLLOW_set[(int)nt]);
        printf(" }\n");
    }
    
    return 0;
}

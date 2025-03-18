# Task 4: Simulate FIRST and FOLLOW Sets of **ANY** given grammar

In this task, I wrote a C program that computes the FIRST and FOLLOW sets for any given grammar. The program reads productions from the user and assumes the following:

- Non-terminals are single uppercase letters (e.g., S, A, B).
- Terminals are any other characters (lowercase letters, digits, or symbols).
- Epsilon is represented by ^. In this code, you can use either # or ^ as input for epsilon. In the program, epsilon is stored as #, but when printing the results, epsilon is displayed as ^ for clarity.

Implementation of first and follow:

### FIRST Set Computation:

- For any terminal, FIRST(terminal) is the terminal itself.
- For a non-terminal, the program examines all productions for that non-terminal.
- Each production's right-hand side is split into alternatives (using the | delimiter). For each alternative, the program examines the symbols from left to right. It adds the terminal (or FIRST set of a non-terminal) to FIRST until it reaches a symbol that does not derive epsilon. If all symbols in the alternative can derive epsilon, then epsilon is added to FIRST.

### FOLLOW Set Computation:

- The start symbol's FOLLOW set is initialized with the end-of-input marker $.
- For every production A -> αBβ, the program adds everything in FIRST(β) (except epsilon) to FOLLOW(B). If β can derive epsilon, then - FOLLOW(A) is also added to FOLLOW(B).
- The process iterates until no more changes occur in any FOLLOW set.


As proof, here tutorial 7 question 1 and 2 answered:

```
Enter number of productions: 5
Enter production 1: S -> AB |eDa
Enter production 2: A -> ab |c
Enter production 3: B -> dC
Enter production 4: C -> eC | ^
Enter production 5: D -> fD | ^ 

=== FIRST Sets ===
FIRST(S) = { e a c }
FIRST(A) = { a c }
FIRST(B) = { d }
FIRST(C) = { e ^ }
FIRST(D) = { f ^ }

=== FOLLOW Sets ===
FOLLOW(S) = { $ }
FOLLOW(A) = { d }
FOLLOW(B) = { $ }
FOLLOW(C) = { $ }
FOLLOW(D) = { a }


Enter number of productions: 3
Enter production 1: S->daT|Rf
Enter production 2: T->aS|baT|^
Enter production 3: R->caTR|^

=== FIRST Sets ===
FIRST(S) = { d c f }
FIRST(T) = { a b ^ }
FIRST(R) = { c ^ }

=== FOLLOW Sets ===
FOLLOW(S) = { $ c f }
FOLLOW(T) = { $ c f }
FOLLOW(R) = { f }
```
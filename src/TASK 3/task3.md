#  TASK 3: YACC specification
This task consists of four subtasks in relation to generating YACC specification for syntactic categories:

- (a): Program to recognize a valid arithmetic expression that uses operator 
+, –, * and /. 
- (b): Program to recognize a valid variable which starts with a letter followed by any number of letters or digits. 
- (c): Implementation of Calculator using LEX and YACC
- (d): Convert the BNF rules into YACC form and write code to generate 
abstract syntax tree 


# A
This part uses a YACC grammar to recognise valid arithmetic expressions that use the operators ```+, -, *, and /``` (with proper precedence and associativity). When a valid expression is entered, the parser confirms its correctness.
The YACC specification can be found in A/expr.y with the lex in expr.l

SAMPLE OUTPUT
```
Enter an arithmetic expression followed by Enter:
(1 * 2) - 1
Expression parsed successfully!


Enter an arithmetic expression followed by Enter:
fds
Parse error: syntax error


```

# B
This part uses YACC and LEX to recognize a valid variable name. A valid variable starts with a letter followed by any number of letters or digits.
The YACC specification can be found in B/var.y with the lex in var.l

SAMPLE OUTPUT
```
Enter a variable name followed by Enter:
var
Valid variable: var


Enter a variable name followed by Enter:
var1
Valid variable: var1


Enter a variable name followed by Enter:
1
Error: syntax error
Error: Not a valid variable

```

# C
The calculator combines LEX and YACC to tokenize arithmetic expressions and then evaluate them.
The YACC specification can be found in C/calc.y with the lex in calc.l


SAMPLE OUTPUT
```
Enter expression:
(3 + 4) * 5
Result = 35

Enter expression:
fd + f
Error: syntax error
```

# D
In this part, BNF rules are converted into a YACC grammar that constructs an AST. The AST is then printed using helper functions.

For instance, consider this grammar for arithmetic expressions:

```
E -> E + T | E - T | T
T -> T * F | T / F | F
F -> ( E ) | id
```

For my implementation, we assume a similar grammar with numeric literals instead of id:
```
E -> E + T | E - T | T
T -> T * F | T / F | F
F -> ( E ) | NUMBER
```

AST is then built where internal nodes store operators and leaf nodes store numbers.

The YACC specification can be found in C/calc.y with the lex in calc.l. There is also two more files:
**ast.h** Declares the structure for an AST node and prototypes for functions that create new nodes and print the AST
**ast.c** Implements the functions declared in ast.h to create AST nodes and to print the AST in a parenthesized infix form

SAMPLE OUTPUT
```
Enter an arithmetic expression followed by newline:
(3 + 4) * 5
The Abstract Syntax Tree is:
((3 + 4) * 5)

Enter an arithmetic expression followed by newline:
fd
Error: syntax error
The Abstract Syntax Tree is:

```
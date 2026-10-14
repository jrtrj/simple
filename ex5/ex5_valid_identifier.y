/* Experiment 5 (part 1 of 2): valid identifier
 * Grammar:  identifier -> LETTER rest
 *           rest       -> LETTER rest | DIGIT rest | (empty)
 * Tool: Yacc (bison)
 *
 * Build:  yacc -d -o y.tab.c ex5_valid_identifier.y     (generates y.tab.h)
 *         lex ex5_valid_identifier.l
 *         gcc -o ex5 y.tab.c lex.yy.c
 * Run:    ./ex5
 */
%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
int yyerror(char *s);
%}

%token LETTER DIGIT

%%

start : identifier
        {
            printf("Valid identifier\n");
        }
      ;

identifier : LETTER rest
           ;

rest : LETTER rest
     | DIGIT rest
     |
     ;

%%

int yyerror(char *s) {
    printf("Invalid identifier\n");
    return 0;
}

int main() {
    printf("Enter an identifier: ");
    yyparse();
    return 0;
}

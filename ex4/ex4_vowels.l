/* Experiment 4: Count of vowels and consonants
 * Tool: Lex (flex)
 *
 * Build:  lex ex4_vowels.l && gcc -o ex4 lex.yy.c
 * Run:    ./ex4      (then type text, end with Ctrl-D)
 */
%{
#include <stdio.h>

int vowels = 0, consonants = 0;
%}

%%

[aeiouAEIOU]    { vowels++; }
[a-zA-Z]        { consonants++; }

.               { }

%%

int yywrap() {
    return 1;
}

int main() {
    printf("Enter the text:\n");

    yylex();

    printf("\nNumber of vowels     : %d\n", vowels);
    printf("Number of consonants : %d\n", consonants);

    return 0;
}

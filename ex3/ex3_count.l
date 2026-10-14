/* Experiment 3: Number of lines, words and characters in an input text
 * Tool: Lex (flex)
 *
 * Build:  lex ex3_count.l && gcc -o ex3 lex.yy.c
 * Run:    ./ex3      (then type text, end with Ctrl-D)
 */
%{
#include <stdio.h>

int lines = 0, words = 0, chars = 0;
%}

%%

\n          { lines++; chars++; }
[ \t]+      { chars += yyleng; }
[^ \t\n]+   { words++; chars += yyleng; }

%%

int yywrap() {
    return 1;
}

int main() {
    printf("Enter the text:\n");

    yylex();

    printf("\nNumber of lines      : %d\n", lines);
    printf("Number of words      : %d\n", words);
    printf("Number of characters : %d\n", chars);

    return 0;
}

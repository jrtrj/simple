/* Experiment 5 (part 2 of 2): valid identifier
 * Tool: Lex (flex) -- the scanner feeding the Yacc parser.
 *
 * Build: see ex5_valid_identifier.y
 */
%{
#include "y.tab.h"
%}

%%

[a-zA-Z]    { return LETTER; }

[0-9]       { return DIGIT; }

\n          { return 0; }

.           { return yytext[0]; }

%%

int yywrap() {
    return 1;
}

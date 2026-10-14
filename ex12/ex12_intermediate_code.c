/* Experiment 12: Intermediate code generation
 * Tool: C
 *
 * Converts an infix expression to postfix, then walks the postfix form
 * to emit three-address code (quadruples of the form t1 = a op b).
 *
 * Build:  gcc -o ex12 ex12_intermediate_code.c
 * Run:    ./ex12
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

int main() {
    char expr[100];
    int i;

    printf("Enter expression (e.g., a+b*c): ");
    scanf("%s", expr);

    /* ---- convert expression to postfix ---- */
    char postfix[100];
    int ptop = -1;
    char opstack[100];
    int otop = -1;

    for (i = 0; expr[i] != '\0'; i++) {
        if (isalnum((unsigned char)expr[i])) {
            postfix[++ptop] = expr[i];
        } else {
            while (otop >= 0 &&
                   precedence(opstack[otop]) >= precedence(expr[i])) {
                postfix[++ptop] = opstack[otop--];
            }
            opstack[++otop] = expr[i];
        }
    }

    while (otop >= 0)
        postfix[++ptop] = opstack[otop--];

    postfix[++ptop] = '\0';

    /* ---- generate three-address code ---- */
    char s[100][10];
    int st = -1, t = 1;

    for (i = 0; postfix[i] != '\0'; i++) {
        if (isalnum((unsigned char)postfix[i])) {
            sprintf(s[++st], "%c", postfix[i]);
        } else {
            char b[10], a[10], result[10];
            strcpy(b, s[st--]);
            strcpy(a, s[st--]);

            sprintf(result, "t%d", t);
            printf("%s = %s %c %s\n", result, a, postfix[i], b);

            strcpy(s[++st], result);
            t++;
        }
    }

    printf("Result = %s\n", s[st]);

    return 0;
}

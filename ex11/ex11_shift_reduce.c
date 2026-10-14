/* Experiment 11: Shift-reduce parser
 * Tool: C
 *
 * Grammar:
 *    E -> E + E | E * E | ( E ) | id
 *
 * Build:  gcc -o ex11 ex11_shift_reduce.c
 * Run:    ./ex11
 *
 * The parser prints every Shift and Reduce step, then Accepted or Rejected.
 */
#include <stdio.h>

char stack[100], input[100];
int top = -1, ip = 0;

void display(char action[]){
    stack[top+1] = '\0';
    printf("%-10s %-10s %s\n", stack, input + ip, action);
}

int reduce(){
    if(top >= 0 && stack[top] == 'i'){
        stack[top] = 'E';
        display("Reduce E->i");
        return 1;
    }

    /* E -> E + E. Reduce only if no tighter-binding operator is waiting in
     * the input -- that is the shift/reduce decision which gives * higher
     * precedence than +. Without this test the parser commits to (i+i)*i. */
    if(top >= 2 && stack[top-2] == 'E' && stack[top-1] == '+' &&
       stack[top] == 'E' && input[ip] != '*' && input[ip] != '/'){
        top -= 2;
        stack[top] = 'E';
        display("Reduce E->E+E");
        return 1;
    }

    if(top >= 2 && stack[top-2] == 'E' && stack[top-1] == '*' && stack[top] == 'E'){
        top -= 2;
        stack[top] = 'E';
        display("Reduce E->E*E");
        return 1;
    }

    if(top >= 2 && stack[top-2] == '(' && stack[top-1] == 'E' && stack[top] == ')'){
        top -= 2;
        stack[top] = 'E';
        display("Reduce E->(E)");
        return 1;
    }

    return 0;
}

int main(){
    printf("Enter input string: ");
    scanf("%99s", input);

    printf("\n%-10s %-10s %s\n", "Stack", "Input", "Action");
    while(input[ip] != '\0'){
        stack[++top] = input[ip++];
        display("Shift");
        while(reduce());
    }

    while(reduce());

    if(top == 0 && stack[0] == 'E')
        printf("\nAccepted\n");
    else
        printf("\nRejected\n");

    return 0;
}

/* Experiment 10: Recursive descent parser
 * Tool: C
 *
 * Grammar, with left recursion already removed:
 *    E  -> T E'
 *    E' -> + T E' | (empty)
 *    T  -> F T'
 *    T' -> * F T' | (empty)
 *    F  -> id | ( E )
 *
 * Build:  gcc -o ex10 ex10_recursive_descent.c
 * Run:    ./ex10
 */
#include <stdio.h>

char input[100];
int pos = 0, errorFlag = 0;

void E();
void Eprime();
void T();
void Tprime();
void F();

void E(){ T(); Eprime(); }

void Eprime(){
    if(input[pos]=='+'){
        pos++; T(); Eprime();
    }
}

void T(){ F(); Tprime(); }

void Tprime(){
    if(input[pos]=='*'){
        pos++; F(); Tprime();
    }
}

void F(){
    if(input[pos]=='i') pos++;
    else if(input[pos]=='('){
        pos++; E();
        if(input[pos]==')') pos++;
        else errorFlag=1;
    }
    else errorFlag=1;
}

int main(){
    printf("Enter the string: ");
    scanf("%99s",input);
    E();

    if(input[pos]!='\0') errorFlag=1;

    if(errorFlag) printf("Invalid string\n");
    else printf("Valid string\n");

    return 0;
}

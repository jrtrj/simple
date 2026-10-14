// NFA to DFA (subset construction)
// Enter one destination-set per (state, symbol) pair.

#include <stdio.h>

#define MAX 20

int n, m;
char symbols[MAX];

int nfa[MAX][MAX][MAX];

int dfaStates[MAX][MAX];
int dfaCount[MAX];
int dfa[MAX][MAX];
int totalDFA = 0;

int contains(int set[], int count, int state){
    for (int i = 0; i < count; i++){
        if (set[i] == state)
            return 1;
    }
    return 0;
}

void sortSet(int set[], int count){
    for (int i = 0; i < count - 1; i++){
        for (int j = i + 1; j < count; j++){
            if (set[i] > set[j]){
                int temp = set[i];
                set[i] = set[j];
                set[j] = temp;
            }
        }
    }
}

int sameSet(int a[], int acount, int b[], int bcount){
    if (acount != bcount)
        return 0;

    for (int i = 0; i < acount; i++){
        if (!contains(b, bcount, a[i]))
            return 0;
    }

    return 1;
}

int findDFAState(int set[], int count){
    for (int i = 0; i < totalDFA; i++){
        if (sameSet(dfaStates[i], dfaCount[i], set, count))
            return i;
    }

    return -1;
}

void printSet(int set[], int count){
    if (count == 0){
        printf("{}");
        return;
    }

    printf("{");

    for (int i = 0; i < count; i++){
        if (i > 0)
            printf(",");

        printf("q%d", set[i]);
    }

    printf("}");
}

int main(){
    printf("Enter number of states: ");
    scanf("%d", &n);

    printf("Enter number of input symbols: ");
    scanf("%d", &m);

    if (n < 1 || n > MAX || m < 1 || m > MAX){
        printf("States and symbols must each be between 1 and %d\n", MAX);
        return 1;
    }

    printf("Enter input symbols: ");
    for (int i = 0; i < m; i++)
        scanf(" %c", &symbols[i]);

    printf("\nEnter NFA transitions.\n");
    printf("For each transition, enter number of destination states followed by states.\n\n");

    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            printf("delta(q%d,%c): ", i, symbols[j]);

            int count;
            scanf("%d", &count);

            if (count < 0 || count > MAX){
                printf("Destination count must be between 0 and %d\n", MAX);
                return 1;
            }

            nfa[i][j][0] = count;

            for (int k = 1; k <= count; k++)
                scanf("%d", &nfa[i][j][k]);
        }
    }

    /* Initial DFA state = {q0} */
    dfaStates[0][0] = 0;
    dfaCount[0] = 1;
    totalDFA = 1;

    int current = 0;

    while (current < totalDFA){
        for (int j = 0; j < m; j++){
            int newSet[MAX];
            int newCount = 0;

            // Find all NFA states reachable on symbol
            for (int i = 0; i < dfaCount[current]; i++){
                int state = dfaStates[current][i];

                int transitionCount = nfa[state][j][0];

                for (int k = 1; k <= transitionCount; k++){
                    int nextState = nfa[state][j][k];

                    if (!contains(newSet, newCount, nextState)){
                        newSet[newCount++] = nextState;
                    }
                }
            }

            sortSet(newSet, newCount);

            int stateIndex = findDFAState(newSet, newCount);

            /* If this is a new DFA state */
            if (stateIndex == -1){
                if (totalDFA >= MAX){
                    printf("\nDFA state limit (%d) reached -- too many subsets.\n", MAX);
                    return 1;
                }

                stateIndex = totalDFA;

                for (int k = 0; k < newCount; k++)
                    dfaStates[totalDFA][k] = newSet[k];

                dfaCount[totalDFA] = newCount;

                totalDFA++;
            }

            dfa[current][j] = stateIndex;
        }

        current++;
    }

    printf("\nEquivalent DFA Transition Table:\n\n");

    printf("DFA State\t");
    for (int i = 0; i < m; i++)
        printf("%c\t\t", symbols[i]);

    printf("\n");

    for (int i = 0; i < totalDFA; i++){
        printSet(dfaStates[i], dfaCount[i]);
        printf("\t\t");

        for (int j = 0; j < m; j++){
            int next = dfa[i][j];

            printSet(dfaStates[next], dfaCount[next]);
            printf("\t\t");
        }
        printf("\n");
    }
    return 0;
}

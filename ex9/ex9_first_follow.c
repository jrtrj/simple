/* Experiment 9: FIRST and FOLLOW of a given grammar
 * Tool: C
 *
 * Build:  gcc -o ex9 ex9_first_follow.c
 * Run:    ./ex9
 *
 * Productions are entered as S=AB, i.e. LHS '=' RHS, with '#' for epsilon.
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int n;
char productions[10][20];
char first[10][20];
char follow[10][20];

void addToSet(char set[], char c) {
    if (strchr(set, c) == NULL) {
        int len = strlen(set);
        set[len] = c;
        set[len + 1] = '\0';
    }
}

void findFirst(char c, int index);
void findFollow(char c, int index);

/* Index of the first symbol of the right-hand side: skip the left-hand side
 * and whatever separates it from the right-hand side. Accepts both S=AB and
 * S->AB, so typing the other separator cannot silently produce nonsense. */
int rhsStart(char p[]) {
    int k = 1;
    while (p[k] != '\0' && (p[k] == '=' || p[k] == '-' || p[k] == '>' || p[k] == ' '))
        k++;
    return k;
}

void findFirst(char c, int index) {
    int i, j;

    if (!isupper(c)) {
        addToSet(first[index], c);
        return;
    }

    for (i = 0; i < n; i++) {
        if (productions[i][0] == c) {
            char symbol = productions[i][rhsStart(productions[i])];

            if (!isupper(symbol)) {
                addToSet(first[index], symbol);
            } else {
                int k;
                for (k = 0; k < n; k++) {
                    if (productions[k][0] == symbol) {
                        findFirst(symbol, k);
                        for (j = 0; j < (int)strlen(first[k]); j++)
                            addToSet(first[index], first[k][j]);
                    }
                }
            }
        }
    }
}

/* Does FIRST(X) contain '#'? That is, can X derive the empty string? */
int hasEpsilon(char X) {
    int k, y;
    for (k = 0; k < n; k++) {
        if (productions[k][0] == X) {
            for (y = 0; first[k][y] != '\0'; y++)
                if (first[k][y] == '#')
                    return 1;
        }
    }
    return 0;
}

void findFollow(char c, int index) {
    int i, j, k, x, y;

    if (productions[0][0] == c)
        addToSet(follow[index], '$');

    for (i = 0; i < n; i++) {
        int len = strlen(productions[i]);
        int start = rhsStart(productions[i]);

        for (j = start; j < len; j++) {
            if (productions[i][j] != c)
                continue;

            /* Walk the WHOLE rest of this right-hand side, not just the next
             * symbol. Everything that can begin the suffix belongs in FOLLOW(c).
             * If the entire suffix can vanish, FOLLOW of the left-hand side
             * flows in as well -- that is the case a single-symbol check
             * misses, and it is what goes wrong on E=TX with X nullable. */
            int nullable = 1;
            for (k = j + 1; k < len && nullable; k++) {
                char next = productions[i][k];

                if (!isupper(next)) {
                    addToSet(follow[index], next);
                    nullable = 0;              /* a terminal cannot vanish */
                } else {
                    for (x = 0; x < n; x++) {
                        if (productions[x][0] == next) {
                            for (y = 0; first[x][y] != '\0'; y++)
                                if (first[x][y] != '#')
                                    addToSet(follow[index], first[x][y]);
                        }
                    }
                    nullable = hasEpsilon(next);
                }
            }

            if (nullable && productions[i][0] != c) {
                for (k = 0; k < n; k++) {
                    if (productions[k][0] == productions[i][0]) {
                        findFollow(productions[i][0], k);
                        for (y = 0; follow[k][y] != '\0'; y++)
                            addToSet(follow[index], follow[k][y]);
                    }
                }
            }
        }
    }
}

int main() {
    int i;

    printf("Enter number of productions: ");
    scanf("%d", &n);

    if (n < 1 || n > 10) {
        printf("Number of productions must be between 1 and 10\n");
        return 1;
    }

    printf("Enter productions (Example: S=AB):\n");
    for (i = 0; i < n; i++) {
        scanf("%s", productions[i]);
        first[i][0] = '\0';
        follow[i][0] = '\0';
    }

    for (i = 0; i < n; i++)
        findFirst(productions[i][0], i);

    for (i = 0; i < n; i++)
        findFollow(productions[i][0], i);

    printf("\nFIRST Sets:\n");
    for (i = 0; i < n; i++)
        printf("FIRST(%c) = { %s }\n", productions[i][0], first[i]);

    printf("\nFOLLOW Sets:\n");
    for (i = 0; i < n; i++)
        printf("FOLLOW(%c) = { %s }\n", productions[i][0], follow[i]);

    return 0;
}

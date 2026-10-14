/* Experiment 7 (CORRECTED): convert an NFA with epsilon transitions to an NFA
 * without them.
 *
 * The version in the class record omits the final epsilon-closure step, so its
 * answers are incomplete. The difference is one extra pass, marked below.
 *
 * The correct construction is:
 *
 *     delta'(q, a) = eps-closure( delta( eps-closure(q), a ) )
 *
 * Build:  gcc -o ex7fixed ex7_epsilon_nfa_to_nfa_fixed.c
 * Run:    ./ex7fixed
 */
#include <stdio.h>

int main() {
    int n, e, t;
    int eps[10][10] = {0};
    int trans[10][10] = {0};
    int target[10], tcount, seen, x;
    int i, j, k, from, to;

    printf("Enter number of states: ");
    scanf("%d", &n);

    printf("Enter number of epsilon transitions: ");
    scanf("%d", &e);

    printf("Enter epsilon transitions (from to):\n");
    for (i = 0; i < e; i++) {
        scanf("%d %d", &from, &to);
        eps[from][to] = 1;
    }

    printf("Enter number of 'a' transitions: ");
    scanf("%d", &t);

    printf("Enter 'a' transitions (from to):\n");
    for (i = 0; i < t; i++) {
        scanf("%d %d", &from, &to);
        trans[from][to] = 1;
    }

    /* ---- step 1: reflexive, transitive closure of the epsilon moves ---- */
    for (i = 0; i < n; i++) {
        eps[i][i] = 1;                       /* a state reaches itself */
        for (j = 0; j < n; j++) {
            if (eps[i][j]) {
                for (k = 0; k < n; k++) {
                    if (eps[j][k])
                        eps[i][k] = 1;
                }
            }
        }
    }

    printf("\nNFA without epsilon transitions:\n");

    for (i = 0; i < n; i++) {
        printf("q%d --a--> ", i);

        /* ---- step 2a: collect delta( eps-closure(q), a ) ---- */
        tcount = 0;
        for (j = 0; j < n; j++) {
            if (eps[i][j]) {
                for (k = 0; k < n; k++) {
                    if (trans[j][k]) {
                        seen = 0;
                        for (x = 0; x < tcount; x++)
                            if (target[x] == k) seen = 1;
                        if (!seen) target[tcount++] = k;
                    }
                }
            }
        }

        /* ---- step 2b: take the eps-closure of that set.
                tcount grows inside the loop, so this walks the whole
                newly reachable frontier -- the same worklist trick as
                experiment 6. This is the step the record's version omits. ---- */
        for (j = 0; j < tcount; j++) {
            for (k = 0; k < n; k++) {
                if (eps[target[j]][k]) {
                    seen = 0;
                    for (x = 0; x < tcount; x++)
                        if (target[x] == k) seen = 1;
                    if (!seen) target[tcount++] = k;
                }
            }
        }

        for (j = 0; j < tcount; j++)
            printf("q%d ", target[j]);
        printf("\n");
    }

    return 0;
}

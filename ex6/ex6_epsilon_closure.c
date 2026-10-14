/* Experiment 6: E-closure of all states of an NFA with epsilon transitions
 * Tool: C
 *
 * Input is the n x n epsilon-transition matrix: a 1 at row i column j means
 * there is an epsilon transition from qi to qj.
 *
 * Build:  gcc -o ex6 ex6_epsilon_closure.c
 * Run:    ./ex6
 */
#include <stdio.h>

int main()
{
    int n, i, j, k, pass;
    int a[10][10], visited[10];

    printf("Enter number of states: ");
    scanf("%d", &n);

    printf("Enter epsilon transition matrix:\n");
    for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
            scanf("%d", &a[i][j]);

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
            visited[j] = 0;

        /* A state always belongs to its own epsilon closure */
        visited[i] = 1;

        /* Relax reachability. A single pass only marks immediate neighbours,
         * so a chain q0 -> q1 -> q2 would stop at q1. Repeating the pass n
         * times guarantees the transitive closure. */
        for(pass = 0; pass < n; pass++)
            for(j = 0; j < n; j++)
                for(k = 0; k < n; k++)
                    if(a[j][k] == 1 && visited[j] == 1)
                        visited[k] = 1;

        printf("E-closure(q%d) = { ", i);

        for(j = 0; j < n; j++)
        {
            if(visited[j] == 1)
                printf("q%d ", j);
        }

        printf("}\n");
    }

    return 0;
}

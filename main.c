#include <stdio.h>
#include "IDA.h"
#include <time.h>

#define BUF 20

int main() {
    setbuf(stdout, 0);
    int n = 4;

    int mattr[4][4];

    printf("Enter file name to get grid: ");
    char f_name[BUF];
    gets(f_name);
    get_grid_from_txt_file(f_name, &mattr, n);

    if (error) {
        printf("Reading error, incorrect input.\n");
        return -1;
    }

    printf("\n - - I N P U T T E D    M A T T R - - \n");
    output_grid(&mattr, 4);

    if (solvable(&(mattr), n)) {
        printf("\n***  YOUR SOLUTION (A*)  ***\n");
        clock_t begin = clock();
        solveIDAStar(&mattr, n);
        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("\n\nTIME SPENT: %f", time_spent);
    } else {
        printf("No solutions.");
    }

    return 0;
}
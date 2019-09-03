//
// Spandan Das
//
#include <stdio.h>
#include <stdlib.h>
#define M 30
#define N 40

int show(char g[][N]) ;
double r() {
    return rand() * 1.0 / RAND_MAX;
}
void fill(char* a, double p, int n) {
    for (int j = 0; j < n; j++) {
        if (r() < p) {
            a[j] = 'X';
        }
        else {
            a[j] = ' ';
        }
    }
}

void fill2(char g[][N], double p) {
    //when you pass a 2d array as an argument,
    //you have to provide the second dimension
    for (int j = 0; j < M; j++) {
        for (int k = 0; k < N; k++) {
            if (r() < p) {
                g[j][k] = 'X'; // on
            }
            else {
                g[j][k] = ' '; // off
            }
        }
    }
}

int main()
{
    srand(1509919);
    char array[M*N];
    array[5*40 + 25] = 'T';
    //char grid[M][N];
    //grid[5][25] = 'T'; // does same thing as above
    fill(array, 0.60, 1200);
    int sum = 0;
    for (int i = 0; i < 100; i++) {
        char grid[M][N];
        fill2(grid, 0.60);
        sum += show(grid);
    }
    printf("sum = %d\n", sum);
    return 0;
}

int show(char g[][N]) {
    int count = 0;
    int countFirst = 0;
    int countNext = 0;
    int countBoth = 0;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            //printf("%c", g[i][j]);
            if (g[i][j] == 'X') {
                count++;
//                if (j == 0) countFirst++;
//                if (j == 1) {
//                    countNext++;
//                    if (g[i][j-1] == 'X') countBoth++;
//                }
            }
        }
        //printf("\n");
    }
    //printf("The count is %d\n", count);
    //printf("The countFirst is %d\n", countFirst);
    //printf("The countNext is %d\n", countNext);
    //printf("The countBoth is %d\n", countBoth);
    return count;
}
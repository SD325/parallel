#include <stdio.h>
#include <stdlib.h>
#define M 30
#define N 40

void show(char g[][N]) ;
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
    srand(7);
    char array[M*N];
    array[5*40 + 25] = 'T';
    char grid[M][N];
    grid[5][25] = 'T'; // does same thing as above
    fill(array, 0.60, 1200);
    fill2(grid, 0.60);
    show(grid);
    return 0;
}

void show(char g[][N]) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            printf("%c", g[i][j]);
        }
        printf("\n");
    }
}
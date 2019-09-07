//
// Created by Spandan Das on 9/5/2019.
//
#include <stdio.h>
#include <stdlib.h>
#define M 30
#define N 40
#define MAX M*N

// implement queue using an array

int time = 0;
int a[MAX];
int front = 0;
int back = -1;
int total = 0;
int time_marker = 0;

void add(int x) {
    a[++back] = x;
    total++;
}

int deque() {
    total--;
    if (front + 1 == time_marker) {
        time++;
        time_marker = back;
    }
    return a[front++];
}


int show(char g[][N]) ;
double r() {
    return rand() * 1.0 / RAND_MAX;
}

void fill(char g[][N], double p) {
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

int fire(char g[][N]) {
    for (int i = 0; i < M; i++) {
        if (g[i][0] == 'X')  {
            add(i*N); //add to queue
            g[i][0] = '*';
            //printf("%d ", i);
        }
    }
    time_marker = back;

    setbuf(stdout, 0); // useful for debug
    while (total != 0) {
        int ind = deque();
        int x = ind/N;
        int y = ind%N;
        //printf("%d   xy: %d, %d \n", ind, x, y);
        int delta_x[] = {1, -1, 0, 0};
        int delta_y[] = {0, 0, 1, -1};
        for (int i = 0; i < 4; i++) {
            int x_new = x + delta_x[i];
            int y_new = y + delta_y[i];
            if ((x_new >= 0) && (x_new < M) && (y_new >= 0) && (y_new < N) && g[x_new][y_new] == 'X') {
                add(x_new*N + y_new);
                g[x_new][y_new] = '*';
            }
        }
    }
    show(g);
    return time;
}

int main()
{
    srand(1509919);
    char grid[M][N];
    fill(grid, 0.60);
    //show(grid);
    printf("time: %d", fire(grid));
    //show(grid);
    return 0;
}

int show(char g[][N]) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            printf("%c", g[i][j]);
        }
        printf("\n");
    }
    //printf("\n");
}
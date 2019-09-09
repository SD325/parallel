//
// Created by Spandan Das on 9/5/2019.
//
#include <stdio.h>
#include <stdlib.h>
#define M 30
#define N 40
#define MAX M*N

// implement queue using an array

int a[MAX];
int front = 0;
int back = -1;
int total = 0;

void reset() {
    for (int i = 0; i < M*N; i++) {
        a[i] = 0;
    }
    front = 0;
    back = -1;
    total = 0;
}

void add(int x) {
    a[++back] = x;
    total++;
}

int deque() {
    total--;
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
    int time = 0;
    for (int i = 0; i < M; i++) {
        if (g[i][0] == 'X')  {
            add(i*N); //add to queue
            g[i][0] = '*';
            //printf("%d ", i);
        }
    }
    int time_step = back;

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
        if (total != 0 && front > time_step) {
            time_step = back;
            time++;
            //printf("Time %d: \n", time);
            //show(g);
            //printf("___________________________________________________\n");
        }
    }
    //show(g);
    return time;
}

int main() {
    srand(1509919);
    int t = 10;
    for (double p = 0.01; p <= 1.00; p += 0.01) {
        double norm_sum = 0;
        for (int i = 0; i < t; i++) {
            char grid[M][N];
            fill(grid, p);
            int burnout = fire(grid);
            norm_sum += burnout / (double) N;
            reset();
            //printf("time: %d\n", burnout);
            //printf("normalized: %f\n", norm);
        }
        double avg_norm = norm_sum / (double) t;
        printf("%f, %f \n", p, avg_norm);
    }
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
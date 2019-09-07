#include <stdio.h>
//#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define M 30
#define N 40
#define MAX M*N

// implement queue using an array

int a[MAX];
int front = 0;
int back = -1;
int total = 0;

void add(int x) {
    a[++back] = x;
    total++;
}

int deque() {
    total--;
    return a[front++];
}

bool isEmpty() {
    return total == 0;
}

double r() {
    return rand() * 1.0 / RAND_MAX;
}

int main() {
    add(1);
    add(2);
    add(3);
    add(4);
    add(5);
    deque();
    for (int i = front; i <= back; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}
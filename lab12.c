//
// Spandan Das, 23 September 2019
//
// mpicc mpiDemo.c
// mpirun -np 4 -mca btl ^openib a.out
//
// Manager-Worker model for parallel processing.
//
// time ... real ... user
//
// htop
//
#include <stdio.h>
#include <stdlib.h>
//
#include "mpi.h"
//
#define M 640
#define N 480

#define min_y -0.166906738281
#define max_y 1.04157104492
#define min_x -0.167883300781
#define max_x 1.04083862305

#define max_iter 1000
#define scale 15.0
static int rgb[N][M][3] ; // red-green-blue for each pixel
static int iter_counts[N][M];
static int num_iter_per_pixel[max_iter];

void color(int y, int x, int r, int g, int b) {
    rgb[y][x][0] = r   ; // red
    rgb[y][x][1] = g   ; // green
    rgb[y][x][2] = b   ; // blue
}

int check(int py, int px) {
    double x_temp;
    double y0 = (double)py*(max_y - min_y)*2.0/N - (max_y - min_y);
    double x0 = (double)px*(max_x - min_x)*2.0/M - (max_x - min_x);
    double x = 0.0;
    double y = 0.0;
    for (int i = 0; i < max_iter; i++) {
        if (x*x + y*y > 4) return i;
        x_temp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = x_temp;
    }
    // return max_iter-1;
    return 0;
}

int main(int argc, char *argv[]) {
    //
    // MPI variables
    //
    int size; // same
    int rank; // different
    //
    MPI_Status status;
    int tag = 0; // same!
    //
    // other variables
    //
    int k, j;
    double result;
    //
    // boilerplate
    //
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //
    srand(rank); // different!
    //
    // manager has rank = 0
    //
    int y_rec, x_rec;
    if (rank == 0) {
        FILE* fout ;

        for (k = 0; k < M*N; k++) {
            MPI_Recv(&result, 1, MPI_DOUBLE, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
            j = status.MPI_SOURCE; // worker number
            MPI_Recv(&y_rec, 1, MPI_DOUBLE, j, tag, MPI_COMM_WORLD, &status);
            MPI_Recv(&x_rec, 1, MPI_DOUBLE, j, tag, MPI_COMM_WORLD, &status);
            iter_counts[y_rec][x_rec] = result;
        }

        for( int y = 0 ; y < N ; y++ ) {
            for( int x = 0 ; x < M ; x++) {
                num_iter_per_pixel[iter_counts[y][x]]++;
            }
        }

        int total = 0;
        for (int i = 0; i < max_iter; i++) total += num_iter_per_pixel[i];

        int col = 0;
        for( int y = 0 ; y < N ; y++ ) {
            for( int x = 0 ; x < M ; x++) {
                col = (int) (iter_counts[y][x] * 255. * scale / (double) max_iter);
                color(y, x, col, col, col);
            }
        }


        fout = fopen( "mandelbrot.ppm" , "w" ) ;

        fprintf( fout , "P3\n" ) ;
        fprintf( fout , "%d %d\n" , M , N ) ;
        fprintf( fout , "255\n" ) ;
        //
        for( int y = 0 ; y < N ; y++ )
        {
            for( int x = 0 ; x < M ; x++)
            {
                fprintf( fout , "%d %d %d\n" ,
                         rgb[y][x][0] , rgb[y][x][1] , rgb[y][x][2] ) ;
            }
        }
        fclose( fout ) ;

    }
        //
        // workers have rank > 0
        //
    else {
        int interval_width = (int) (M / (size-1));
        int x_max = interval_width * rank;
        int x_min = x_max - interval_width;
        int iter_count_send;
        for (int py = 0 ; py < N ; py++ ) {
            for (int px = x_min ; px < x_max ; px++) {
                iter_count_send = check(py, px);
                // order: iterations, y, x
                MPI_Send(&iter_count_send, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
                MPI_Send(&py, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
                MPI_Send(&px, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
            }
        }
    }
    //
    // boilerplate
    //
    MPI_Finalize();
    //
    return 0;
}
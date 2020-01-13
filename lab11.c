#include <stdio.h>
//
#define M 640
#define N 480

#define max_iter 1000
#define scale 15.0
static int rgb[N][M][3] ; // red-green-blue for each pixel
static int iter_counts[N][M];
static int num_iter_per_pixel[max_iter];
// static double hue[N][M];
//

void color(int y, int x, int r, int g, int b) {
    rgb[y][x][0] = r   ; // red
    rgb[y][x][1] = g   ; // green
    rgb[y][x][2] = b   ; // blue
}

int check(int py, int px) {
    double x_temp;
    double y0 = (double)py*4./N - 2;
    double x0 = (double)px*4./M - 2;
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

int main(void) {
    FILE* fout ;
    //
    for (int py = 0 ; py < N ; py++ ) {
        for (int px = 0 ; px < M ; px++) {
//            if (check(py, px) == 0) color(py, px, 255, 255, 255);
//            else color(py, px, 0, 0, 0);
            iter_counts[py][px] = check(py, px);
        }
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
//            for (int i = 0 ; i <= iter_counts[y][x]; i++) {
//                hue[y][x] += ((double) num_iter_per_pixel[i] / (double) total);
//            }
//            color(y, x, (int) (hue[y][x]*255.0), (int) (hue[y][x]*255.0), (int) (hue[y][x]*255.0));
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
    //
    return 0 ;
}
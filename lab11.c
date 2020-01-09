#include <stdio.h>
//
#define M 640
#define N 480

#define max_iter 100
static int rgb[N][M][3] ; // red-green-blue for each pixel
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
        if (x*x + y*y > 4) return 0;
        x_temp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = x_temp;
    }
    return 1;
}

int main(void) {
    FILE* fout ;
    //
    for (int py = 0 ; py < N ; py++ ) {
        for (int px = 0 ; px < M ; px++) {
            if (check(py, px) == 0) color(py, px, 255, 255, 255);
            else color(py, px, 0, 0, 0);
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
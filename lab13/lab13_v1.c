#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
//
#define M 640
#define N 480

static int rgb[N][M][3];
typedef struct
{
    double x ;
    double y ;
    double z ;
    //
} triple ;

typedef struct
{
    int r ;
    int g ;
    int b ;
    //
} hue ;

typedef struct
{
    triple c ;
    hue h ;
    double r ;
} sphere ;
static sphere a[4];
//
triple e = { 0.50 , 0.50 , -1.00 } ; // the eye
triple g = { 0.00 , 1.25 , -0.50 } ; // the light
//
double dotp( triple t , triple u ) {
    return t.x * u.x + t.y * u.y + t.z * u.z ;
}
void norm(triple t) {
    double mag = sqrt(dotp(t, t));
    t.x /= mag;
    t.y /= mag;
    t.z /= mag;
}
//
void diff( triple* t , triple u , triple v ) // t = u - v
{
    t->x = u.x - v.x ;
    t->y = u.y - v.y ;
    t->z = u.z - v.z ;
}

hue getHue(int y , int x) {
    double px = ((double)x + 0.5) / N - 1./6.;
    double py = ((double)y + 0.5) / N;
    double a_, b_, c_;
    triple r = {px - e.x, py - e.y, 0.0 - e.z};
    double T1, T2;
    bool none = true;
    double minPos_T = INFINITY;
    double rsq = dotp(r, r);
    hue h = {0, 0, 0};
    for (int i = 0; i < 4; i++) {
        triple d = {e.x - a[i].c.x, e.y - a[i].c.y, e.z - a[i].c.z};
        a_ = 1.0;
        b_ = 2.0 * dotp(r, d) / rsq;
        c_ = (dotp(d, d) - a[i].r * a[i].r) / rsq;
        T1 = (-b_ + sqrt(b_*b_ - 4.0 * a_ * c_)) / (2 * a_);
        T2 = (-b_ - sqrt(b_*b_ - 4.0 * a_ * c_)) / (2 * a_);
        if (T1 > 0 && T2 > 0) {
            if (__min(T1, T2) < minPos_T) {
                h.r = a[i].h.r;
                h.g = a[i].h.g;
                h.b = a[i].h.b;
            }
            minPos_T = __min(minPos_T, __min(T1, T2));
            if (none) none = false;
        }
        else if (T1 > 0) {
            if (T1 < minPos_T) {
                h.r = a[i].h.r;
                h.g = a[i].h.g;
                h.b = a[i].h.b;
            }
            minPos_T = __min(minPos_T, T1);
            if (none) none = false;
        }
        else if (T2 > 0) {
            if (T2 < minPos_T) {
                h.r = a[i].h.r;
                h.g = a[i].h.g;
                h.b = a[i].h.b;
            }
            minPos_T = __min(minPos_T, T2);
            if (none) none = false;
        }
    }
    if (none) {
        h.r = 178;
        h.g = 255;
        h.b = 255;
    }
    return h;
}

//
void init()
{
    a[0].c.x =      0.50 ;
    a[0].c.y = -20000.00 ; // the floor
    a[0].c.z =      0.50 ;
    a[0].r   =  20000.25 ;
    a[0].h.r =    205    ; // color is Peru
    a[0].h.g =    133    ;
    a[0].h.b =     63    ;
    //
    a[1].c.x =      0.50 ;
    a[1].c.y =      0.50 ;
    a[1].c.z =      0.50 ;
    a[1].r   =      0.25 ;
    a[1].h.r =      0    ; // color is Blue
    a[1].h.g =      0    ;
    a[1].h.b =    255    ;
    //
    a[2].c.x =      1.00 ;
    a[2].c.y =      0.50 ;
    a[2].c.z =      1.00 ;
    a[2].r   =      0.25 ;
    a[2].h.r =      0    ; // color is Green
    a[2].h.g =    255    ;
    a[2].h.b =      0    ;
    //
    a[3].c.x =      0.00 ;
    a[3].c.y =      0.75 ;
    a[3].c.z =      1.25 ;
    a[3].r   =      0.50 ;
    a[3].h.r =    255    ; // color is Red
    a[3].h.g =      0    ;
    a[3].h.b =      0    ;
}
int main(int argc, char *argv[])
{
    int y , x ;
    hue temp;
    //
    FILE* fout ;
    //
    init();
    for( y = 0 ; y < N ; y++ )
    {
        for( x = 0 ; x < M ; x++)
        {
            temp = getHue(y, x);
            rgb[N - 1 - y][x][0] = temp.r   ;
            rgb[N - 1 - y][x][1] = temp.g   ;
            rgb[N - 1 - y][x][2] = temp.b   ;
        }
    }
    //
    //
    //
    fout = fopen( "ray_trace_1.ppm" , "w" ) ;
    //
    fprintf( fout , "P3\n" ) ;
    fprintf( fout , "%d %d\n" , M , N ) ;
    fprintf( fout , "255\n" ) ;
    //
    for( y = 0 ; y < N ; y++ )
    {
        for( x = 0 ; x < M ; x++)
        {
            fprintf( fout , "%d %d %d\n" , rgb[y][x][0] , rgb[y][x][1] , rgb[y][x][2] ) ;
        }
    }
    fclose( fout ) ;
    //
    return 0 ;
}
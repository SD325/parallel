//
// Spandan Das, 21 October 2019
//
#include <stdio.h>
#include <math.h>
//
// https://physics.nist.gov/cgi-bin/cuu/Value?bg
//
#define G 6.674e-11 // (m^3)(kg^-1)(s^-2)
//
// https://ssd.jpl.nasa.gov/?planet_phys_par
//
#define M 7.349e+22 // kg
#define R 1.7374e+6 // m
#define V 1023.157  // m/s

//
// parameters
//
#define DT 100    // s
//
int main()
{
    //
    // time intervals - duration is 90 minutes
    //
    int n = (int)( 0.5 + ( 29 * 24.0 * 60 * 60 ) / DT ) ;
    //
    //////////////////////////////////////////////////
    //
    double  t[n] ;
    double  x[n] ;
    double  y[n] ;
    double vx[n] ;
    double vy[n] ;
    //
    //////////////////////////////////////////////////
    //
    FILE*  fout  ;
    //
    int    j     ;
    //
    double r = 3.844e8 ;
    double a ;
    //
    //////////////////////////////////////////////////
    //
    // International Space Station
    //
    // https://www.nasa.gov/sites/default/files/atoms/files/np-2015-05-022-jsc-iss-guide-2015-update-111015-508c.pdf
    //
    // Page 54 - altitude : 370 km to 460 km
    // Page 54 - speed    : 28,000 km per hour
    //
    t[0]  =          0.0 ;
    x[0]  =          r ;
    y[0]  =         0.0 ;
    vx[0] =         0.0 ;
    vy[0] =          V;

    // for hyperbolic:
//    x[0] = -25292115.2221577350000000;
//    y[0] = -9182197.7074789144000000;
//    vx[0] = 4640.4073870538150000;
//    vy[0] = 4764.7461535995972000;
    //
    //////////////////////////////////////////////////
    //
    // elliptical
    //vx[0] *= 1.2;
    //hyperbolic
    //vx[0] *= 1.5;

    for( j = 1 ; j < n ; j ++ )
    {
        t[j] = t[j-1] + DT           ;
        //
        x[j] = x[j-1] + DT * vx[j-1] ;
        y[j] = y[j-1] + DT * vy[j-1] ;
        //
        // calculate a
        r = sqrt(x[j]*x[j] + y[j]*y[j]);
        a = (M*G)/(r*r);
        //
        // update vx
        vx[j] = vx[j-1] - DT * (x[j] * a) / r;
        // update vy
        vy[j] = vy[j-1] - DT * (y[j] * a) / r;
        //
    }
    //
    //////////////////////////////////////////////////
    //
    fout = fopen( "orbit2.csv" , "w" ) ;
    //
    double dist, speed;
    fprintf(fout, "Index, Time, X, Y, Distance, Speed, vx, vy\n");
    for( j = 0 ; j < n ; j ++ )
    {
        dist = sqrt(x[j]*x[j] + y[j]*y[j]);
        speed = sqrt(vx[j]*vx[j] + vy[j]*vy[j]);
        fprintf( fout , "%d, %0.16f, %0.16f, %0.16f, %0.16f, %0.16f, %0.16f, %0.16f\n" , j , t[j] , x[j] , y[j], dist, speed, vx[j], vy[j]) ;
        //fprintf( fout , "%d, %0.16f, %0.16f, %0.16f\n" , j, t[j] , dist, speed);
        //
        // what else to print ?
        //
    }
    //
    fclose( fout ) ;
    //
    return 0 ;
}
//
// end of file
//
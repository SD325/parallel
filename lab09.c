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
#define ME 5.972e+24 // kg
#define M 7.349e+22 // kg    // mass of moon
#define R 1.7374e+6 // m     // radius of moon
#define V 1023.157  // m/s
#define RE 6.371e+6  // m

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
    int n = (int)( 0.5 + ( 4.0 * 24 * 60 * 60) / DT ) ;
    //
    //////////////////////////////////////////////////
    //
    double  t[n] ;
    double  x[n] ;
    double  y[n] ;
    double vx[n] ;
    double vy[n] ;

    // double  ts[n] ;
    double  xs[n] ;
    double  ys[n] ;
    double vxs[n] ;
    double vys[n] ;
    //
    //////////////////////////////////////////////////
    //
    FILE*  fout  ;
    //
    int    j     ;
    //
    double dist_to_moon_from_earth = 3.844e8 ;
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
    x[0]  =          dist_to_moon_from_earth ;
    y[0]  =         0.0 ;
    vx[0] =         0.0 ;
    vy[0] =          V;

    // satellite
    double theta = 3.14159265/6.4;
    //ts[0]  =          0.0 ;
    vxs[0] = 1527.048 * cos(theta);
    vys[0] = 1527.048 * sin(theta);
    xs[0]  = (RE + 202751774.4 ) * cos(theta);
    ys[0]  = (RE + 202751774.4 ) * sin(theta);
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
    double r;
    for( j = 1 ; j < n ; j ++ )
    {
        t[j] = t[j-1] + DT           ;
        //
        x[j] = x[j-1] + DT * vx[j-1] ;
        y[j] = y[j-1] + DT * vy[j-1] ;
        //
        // calculate a
        r = sqrt(x[j]*x[j] + y[j]*y[j]);
        a = (ME*G)/(r*r);
        //
        // update vx
        vx[j] = vx[j-1] - DT * (x[j] * a) / r;
        // update vy
        vy[j] = vy[j-1] - DT * (y[j] * a) / r;

        // Satellite
        xs[j] = xs[j-1] + DT * vxs[j-1] ;
        ys[j] = ys[j-1] + DT * vys[j-1] ;
        r=sqrt(xs[j]*xs[j]+ys[j]*ys[j]);
        double rm = sqrt((xs[j]-x[j])*(xs[j]-x[j]) + (ys[j] - y[j])*(ys[j] - y[j]));
        double as_m = (M*G)/(rm*rm);
        double as_e = (ME*G)/(r*r);
        vxs[j] = vxs[j-1] + DT*(-xs[j]*as_e/r-(xs[j]-x[j])*as_m/rm);
        vys[j] = vys[j-1] + DT*(-ys[j]*as_e/r-(ys[j]-y[j])*as_m/rm);

    }
    //
    //////////////////////////////////////////////////
    //
    fout = fopen( "orbit2.csv" , "w" ) ;
    //
    double dist, speed, sat_speed;
    fprintf(fout, "Index, Time, X, Y, Distance, Speed, vx, vy, X_s, Y_s, Sat_Speed, vxs, vys\n");
    for( j = 0 ; j < n ; j ++ )
    {
        dist = sqrt(x[j]*x[j] + y[j]*y[j]);
        speed = sqrt(vx[j]*vx[j] + vy[j]*vy[j]);
        sat_speed = sqrt(vxs[j]*vxs[j] + vys[j]*vys[j]);
        fprintf( fout , "%d, %0.16f, %0.16f, %0.16f, %0.16f, %0.16f, %0.16f, %0.16f, %0.16f, %0.16f, %0.16f, %0.16f, %0.16f\n" , j , t[j] , x[j] , y[j], dist, speed, vx[j], vy[j], xs[j], ys[j], sat_speed, vxs[j], vys[j]) ;
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
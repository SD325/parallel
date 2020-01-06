//
// Spandan Das, 21 October 2019
//
#include <stdio.h>
#include <math.h>
//
#include "mpi.h"
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
    int k, worker_num;
    int result[3];
    //
    // boilerplate
    //
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //
    //srand(rank); // different!
    //
    // manager has rank = 0
    //
    if (rank == 0) {
        //printf("\n");
        //
        for (k = 1; k < size; k++) {
            MPI_Recv(*result, 1, MPI_DOUBLE, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
            //
            worker_num = status.MPI_SOURCE; // worker number
            //
            printf("%d: %d %d %d\n", worker_num, result[0], result[1], result[2]);
        }
        //
        //printf("\n");
    }
        //
        // workers have rank > 0
        //
    else {
        int each_range = 1000 / (size-1);
        int speed;
        int id;
        for (int angleDeg = 181; angleDeg <= 269; angleDeg++) {
            int curr_result[] = {0, (int) angleDeg, 0};
            for (speed = 1500; speed < 2500; speed += 10) {
                id = angleDeg*1000+speed;
                curr_result[2] = speed;
                int n = (int)( 0.5 + ( 6.0 * 24 * 60 * 60) / DT ) ;
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
                // FILE*  fout  ;
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
                x[0]  =          dist_to_moon_from_earth;
                y[0]  =         0.0 ;
                vx[0] =         0.0 ;
                vy[0] =          V;

                // satellite

                // need to guess and check
                double angle = angleDeg*M_PI/180.;
                double spd_guess = speed;
                //ts[0]  =          0.0 ;
                vxs[0] = spd_guess * cos(angle);
                vys[0] = spd_guess * sin(angle);
                xs[0]  = x[0] + 2*R * cos(angle);
                ys[0]  = y[0] + 2*R * sin(angle);
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
            }
            MPI_Send(*curr_result, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
        }
    }
    //
    // boilerplate
    //
    MPI_Finalize();

    //
    return 0 ;
}
//
// end of file
//

/*
 * //

    //
    //////////////////////////////////////////////////
    //
    fout = fopen( "orbit2.csv" , "w" ) ;
    //
    double dist, speed, sat_speed, sat_dist_to_moon;
    fprintf(fout, "Index, Time, X, Y, Distance, Speed, vx, vy, X_s, Y_s, Sat_Speed, vxs, vys, Dist_to_Moon\n");
    for( j = 0 ; j < n ; j ++ )
    {
        sat_dist_to_moon = sqrt((x[j]-xs[j])*(x[j]-xs[j])+(y[j]-ys[j])*(y[j]-ys[j]));
        dist = sqrt(x[j]*x[j] + y[j]*y[j]);
        speed = sqrt(vx[j]*vx[j] + vy[j]*vy[j]);
        sat_speed = sqrt(vxs[j]*vxs[j] + vys[j]*vys[j]);
        fprintf( fout , "%d, %0.16f, %0.16f, %0.16f, %0.16f, %0.16f, %0.16f, %0.16f, %0.16f, %0.16f, %0.16f, %0.16f, %0.16f, %0.16f\n" , j , t[j] , x[j] , y[j], dist, speed, vx[j], vy[j], xs[j], ys[j], sat_speed, vxs[j], vys[j], sat_dist_to_moon) ;
        //fprintf( fout , "%d, %0.16f, %0.16f, %0.16f\n" , j, t[j] , dist, speed);
        //
        // what else to print ?
        //
    }
    //
    fclose( fout ) ;
 *
 * */
//
// Torbert, 16 September 2019
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
int main( int argc , char* argv[] )
{
    //
    // MPI variables
    //
    int        size    ; // same
    int        rank    ; // different
    //
    MPI_Status status  ;
    int        tag = 0 ; // same!
    //
    // other variables
    //
    int        k , j  ;
    double     result ;
    //
    // boilerplate
    //
    MPI_Init(      &argc          , &argv ) ;
    MPI_Comm_size( MPI_COMM_WORLD , &size ) ;
    MPI_Comm_rank( MPI_COMM_WORLD , &rank ) ;
    //
    srand( rank ) ; // different!
    //
    // manager has rank = 0
    //
    if( rank == 0 )
    {
        printf( "\n" ) ;
        //
        for( k = 1 ; k < size ; k++ )
        {
            MPI_Recv( &result , 1 , MPI_DOUBLE , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
            //
            j = status.MPI_SOURCE ;
            //
            printf( "%d %d %20.16f\n" , j , size , result ) ;
        }
        //
        printf( "\n" );
    }
        //
        // workers have rank > 0
        //
    else
    {
        result = 0.0 ;
        //
        j = 100000000 ; // trials = 100 million
        //
        for( k = 0 ; k < j ; k++ )
        {
            result += ( ( rand() % 6 + 1 ) + ( rand() % 6 + 1 ) ) ; // dice
        }
        //
        result *= ( 1.0 / j ) ; // average
        //
        MPI_Send( &result , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD ) ;
    }
    //
    // boilerplate
    //
    MPI_Finalize() ;
    //
    return 0;
}
//
// end of file
//
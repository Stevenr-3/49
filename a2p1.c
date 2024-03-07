/******************************************************************************
Title : a2p1.c
Author : Steven Rojas
Created on : March 2, 2024
Description : Estimates natural log of input with rectangle method
Purpose : Finds estimate of natural log of given input and error from actual value
Usage : Input number 
Build with : gcc -o drawing_demo_03 draw_stars.c \
‘pkg-config --cflags --libs gtk+-2.0‘
Modifications: 
******************************************************************************/
/*integral 0 to x of 1/t
X is inputted number
*/

#include <mpi.h>
#include <stdio.h>
//math h for fabs(), abs value of real ln error
#define ROOT 0

int main(int argc, char *argv[]){
    int s = 10;//test segments set 10
    int i;
    int id;
    int p;
    float ans;

    MPI_Init(int &argc, char &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if(s == 0)
        //MPI_Finalize()return 0;
    MPI_Bcast (&s , 1 , MPI_INT , ROOT , MPI_COMM_WORLD ) ;



    float loc = 0;
    for(i=id;i<s;i++){
        loc = estimate_ln(s);
    }

    MPI_Reduce(&loc,&ans,1,MPI_FLOAT,MPI_SUM,ROOT,MPI_COMM_WORLD);


    if(ROOT == id){
        printf(ans);
        fflush(stdout);
    }

    MPI_Finalize();

}

float estimate_ln(int in,int s){
    int i;
    double sum;
    double dx;
    sum = 0.0;
if ( 0 == s ) return 0; /* handle bad argument */

dx = 1.0 / ( double ) s ; /* Set dx to the width of a segment*/

for(i = 1;i <= in;i++){
    x =dx * ((double) i - 0.5)/*multiply with dx get rectangle*/
    sum += 1.0/x;  //  FORMULA 
}
return sum * dx;
}

/*
9 
10 for ( i = 1; i <= num_segments ; i ++ ) {
11 x = dx * (( double ) i - 0.5) ; /* x is midpoint of segment i 
12 sum += 1.0 / (1.0 + x * x ) ; /* add new area to sum 
13 }
14 return 4.0 * dx * sum ; /* we multiply sum by dx because we are
computing an integral and dx is the differential */
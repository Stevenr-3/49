/******************************************************************************
Title : estimate_ln.c
Author : Steven Rojas
Created on : March 2, 2024
Description : Estimates natural log of input with rectangle method
Purpose : Finds estimate of natural log of given input and error from actual value
Usage : Input number
Build with : mpicc -Wall -g -o prog estimate_ln.c -lm \
Modifications:
******************************************************************************/


#include <mpi/mpi.h>
#include <math.h>
#include <stdio.h>
#define ROOT 0

/*
estimate_ln calculates segment width to get each midpoint of the curve
 through for loop with cyclic mapping. Then uses derivative of ln to get 
 area under curve returning double
*/
double estimate_ln(int input, int seg, int id, int p)
{
    int i;
    double dx;
    double sum;
    sum = 0.0;
    if (seg < 1 || input < 1)
    {
        return 0; /* handle bad argument */
    }
    dx = (input - 1.0) / seg; /* Set dx to the width of a segment*/
    double x;
    for (i = id + 1; i <= seg; i += p)
    {
        x = 1 + dx * ((double)i - 0.5); // get midpoint
        sum += 1.0 / x;                 //  FORMULA
    }
    return (sum * dx); // return area of segment rectangle
}
/*
Gets input number and number of segments and estimates ln of input using mpi*/
int main(int argc, char *argv[])
{
    int input;
    int id;
    int p;
    double ans;    // final ans
    int segs;      // segments
    double st, et; // start and end time

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    

    if (ROOT == id)
    {
        printf(" Enter the number of ln : [0 to quit ] ");
        fflush(stdout);
        scanf("%d", &input);
        printf(" Enter the number of intervals : [0 to quit ] ");
        fflush(stdout);
        scanf("%d", &segs);
    }
    MPI_Bcast(&input, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
    MPI_Bcast(&segs, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
    st = MPI_Wtime(); /*start time*/

    if (segs < 1 || input < 1)
    {
        return 0; /* handle bad argument */
    }

    double loc;
    loc = estimate_ln(input, segs, id, p);

    MPI_Reduce(&loc, &ans, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (ROOT == id)
    {
        double r = log(input);  /*error comparison*/
        r = fabs(ans - r);
        et = MPI_Wtime(); /*end time*/
        printf("%d\t %.13f \t", input, ans);
        printf("%.15f \t %.6f seconds", r, et - st);
        printf("\n");
    }
    MPI_Finalize();
    return 0;
}

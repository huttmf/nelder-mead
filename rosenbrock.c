/* 
 * Program: rosenbrock.c
 * Author: Michael F. Hutt
 * http://www.mikehutt.com
 * May 19, 2016
 * gcc -Wall -o rosenbrock rosenbrock.c nmsimplex.c -DMACOSX
 * test function for nmsimplex
 *
 */
 
#include "nmsimplex.h"
#include <stdio.h>
#include <math.h>
 
double rosen(double x[])
{
  return (100*((x[1]-x[0]*x[0])*(x[1]-x[0]*x[0]))+(1.0-x[0])*(1.0-x[0]));
}

double round2(double num, int precision)
{
  double rnum = 0.0;
  int tnum;

  if (num == 0.0)
    return num;

  rnum = num*pow(10,precision);
  tnum = (int)(rnum < 0 ? rnum-0.5 : rnum + 0.5);
  rnum = tnum/pow(10,precision);

  return rnum;
}

void my_constraints(double x[], int n)
{
  // round to 2 decimal places
  int i;

  for (i=0; i<n; i++) {
    x[i] = round2(x[i],2);
  }
}
 
int main()
{
  double start[] = {-1.2,1.0};
  double start_null[] = {0.0,0.0};
  double min;
  int i;

  /* run optimization with a starting point */
  min=simplex(rosen,start,2,1.0e-4,1,my_constraints);
  printf("The minimum was found at\n"); 
  for (i=0;i<2;i++) {
    printf("%0.2f, ",start[i]);
  }
  printf("value %0.2f\n\n",min);

  /* 
     use default starting point of:
     0,0
     0.965,0.259
     0.259,0.965
  */

  min=simplex(rosen,start_null,2,1.0e-4,1,my_constraints);
  printf("The minimum was found at\n"); 
  for (i=0;i<2;i++) {
    printf("%0.2f, ",start_null[i]);
  }
  printf("value %0.2f\n",min);
  
  return 0;
}

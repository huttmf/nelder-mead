/* 
 * Program: nmsimplex.c
 * Author : Michael F. Hutt
 * http://www.mikehutt.com
 * Nov. 3, 1997
 *
 * An implementation of the Nelder-Mead simplex method.
 *
 * Copyright (c) 1997-present <Michael F. Hutt>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 * Jan. 6, 1999 
 * Modified to conform to the algorithm presented
 * in Margaret H. Wright's paper on Direct Search Methods.
 *
 * Jul. 23, 2007
 * Fixed memory leak.
 *
 * Mar. 1, 2011
 * Added constraints.
 *
 * May 18, 2016
 * Re-evaluate all vertices after an unsuccessful contraction.
 */

#include "nmsimplex.h"

/* create the initial simplex */
void initialize_simplex(double **v, double start[], double scale, int n)
{
  /* assume one of the vertices is 0,0 */

  double pn,qn;   /* values used to create initial simplex */
  int i,j;
	
  pn = scale*(sqrt(n+1)-1+n)/(n*sqrt(2));
  qn = scale*(sqrt(n+1)-1)/(n*sqrt(2));
	
  for (i=0;i<n;i++) {
    v[0][i] = start[i];
  }
	
  for (i=1;i<=n;i++) {
    for (j=0;j<n;j++) {
      if (i-1 == j) {
	v[i][j] = pn + start[j];
      }
      else {
	v[i][j] = qn + start[j];
      }
    }
  }


}


/* print out the initial values */
void print_initial_simplex(double **v, double *f, int n)
{
  int i,j;
  
  printf("Initial Values\n");
  for (j=0;j<=n;j++) {
    for (i=0;i<n;i++) {
      printf("%f %f\n",v[j][i],f[j]);
    }
  }
}


/* print out the value at each iteration */
void print_iteration(double **v, double *f, int n, int itr)
{
  int i,j;
  
  printf("Iteration %d\n",itr);
  for (j=0;j<=n;j++) {
    for (i=0;i<n;i++) {
      printf("%f %f\n",v[j][i],f[j]);
    }
  }
}


/* find the index of the largest value */
int vg_index(double *f, int vg, int n)
{
  int j;
  
  for (j=0;j<=n;j++) {
    if (f[j] > f[vg]) {
      vg = j;
    }
  }
  return vg;
}


/* find the index of the smallest value */
int vs_index(double *f, int vs, int n)
{
  int j;
    
  for (j=0;j<=n;j++) {
    if (f[j] < f[vs]) {
      vs = j;
    }
  }
  return vs;
}


/* find the index of the second largest value */
int vh_index(double *f, int vh, int vg, int n)
{
  int j;
  
  for (j=0;j<=n;j++) {
    if (f[j] > f[vh] && f[j] < f[vg]) {
      vh = j;
    }
  }
  return vh;
}


/* calculate the centroid */
void centroid(double *vm, double **v, int n, int vg)
{
  int j,m;
  double cent;
  
  for (j=0;j<=n-1;j++) {
    cent=0.0;
    for (m=0;m<=n;m++) {
      if (m!=vg) {
	cent += v[m][j];
      }
    }
    vm[j] = cent/n;
  }
}


double simplex(double (*objfunc)(double[]), double start[],int n, double EPSILON, double scale, void (*constrain)(double[],int n))
{
	
  int vs;         /* vertex with smallest value */
  int vh;         /* vertex with next smallest value */
  int vg;         /* vertex with largest value */
	
  int i,j,row;
  int k;   	  /* track the number of function evaluations */
  int itr;	  /* track the number of iterations */
	
  double **v;     /* holds vertices of simplex */
  double *f;      /* value of function at each vertex */
  double fr;      /* value of function at reflection point */
  double fe;      /* value of function at expansion point */
  double fc;      /* value of function at contraction point */
  double *vr;     /* reflection - coordinates */
  double *ve;     /* expansion - coordinates */
  double *vc;     /* contraction - coordinates */
  double *vm;     /* centroid - coordinates */
  double min;
	
  double fsum,favg,s;
	
  /* dynamically allocate arrays */
	
  /* allocate the rows of the arrays */
  v =  (double **) malloc ((n+1) * sizeof(double *));
  f =  (double *) malloc ((n+1) * sizeof(double));
  vr = (double *) malloc (n * sizeof(double));
  ve = (double *) malloc (n * sizeof(double));  
  vc = (double *) malloc (n * sizeof(double));  
  vm = (double *) malloc (n * sizeof(double));  
	
  /* allocate the columns of the arrays */
  for (i=0;i<=n;i++) {
    v[i] = (double *) malloc (n * sizeof(double));
  }
	
  /* create the initial simplex */
  initialize_simplex(v,start,scale,n);

  /* impose constraints */
  if (constrain != NULL) {
    for (j=0;j<=n;j++) {
      constrain(v[j],n);
    }
  }
  
  /* find the initial function values */
  for (j=0;j<=n;j++) {
    f[j] = objfunc(v[j]);
  }
  k = n+1;
	
  /* print out the initial values */
  print_initial_simplex(v,f,n);

  /* begin the main loop of the minimization */
  for (itr=1;itr<=MAX_IT;itr++) {     
    /* find the index of the largest value */
    vg = vg_index(f,0,n);

    /* find the index of the smallest value */
    vs = vs_index(f,0,n);
		
    /* find the index of the second largest value */
    vh = vh_index(f,vs,vg,n);
		
    /* calculate the centroid */
    centroid(vm,v,n,vg);

    /* reflect vg to new vertex vr */
    for (j=0;j<=n-1;j++) {
      /*vr[j] = (1+ALPHA)*vm[j] - ALPHA*v[vg][j];*/
      vr[j] = vm[j]+ALPHA*(vm[j]-v[vg][j]);
    }
    if (constrain != NULL) {
      constrain(vr,n);
    }
    fr = objfunc(vr);
    k++;
		
    if (fr < f[vh] && fr >= f[vs]) {
      for (j=0;j<=n-1;j++) {
	v[vg][j] = vr[j];
      }
      f[vg] = fr;
    }
		
    /* investigate a step further in this direction */
    if ( fr <  f[vs]) {
      for (j=0;j<=n-1;j++) {
	/*ve[j] = GAMMA*vr[j] + (1-GAMMA)*vm[j];*/
	ve[j] = vm[j]+GAMMA*(vr[j]-vm[j]);
      }
      if (constrain != NULL) {
        constrain(ve,n);
      }
      fe = objfunc(ve);
      k++;
			
      /* 
	 by making fe < fr as opposed to fe < f[vs], 			   
	 Rosenbrocks function takes 63 iterations as opposed 
	 to 64 when using double variables. 
      */
			
      if (fe < fr) {
	for (j=0;j<=n-1;j++) {
	  v[vg][j] = ve[j];
	}
	f[vg] = fe;
      }
      else {
	for (j=0;j<=n-1;j++) {
	  v[vg][j] = vr[j];
	}
	f[vg] = fr;
      }
    }
		
    /* check to see if a contraction is necessary */
    if (fr >= f[vh]) {
      if (fr < f[vg] && fr >= f[vh]) {
	/* perform outside contraction */
	for (j=0;j<=n-1;j++) {
	  /*vc[j] = BETA*v[vg][j] + (1-BETA)*vm[j];*/
	  vc[j] = vm[j]+BETA*(vr[j]-vm[j]);
	}
	if (constrain != NULL) {
          constrain(vc,n);
        }
	fc = objfunc(vc);
	k++;
      }
      else {
	/* perform inside contraction */
	for (j=0;j<=n-1;j++) {
	  /*vc[j] = BETA*v[vg][j] + (1-BETA)*vm[j];*/
	  vc[j] = vm[j]-BETA*(vm[j]-v[vg][j]);
	}
	if (constrain != NULL) {
          constrain(vc,n);
        }
	fc = objfunc(vc);
	k++;
      }
			
			
      if (fc < f[vg]) {
	for (j=0;j<=n-1;j++) {
	  v[vg][j] = vc[j];
	}
	f[vg] = fc;
      }

      else {
	/* 
	   at this point the contraction is not successful,
	   we must halve the distance from vs to all the 
	   vertices of the simplex and then continue.
	   1997-10-31 - modified to account for ALL vertices. 
	*/
	
	for (row=0;row<=n;row++) {
	  if (row != vs) {
	    for (j=0;j<=n-1;j++) {
	      v[row][j] = v[vs][j]+(v[row][j]-v[vs][j])/2.0;
	    }
	  }
	}

	/* re-evaluate all the vertices */
	for (j=0;j<=n;j++) {
	  f[j] = objfunc(v[j]);
	}
	
	/* find the index of the largest value */
	vg = vg_index(f,0,n);
	
	/* find the index of the smallest value */
	vs = vs_index(f,0,n);
	
	/* find the index of the second largest value */
	vh = vh_index(f,vs,vg,n);

	if (constrain != NULL) {
          constrain(v[vg],n);
        }
	f[vg] = objfunc(v[vg]);
	k++;
	if (constrain != NULL) {
          constrain(v[vh],n);
        }
	f[vh] = objfunc(v[vh]);
	k++;
				
				
      }
    }
		
    /* print out the value at each iteration */
    print_iteration(v,f,n,itr);
		
    /* test for convergence */
    fsum = 0.0;
    for (j=0;j<=n;j++) {
      fsum += f[j];
    }
    favg = fsum/(n+1);
    s = 0.0;
    for (j=0;j<=n;j++) {
      s += pow((f[j]-favg),2.0)/(n);
    }
    s = sqrt(s);
    if (s < EPSILON) break;
  }
  /* end main loop of the minimization */
	
  /* find the index of the smallest value */
  vs = vs_index(f,0,n);
	
  printf("The minimum was found at\n"); 
  for (j=0;j<n;j++) {
    printf("%e\n",v[vs][j]);
    start[j] = v[vs][j];
  }
  min=objfunc(v[vs]);
  k++;
  printf("%d Function Evaluations\n",k);
  printf("%d Iterations through program\n",itr);
	
  free(f);
  free(vr);
  free(ve);
  free(vc);
  free(vm);
  for (i=0;i<=n;i++) {
    free (v[i]);
  }
  free(v);
  return min;
}






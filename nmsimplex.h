/*
 * header for nmsimplex.c
 * Author : Michael F. Hutt
 * http://www.mikehutt.com
 * 
 * An implementation of the Nelder-Mead simplex method.
 *
 * Copyright (c) 1997 Michael F. Hutt
 * Released under the MIT License
 *
 */

#ifndef NM_SIMPLEX_H
#define NM_SIMPLEX_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef MACOSX
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif

#define MAX_IT      1000      /* maximum number of iterations */
#define ALPHA       1.0       /* reflection coefficient */
#define BETA        0.5       /* contraction coefficient */
#define GAMMA       2.0       /* expansion coefficient */

void my_constraints(double x[], int n);
void initialize_simplex(double **v, double start[], double scale, int n);
void print_initial_simplex(double **v, double *f, int n);
void print_iteration(double **v, double *f, int n, int itr);
int vg_index(double *f, int vg, int n);
int vs_index(double *f, int vs, int n);
int vh_index(double *f, int vh, int vg, int n);
void centroid(double *vm, double **v, int n, int vg);
double simplex(double (*objfunc)(double[]), double start[],int n, double EPSILON, double scale, void (*constrain)(double[],int n));

#endif

#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

typedef struct matrix{
	int nrow;
	int ncol;
	double** values;
} MATRIX;

typedef struct neural_network{
	int hidden_length;
	int output_length;
	int input_length;
	MATRIX* hidden_layer;
	MATRIX* output_layer;
	double(*function)(double);
} NEURAL_NETWORK;

double* alloc_vector( int size );

NEURAL_NETWORK* new_neural_network( int hidden_length, int output_length, int input_length, double(*function)(double) );

MATRIX* new_matrix( int nrow, int ncol );

void free_matrix( MATRIX* matrix );

void print_matrix( MATRIX* matrix );

void fill( MATRIX* layer );

int argmax( double* vetor, int length );

void free_neural_network( NEURAL_NETWORK* m );

double sigmoid( double x );

void apply( double(*function)(double), double* output, double* source, int length );

void compute_net( MATRIX* layer, double* vetor, double* net );

double* run( NEURAL_NETWORK* m, double* input );

#endif

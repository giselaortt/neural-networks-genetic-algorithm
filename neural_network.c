#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "neural_network.h"

double* alloc_vector( int size ){
	double* ans = (double*)malloc(sizeof(double)*size);
	return ans;
}

MATRIX* new_matrix( int nrow, int ncol ){

	MATRIX* matrix = (MATRIX*)malloc(sizeof(MATRIX));
	(matrix)->nrow = nrow;
	(matrix)->ncol = ncol;
	(matrix)->values = (double**)malloc(sizeof(double*)*nrow);
	int i;
	for( i=0; i<nrow; i++ )
		matrix->values[i] = (double*)malloc(sizeof(double)*ncol);

}

void free_matrix( MATRIX* matrix ){
	int i;
	for( i=0; i<matrix->nrow; i++ )
		free( matrix->values[i] );
	free( matrix );
}

void print_matrix( MATRIX* matrix ){
	int i, j;
	for( i=0; i<matrix->nrow; i++ )
		for( j=0; j<matrix->ncol; j++ )
			printf("%lf ", matrix->values[i][j] );
		printf("\n");
	printf("\n");
}

void fill( MATRIX* layer ){
	srand(time(NULL));
	int i, j;
	for( i=0; i<layer->nrow; i++ )
		for( j=0; j<layer->ncol; j++ )
			layer->values[i][j] = (float)( rand()%1000 )/1000.0 - 0.5;
}

int argmax( double* vetor, int length ){
	int i, j = 0;
	for( i=1; i < length; i ++ )
		if( vetor[i] > vetor[j] )
			j = i;
	return j;
}

NEURAL_NETWORK* new_neural_network( int hidden_length, int output_length, int input_length, double(*function)(double) ){

	NEURAL_NETWORK*  m = (NEURAL_NETWORK*)malloc(sizeof(NEURAL_NETWORK));
	m->hidden_length = hidden_length;
	m->input_length = input_length;
	m->output_length = output_length;
	m->function = function;
	m->hidden_layer = new_matrix( m->hidden_length, m->input_length+1 );
	m->output_layer = new_matrix( m->output_length, m->hidden_length+1 );
	fill( m->hidden_layer );
	fill( m->output_layer );

	return m;
}

void free_neural_network( NEURAL_NETWORK* m ){
	free_matrix( m->hidden_layer );
	free_matrix( m->output_layer );
	free( m );
}

double sigmoid( double x ){
        if( x < 0 )
                return 1.0 - 1.0/(1.0 + pow( M_E , x ));
        else
                return 1.0/(1.0 + pow( M_E, -x ));
}

void apply( double(*function)(double),
		double* output, double* source, int length ){
	
	int i;
	for( i=0; i<length; i++ )
		output[i] = function( source[i] );
}

void compute_net( MATRIX* layer, double* vetor, double* net ){

	int i, j;
	for( i=0; i<layer->nrow; i++ ){
		for( j=0; j<layer->ncol - 1; j++ )
			net[i] += layer->values[i][j]*vetor[j];
		net[i] += layer->values[i][layer->ncol-1];
	}
}

double* run( NEURAL_NETWORK* m, double* input ){
	int i;

	double* net_hidden = alloc_vector( m->hidden_length );
	double* net_output = alloc_vector( m->output_length );
	double* f_hidden = alloc_vector( m->hidden_length );
	double* f_output = alloc_vector( m->output_length );

	compute_net( m->hidden_layer, input, net_hidden );
	apply( m->function, f_hidden, net_hidden, m->hidden_length );
	compute_net( m->output_layer, f_hidden , net_output );
	apply( m->function, f_output, net_output, m->output_length );

	free( net_hidden );
	free( f_hidden );
	free( net_output );
	free( f_output );

	return f_output;
}

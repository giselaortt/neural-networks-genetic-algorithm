#ifndef GENETIC_H
#define GENETIC_H

#define prob 0.03
#define tax 0.4
#define it 10000

#include "neural_network.h"

MATRIX* matrix_crossover( MATRIX* pai, MATRIX* mae );

NEURAL_NETWORK* neural_network_crossover( NEURAL_NETWORK* pai, NEURAL_NETWORK* mae );

void matrix_mutation( MATRIX* matrix );

void neural_network_mutation( NEURAL_NETWORK* neural_network );

NEURAL_NETWORK* genetic( int size, int generation, double(*fitness)(double), void(*mutation)( double* ), double(*crossover)(double, double), int input_length, int hidden_length, int output_length );

#endif

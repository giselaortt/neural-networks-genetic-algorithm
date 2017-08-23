#ifndef GENETIC_H
#define GENETIC_H

#include "neural_network.h"

int get_rand( int begin, int end );

double media( int vector[], int tam );

MATRIX* matrix_crossover( MATRIX* pai, MATRIX* mae );

void testing_the_testing();

NEURAL_NETWORK* neural_network_crossover( NEURAL_NETWORK* pai, NEURAL_NETWORK* mae );

void matrix_mutation( MATRIX* matrix );

void neural_network_mutation( NEURAL_NETWORK* neural_network );

NEURAL_NETWORK* genetic_elitism( int size, int generation, double(*fitness)(NEURAL_NETWORK*), void(*mutation)(NEURAL_NETWORK*), NEURAL_NETWORK*(*crossover)(NEURAL_NETWORK*, NEURAL_NETWORK*), int input_length, int hidden_length, int output_length );

double testing( NEURAL_NETWORK* element );

#endif

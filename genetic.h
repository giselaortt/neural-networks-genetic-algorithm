#ifndef GENETIC_H
#define GENETIC_H

#define prob 0.05
#define tax 0.4
#define it 10000

#include "neural_network.h"

MATRIX* matrix_crossover( MATRIX* pai, MATRIX* mae );
int fitness( NEURAL_NETWORK* element );

NEURAL_NETWORK* neural_network_crossover( NEURAL_NETWORK* pai, NEURAL_NETWORK* mae );

void matrix_mutation( MATRIX* matrix );

void neural_network_mutation( NEURAL_NETWORK* neural_network );

NEURAL_NETWORK* genetic_elitism( int size, int generation, int(*fitness)(NEURAL_NETWORK*), void(*mutation)(NEURAL_NETWORK*), NEURAL_NETWORK*(*crossover)(NEURAL_NETWORK*, NEURAL_NETWORK*), int input_length, int hidden_length, int output_length );

int testing( NEURAL_NETWORK* element );

#endif

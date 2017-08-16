#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "genetic.h"
#include "neural_network.h"
#include "game.h"

int fitness( NEURAL_NETWORK* element ){

	int i, j, ans;
	char dictionary[4] = {'w','a','s','d'};
	GAME* game = new_game();
	new_tile( game->table );
	double* temp = alloc_vector( 16 );
	while( !game_over( game->table ) ){
		for( i=0; i<4; i++ )
			for( j=0; j<4; j++ )
				temp[ i*4 + j ] = game->table[i][j];
		double* result = run( element, temp );
		move( dictionary[ argmax( result, 4 ) ], game );
		free(result);
	}
	ans = rank( game->table );
	free_game( game );
	free( temp );

	return ans;
}

MATRIX* matrix_crossover( MATRIX* pai, MATRIX* mae ){

	if( pai->nrow != mae->nrow || pai->ncol != mae->ncol )
		return NULL;
	MATRIX* kid = new_matrix(  mae->nrow, mae->ncol );
	srand(time(NULL));
	int i, j;
	for( i=0; i<mae->nrow; i++ ){
		for( j=0; j<mae->ncol; j++ ){
			if( rand()%2 )
				kid->values[i][j] = mae->values[i][j];
			else
				kid->values[i][j] = pai->values[i][j];
		}
	}

	return kid;
}

NEURAL_NETWORK* neural_network_crossover( NEURAL_NETWORK* pai, NEURAL_NETWORK* mae ){

	if( pai->hidden_length != mae->hidden_length || pai->output_length != mae->output_length || pai->input_length != mae->input_length )
		return NULL;
	NEURAL_NETWORK* kid = new_neural_network( pai->input_length, pai->hidden_length, pai->output_length, sigmoid );
	kid->hidden_layer = matrix_crossover( pai->hidden_layer, mae->hidden_layer );
	kid->output_layer = matrix_crossover( pai->output_layer, mae->output_layer );
	return kid;

}

void matrix_mutation( MATRIX* matrix ){

	srand(time(NULL));
	if( ( rand() % 100 ) < 100*prob ){
		int i = rand()%matrix->nrow;
		int j = rand()%matrix->ncol;
		if( rand()%2 )
			matrix->values[i][j] += tax;
		else
			matrix->values[i][j] -= tax;
	}

}

void neural_network_mutation( NEURAL_NETWORK* neural_network ){

	matrix_mutation( neural_network->hidden_layer );
	matrix_mutation( neural_network->output_layer );

}

NEURAL_NETWORK* genetic_elitism( int size, int generation, int(*fitness)(NEURAL_NETWORK*), void(*mutation)(NEURAL_NETWORK*), double(*crossover)(NEURAL_NETWORK*, NEURAL_NETWORK*), int input_length, int hidden_length, int output_length ){

	NEURAL_NETWORK* population[size];
	NEURAL_NETWORK* best = NULL;
	int i, j;
	srand(time(NULL));

	for( i = 0; i<size; i++ ){
		population[i] = new_neural_network( hidden_length, output_length, input_length, sigmoid );
		if( fitness( population[i] ) > fitness( best ) )
			best = population[i];
	}

	for( j=0; j<generation; j++){
		for( i=0; i<size; i++ ){
			if( population[i] != best ){
				neural_network_crossover( best, population[i] );
				mutation( population[i] );
			}
			if( fitness(population[i]) > fitness(best) )
				best = population[i];
		}
		printf("%d\n",fitness( best ));
	}
	for( i=0; i<size; i++ )
		if( population[i] != best )
			free_neural_network( population[i] );

	return best;
}

NEURAL_NETWORK* genetic_tourney( int size, int generation, int(*fitness)(NEURAL_NETWORK*), void(*mutation)(NEURAL_NETWORK*), double(*crossover)(NEURAL_NETWORK*, NEURAL_NETWORK*), int input_length, int hidden_length, int output_length ){

	NEURAL_NETWORK* population[size];
	int i, j, bestfit;
	srand(time(NULL));
	NEURAL_NETWORK* best;

	for( i = 0; i<size; i++ ){
		population[i] = new_neural_network( hidden_length, output_length, input_length, sigmoid );
		int fit = fitness( population[i] );
		if( fit > bestfit ) best = population[i];
	}

	for( j=0; j<generation; j++){
		
	}
	for( i=0; i<size; i++ )
		if( population[i] != best )
			free_neural_network( population[i] );

	return best;
}

void* genetic_roulette( int size, int generation, int(*fitness)(NEURAL_NETWORK*), void(*mutation)(NEURAL_NETWORK*), double(*crossover)(NEURAL_NETWORK*, NEURAL_NETWORK*), int input_length, int hidden_length, int output_length ){

	NEURAL_NETWORK* population[size];
	NEURAL_NETWORK* children[size];
	int fitness_sum = 0;
	int fitness_vector[size];
	int i, j;
	int mom_id, dad_id;
	srand(time(NULL));

	for( i = 0; i<size; i++ ){
		population[i] = new_neural_network( hidden_length, output_length, input_length, sigmoid );
		fitness_vector[i] = fitness( population[i] );
		fitness_sum += fitness_vector[i];
	}

	for( i=0; i<generation; i++){
		for( j=0; j<size;j++ ){
			int dad = rand()%fitness_sum;
			int mom = rand()%fitness_sum;
			int sum = 0;
			for( dad_id=0; sum < dad; dad_id++ ){
				sum += fitness_vector[dad_id];
			}
			for( mom_id=0; sum < mom; mom_id++ ){
				sum += fitness_vector[mom_id];
			}
			if( mom_id == dad_id )
				if( dad_id != size - 1 ) mom_id++;
				else mom_id = 0;
			children[j] = neural_network_crossover( population[mom_id], population[dad_id] );
		}
		for( j=0; j<size; j++ ){
			free_neural_network( population[i] );
			population[i] = children[i];
			fitness_vector[i] = fitness( population[i] );
			fitness_sum += fitness_vector[i];
		}
		printf("%lf\n",(float)fitness_sum/(float)size);
	}
	for( i=0; i<size; i++ )
//		if( population[i] != best )
		free_neural_network( population[i] );

//	return best;
}

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "genetic.h"
#include "neural_network.h"
#include "game.h"

#define prob 0.05
#define tax 0.1
#define it 10000

#define true 1
#define false 0
#define min(a,b) a < b ? a : b
#define max(a,b) a > b ? a : b

int turn;

double media( int vector[], int tam ){
	int total = 0;
	int i;
	for( i=0; i<tam; i++ )
		total += vector[i];
	return (double)total/(double)tam;
}

double testing( NEURAL_NETWORK* element ){

	if( element == NULL )
		return 0;
	int i, j, ans;
	char dictionary[4] = {'w','a','s','d'};
	int scores[10];
	GAME* game = new_game();
	new_tile( game->table );
	double* temp = alloc_vector( 16 );
	for( turn = 0; turn < 10; turn++ ){
		while( !game_over( game->table ) ){
			for( i=0; i<4; i++ )
				for( j=0; j<4; j++ )
					temp[ i*4 + j ] = game->table[i][j];
			double* result = run( element, temp );
			if( move( dictionary[ argmax( result, 4 ) ], game ) == false )
				break;
			free(result);
		}
		scores[ turn ] = rank( game->table );
		reinit( game );
	}
	free_game( game );
	free( temp );

	return media( scores, 10 );
}
// made only in order to see whether the fitness function its been used is somehow stable.
// TODO some weird numbers have apperd, needs debugging.
void testing_the_testing(){

	NEURAL_NETWORK* population[4];
	int i, j;
	int size = 4, input_length = 16, output_length = 4;
	int hidden_length;

	printf("hidden lengths\n");
	for( i = 0; i<size; i++ ){
		hidden_length = rand()%50; 
		printf("%d ", hidden_length );
		population[i] = new_neural_network( hidden_length, output_length, input_length, sigmoid );
		usleep(500000);
		srand(time(NULL)*(size+1));
	}
	printf("\nresults\n");
	for( j=0; j<4; j++ ){
		for( i=0; i<10; i++ )
			printf("%lf\n", testing( population[j] ) );
		printf("\n");
	}
	for( i=0; i<size; i++ )
		free_neural_network( population[i] );
	return;
}

MATRIX* matrix_crossover( MATRIX* pai, MATRIX* mae ){

	MATRIX* kid = new_matrix( (mae->nrow + pai->nrow)/2, (mae->ncol + pai->ncol)/2 );
	srand(time(NULL));
	int i, j, nrow, ncol; // 19 / 8 / 2017, nakano, andre: minha bunda esta em risco, quero emocao.
	int rowmenor = min( mae->nrow, pai->nrow );
	int colmenor = min( mae->ncol, pai->ncol );
	for( i = 0; i < rowmenor; i++ ){
		for( j = 0; j < colmenor; j++ ){
			if( rand()%2 )
				kid->values[i][j] = mae->values[i][j];
			else
				kid->values[i][j] = pai->values[i][j];
		}
	}

	MATRIX* maior;
	if( pai->nrow > mae->nrow )
		maior = pai;
	else
		maior = mae;

	for( i = rowmenor; i < kid->nrow; i++ )
		for( j = colmenor; j < kid->ncol; j++ )
			kid->values[i][j] = maior->values[i][j];

	return kid;
}

NEURAL_NETWORK* neural_network_crossover( NEURAL_NETWORK* pai, NEURAL_NETWORK* mae ){

	NEURAL_NETWORK* kid = new_neural_network( 16, (pai->hidden_length + mae->hidden_length)/2, 4, sigmoid );
	kid->hidden_layer = matrix_crossover( pai->hidden_layer, mae->hidden_layer );
	if( ( rand() % 100 ) < 100*prob )
		matrix_mutation( kid->hidden_layer );
	kid->output_layer = matrix_crossover( pai->output_layer, mae->output_layer );
	if( ( rand() % 100 ) < 100*prob )
		matrix_mutation( kid->output_layer );

	return kid;
}

void matrix_mutation( MATRIX* matrix ){

	int i = rand()%matrix->nrow;
	int j = rand()%matrix->ncol;
	if( rand()%2 )
		matrix->values[i][j] += tax;
	else
		matrix->values[i][j] -= tax;
}

void neural_network_mutation( NEURAL_NETWORK* neural_network ){

	srand(time(NULL));
	if( rand()%100 > prob*100 )
		matrix_mutation( neural_network->hidden_layer );
	if( rand()%100 > prob*100 )
		matrix_mutation( neural_network->output_layer );
	//if( rand()%100 > prob*100 )
	//	neural_network->hidden_length++;
	// neural_network->hidden_layer->values = (double**)realloc( neural_network->hidden_layer->values, sizeof(double*)*hidden_length );
	// neural_network->hidden_layer->values[ neural_network->hidden_length - 1 ] = (double*)malloc(sizeof(double)*(neural_network->input_length + 1 ) );
	//

}

/*
NEURAL_NETWORK* genetic_elitism( int size, int generation, double(*fitness)(NEURAL_NETWORK*), void(*mutation)(NEURAL_NETWORK*), NEURAL_NETWORK*(*crossover)(NEURAL_NETWORK*, NEURAL_NETWORK*), int input_length, int hidden_length, int output_length ){

	NEURAL_NETWORK* population[size];
	NEURAL_NETWORK* best = NULL;
	int i, j;
	double best_fit, fit;
	turn = 1;

	for( i = 0; i<size; i++ ){
		population[i] = new_neural_network( rand()%50, output_length, input_length, sigmoid );
		usleep(500000);
		srand(time(NULL)*(size+1));
		printf("%d ", population[i]->hidden_length );
		fit = fitness( population[i] );
		if( fit > best_fit ){
			best = population[i];
			best_fit = fit;
			printf("best =  %lf\n", best_fit );
		}
	}
	for( j=0; j<generation; j++){
		for( i=0; i<size; i++ ){
			if( population[i] != best ){
				neural_network_crossover( best, population[i] );
				mutation( population[i] );
			}
			fit = fitness( population[i] );
			if( fit > best_fit ){
				best = population[i];
				best_fit = fit;
				printf("best =  %lf\n", best_fit );
			}
		}
		printf("%d\n", j );
	}
	for( i=0; i<size; i++ )
		if( population[i] != best )
			free_neural_network( population[i] );

	return best;
}
*//* 
NEURAL_NETWORK* genetic_tourney( int size, int generation, double(*fitness)(NEURAL_NETWORK*), void(*mutation)(NEURAL_NETWORK*), double(*crossover)(NEURAL_NETWORK*, NEURAL_NETWORK*), int input_length, int hidden_length, int output_length ){

	NEURAL_NETWORK* population[size];
	int i, j, bestfit=0;
	srand(time(NULL));

	for( i = 0; i<size; i++ ){
		population[i] = new_neural_network( rand()%50, output_length, input_length, sigmoid );
		usleep(500000);
		srand(time(NULL)*(size+1));
	}

	for( j=0; j<generation; j++){
		int father1, father2;
		father1 = rand()%size;
		father2 = rand()%size;
		while( father2 == father1 ) father2 = rand()%size;

		int mother1, mother2;
		mother1 = rand()%size;
		mother2 = rand()%size;
		while( mother1 == mother2 ) mother2 = rand()%size;
		
		neural_network* mother, father;
		if( fitness( population[ mother1 ] ) > fitness( population[ mother2 ] ) )
			mother = population[ mother1 ];
		else
			mother = population[ mother2 ];

		if( fitness( population[father1] ) > fitness( population[ father2 ] ) )
			father = population[ father1 ];
		else 
			father = population[ father2 ];

		child = neural_network_crossover( father, mother );
		adversary = rand()%size;
		if( fitness( population[adversary] ) < fitness( child ) ){
			free_neural_network( population[adversary] );
			population[adversary] = child;
		}
	}
	for( i=0; i<size; i++ )
		if( population[i] != best )
			free_neural_network( population[i] );

	return best;
}
*/
/*
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
//		if( population[i] != best )
	for( i=0; i<size; i++ )
		free_neural_network( population[i] );

//	return best;
}*/

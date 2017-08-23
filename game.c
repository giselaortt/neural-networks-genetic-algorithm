#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "game.h"

#define erro 0
#define deu_bom 1
#define false 0
#define true 1


GAME* new_game(){

	int i, j;
	GAME* game = (GAME*)malloc(sizeof(GAME));
	game->table = (int**)malloc(sizeof(int*)*4);
	for( i=0; i<4; i++ )
		game->table[i] = (int*)malloc(sizeof(int)*4);
	game->points = 0;
	for( i=0; i<4; i++)
		for(j=0; j<4; j++) 
			game->table[i][j] = 0;

	return game;
}

void free_game( GAME* game ){

	int i;
	for( i=0; i<4; i++ )
		free( game->table[i] );
	free( game->table );
	free( game );
}

int new_tile( int **table ){
	int num = 0, i, j;
	for( i=0; i<4; i++ )
		for( j=0; j<4; j++ )
			if( table[i][j] == 0 ) num++;
	if( num == 0 )
		return erro;

	int position = rand()%num;
	for( i=0; i<4 && position != 0; i++ ){
		for( j=0; j<4 && position != 0; j ++ ){
			if( table[i][j] == 0 ){
				position--;
				if( position == 0 ){
					srand(time(NULL)*i*j);
					if( rand()%10 == 1 )
						table[i][j] = 4;
					else
						table[i][j] = 2;
				}
			}
		}
	}

	return deu_bom;
}

int can_move_left( int row, int col, int **table ){
	if(table[row][col] == 0)
		return 0;

	if(table[row][col - 1] == 0)
		return 1;

	if(table[row][col - 1] == table[row][col])
		return 2;

	return 0;
}

int move_left( int **table, int* points ){
	int i, j;
	int stop = 0;
	int move = false;

	for(i = 0; i < 4; i++){
		for(j = 1; j < 4; j++){
			while(j != stop && can_move_left(i, j, table) == 1){
				table[i][j - 1] += table[i][j];
				table[i][j] = 0;
				j--;
				move = true;
			}

			if(j != stop && can_move_left(i, j, table) == 2){
				table[i][j - 1] += table[i][j];
				table[i][j] = 0;
				(*points) += table[i][j - 1];
				stop = j;
				move = true;
			}
		}
		stop = 0;
	}

	return move;
}

int can_move_right( int **table, int row, int col){
	if(table[row][col] == 0)
		return 0;

	if(table[row][col + 1] == 0)
		return 1;

	if(table[row][col + 1] == table[row][col]){
		return 2;
	}

	return 0;
}

int move_right( int **table, int* points ){
	int i, j;
	int stop = 3;
	int move = false;

	for(i = 0; i < 4; i++){
		for(j = 2; j >= 0; j--){
			while(j != stop && can_move_right(table, i, j) == 1){
				table[i][j + 1] += table[i][j];
				table[i][j] = 0;
				j++;
				move = true;
			}

			if(j != stop && can_move_right(table, i, j) == 2){
				table[i][j + 1] += table[i][j];
				table[i][j] = 0;
				(*points) += table[i][j + 1];
				stop = j;
				move = true;
			}
		}
		stop = 3;
	}
	return move;
}

int can_move_up( int **table, int row, int col ){
	if(table[row][col] == 0)
		return 0;

	if(table[row - 1][col] == 0)
		return 1;

	if(table[row - 1][col] == table[row][col]){
		return 2;
	}

	return 0;
}

int move_up( int **table, int* points ){
	int i, j;
	int stop = 0;
	int move = false;

	for(j = 0; j < 4; j++){
		for(i = 1; i < 4; i++){
			while(i != stop && can_move_up( table, i, j ) == 1){
				table[i - 1][j] += table[i][j];
				table[i][j] = 0;
				i--;
				move = true;
			}

			if(i != stop && can_move_up( table, i, j ) == 2){
				table[i - 1][j] += table[i][j];
				table[i][j] = 0;
				(*points) += table[i - 1][j];
				stop = i;
				move = true;
			}
		}
		stop = 0;
	}

	return move;
}

int can_move_down( int **table, int row, int col ){
	if(table[row][col] == 0)
		return 0;

	if(table[row + 1][col] == 0)
		return 1;

	if(table[row + 1][col] == table[row][col]){
		return 2;
	}

	return 0;
}

int move_down( int **table, int* points ){
	int i, j;
	int stop = 4 - 1;
	int move = false;

	for(j = 0; j < 4; j++){
		for(i = 4 - 2; i >= 0; i--){
			while(i != stop && can_move_down(table, i, j) == 1){
				table[i + 1][j] += table[i][j];
				table[i][j] = 0;
				i++;
				move = true;
			}

			if(i != stop && can_move_down(table, i, j) == 2){
				table[i + 1][j] += table[i][j];
				table[i][j] = 0;
				(*points) += table[i + 1][j];
				stop = i;
				move = true;
			}
		}

		stop = 3;
	}

	return move;
}

int move( char direction, GAME* game ) {
	if(direction != 'w' && direction != 's' && direction != 'a' && direction != 'd')
		return false;

	if(direction == 'w')
		if(move_up( game->table, &game->points ))
			new_tile( game->table );
		else
			return false;

	if(direction == 's')
		if(move_down( game->table, &game->points ))
			new_tile( game->table );
		else
			return false;

	if(direction == 'a')
		if(move_left( game->table, &game->points ))
			new_tile( game->table );
		else
			return false;

	if(direction == 'd')
		if(move_right( game->table, &game->points ))
			new_tile( game->table );
		else
			return false;

	return true;
}

void print_table( int **table ){
	for(int i = 0; i < 4; i++ ){
		for(int j = 0; j < 4; j++ ){
			if(table[i][j] == 0)
				printf(". ");
			else
				printf("%d ", table[i][j]);
		}
		printf("\n");
	}
}

int game_over( int **table ){
	int i, j;
	for( i=0; i<4; i++ ){
		for( j=0; j<4; j++ ){
			if( table[i][j] == 0 )
				return false;
			if( j != 3 && table[i][j+1] == table[i][j] )
				return false;
			if( i != 3 && table[i+1][j] == table[i][j] )
				return false;
		}
	}
	return true;
}

int rank( int **table ){
	int maior = 0;

	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			if(maior < table[i][j])
				maior = table[i][j];

	if( maior == 0 ) return 0;
	return log(maior)/log(2);
}
void reinit( GAME* game ){
	if( game == NULL ) return;
	game->points = 0;
	int i, j;
	for( i=0; i<4; i++ )
		for( j=0; j<4; j++ )
			game->table[i][j] = 0;
	new_tile( game->table );
}

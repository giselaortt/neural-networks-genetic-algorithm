#ifndef GAME_H
#define GAME_H

typedef struct game{
	int **table;
	int points;
} GAME;

GAME* new_game();

void free_game( GAME* game );

int new_tile( int **table );

int can_move_left( int row, int col, int **table );

int move_left( int **table, int* points );

int can_move_right( int **table, int row, int col);

int move_right( int **table, int* points );

int can_move_up( int **table, int row, int col );

int move_up( int **table, int* points );

int can_move_down( int **table, int row, int col );

int move_down( int **table, int* points );

int move( char direction, GAME* game ) ;

void print_table( int **table );

int game_over( int **table );

int rank( int **table );

#endif

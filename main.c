#include <stdio.h>
#include <stdlib.h>

#include "genetic.h"
#include "game.h"
#include "neural_network.h"

int main(){

	int i, j;
	GAME* game = new_game();
	new_tile( game->table );
	print_table( game->table );
	printf("\n\n");

	while( !game_over( game->table ) ){
		char direction;
		scanf(" %c", &direction );
		move( direction, game );
		print_table( game->table );
		printf("\n\n");
	}

	free_game( game );

return 0;
}

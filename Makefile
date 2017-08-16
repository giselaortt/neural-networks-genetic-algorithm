main: game neural genetic
	cc main.c game.o neural_network.o genetic.o -o ex -lm

game:
	gcc game.c -c

genetic:
	gcc genetic.c -c

neural:
	gcc neural_network.c -c

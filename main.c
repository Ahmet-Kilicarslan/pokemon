#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "functions.h"
/*
 *   gcc -Wall -Werror -Wextra *.c -o project
 *   ./project
 */

int main(void) {

    srand(time(NULL));

    Type types[18];
    Move moves[486];
    Pokemon pokemons[1015];
    Player player1, player2;

    initialize(types, moves, pokemons, &player1, &player2);

    game(&player1, &player2);

    return 0;
}
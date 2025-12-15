#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "structs.h"

// Main initialization - calls all others
void initialize(Type types[], Move moves[], Pokemon pokemons[], Player *player1, Player *player2);

// Individual initialization functions
void initializeTypes(Type types[]);
void initializeMoves(Move moves[], Type types[]);
void initializePokemons(Pokemon pokemons[], Move moves[], Type types[]);

// Game functions
void game(Player *player1, Player *player2);
void gameRound(Player *player1, Player *player2);
void applyDamage(Player *player1, Player *player2, int p1Choice, int p1MoveIndex, int p2Choice, int p2MoveIndex);

// Helper functions
int isAlive(Player *player);
void updateCurrentIndex(Player *player);
float getTypeEffectMultiplier(Type moveType, Type defenderType1);

#endif
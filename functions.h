#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "structs.h"

void initialize(Type types[], Move moves[], Pokemon pokemons[],
                Player *player1, Player *player2);
void initializeTypes(Type types[]);
void initializeMoves(Move moves[], Type types[]);
void initializePokemons(Pokemon pokemons[], Move moves[], Type types[]);

void game(Player *player1, Player *player2);
void gameRound(Player *player1, Player *player2);

void applyDamage(Player *player1, Player *player2,
                 int p1Choice, int p1MoveIndex,
                 int p2Choice, int p2MoveIndex);
float getTypeEffectMultiplier(Type moveType, Type defenderType);

int isAlive(Player *player);
void updateCurrentIndex(Player *player);

Type* findTypeByName(Type types[], const char *name);
void assignRandomPokemon(Player *player, Pokemon pokemons[], int used[]);
void assignRandomMoves(Pokemon *pokemon, Move moves[]);

int getPlayerAction(Player *player);
int getMoveChoice(Player *player);
int getSwitchChoice(Player *player);
float calculateSingleDamage(Pokemon *attacker, Pokemon *defender, Move *move);
void displayBattleStatus(Player *player1, Player *player2);

#endif
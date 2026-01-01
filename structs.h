#ifndef POKEMON_STRUCTS_H
#define POKEMON_STRUCTS_H

#define MAX_NAME_LENGTH 20
#define NUM_TYPES 18
#define NUM_MOVES 486
#define NUM_POKEMON 1015
#define MOVES_PER_POKEMON 4
#define POKEMON_PER_PLAYER 6

typedef enum {
    PHYSICAL,
    SPECIAL
} Category;

typedef struct {
    char atkName[MAX_NAME_LENGTH];
    char defName[MAX_NAME_LENGTH];
    float multiplier;
} TypeEffect;

typedef struct {
    char name[MAX_NAME_LENGTH];
    TypeEffect effects[NUM_TYPES];
} Type;

typedef struct {
    char name[MAX_NAME_LENGTH];
    Type type;
    Category category;
    float power;
} Move;

typedef struct {
    char name[MAX_NAME_LENGTH];
    Type types[2];
    float maxHP;
    float currentHP;
    float attack;
    float defense;
    float spAtk;
    float spDef;
    float speed;
    Move moves[MOVES_PER_POKEMON];
} Pokemon;

typedef struct {
    char name[MAX_NAME_LENGTH];
    Pokemon pokemons[POKEMON_PER_PLAYER];
    int currentIndex;
} Player;

#endif
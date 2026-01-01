#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

Type* findTypeByName(Type types[], const char *name) {
    for (int i = 0; i < NUM_TYPES; i++) {
        if (strcmp(types[i].name, name) == 0) {
            return &types[i];
        }
    }
    return NULL;
}

static void initializeNoneType(Type *type) {
    strcpy(type->name, "None");
    for (int i = 0; i < NUM_TYPES; i++) {
        type->effects[i].multiplier = 1.0f;
    }
}

void assignRandomPokemon(Player *player, Pokemon pokemons[], int used[]) {
    for (int i = 0; i < POKEMON_PER_PLAYER; i++) {
        int index;
        do {
            index = rand() % NUM_POKEMON;
        } while (used[index]);

        used[index] = 1;
        player->pokemons[i] = pokemons[index];
    }
    player->currentIndex = 0;
}

void assignRandomMoves(Pokemon *pokemon, Move moves[]) {
    int usedMoves[NUM_MOVES] = {0};

    for (int i = 0; i < MOVES_PER_POKEMON; i++) {
        int moveIndex;
        do {
            moveIndex = rand() % NUM_MOVES;
        } while (usedMoves[moveIndex]);

        usedMoves[moveIndex] = 1;
        pokemon->moves[i] = moves[moveIndex];
    }
}

static FILE* openFileOrExit(const char *filename, const char *mode) {
    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        printf("Error: Could not open %s\n", filename);
        exit(1);
    }
    return file;
}

void initialize(Type types[], Move moves[], Pokemon pokemons[],
                Player *player1, Player *player2) {
    initializeTypes(types);
    initializeMoves(moves, types);
    initializePokemons(pokemons, moves, types);

    strcpy(player1->name, "Player 1");
    strcpy(player2->name, "Player 2");

    int used[NUM_POKEMON] = {0};
    assignRandomPokemon(player1, pokemons, used);
    assignRandomPokemon(player2, pokemons, used);
}

void initializeTypes(Type types[]) {
    FILE *file = openFileOrExit("types.txt", "r");

    int typeIndex = 0;
    char line[100];

    while (fgets(line, sizeof(line), file) && typeIndex < NUM_TYPES) {
        line[strcspn(line, "\n")] = '\0';

        if (strchr(line, ' ') == NULL) {
            strcpy(types[typeIndex].name, line);

            for (int i = 0; i < NUM_TYPES; i++) {
                char defName[MAX_NAME_LENGTH];
                float multiplier;
                fscanf(file, "%s %f\n", defName, &multiplier);

                strcpy(types[typeIndex].effects[i].atkName, types[typeIndex].name);
                strcpy(types[typeIndex].effects[i].defName, defName);
                types[typeIndex].effects[i].multiplier = multiplier;
            }
            typeIndex++;
        }
    }
    fclose(file);
}

void initializeMoves(Move moves[], Type types[]) {
    FILE *file = openFileOrExit("moves.txt", "r");

    for (int i = 0; i < NUM_MOVES; i++) {
        char name[50], typeName[MAX_NAME_LENGTH], category[MAX_NAME_LENGTH];
        float power;
        fscanf(file, "%s %s %s %f\n", name, typeName, category, &power);

        strcpy(moves[i].name, name);
        moves[i].power = power;

        Type *foundType = findTypeByName(types, typeName);
        if (foundType != NULL) {
            moves[i].type = *foundType;
        }

        moves[i].category = (strcmp(category, "Physical") == 0) ? PHYSICAL : SPECIAL;
    }
    fclose(file);
}

void initializePokemons(Pokemon pokemons[], Move moves[], Type types[]) {
    FILE *file = openFileOrExit("pokemon.txt", "r");

    for (int i = 0; i < NUM_POKEMON; i++) {
        char name[50], type1[MAX_NAME_LENGTH], type2[MAX_NAME_LENGTH];
        float maxHP, attack, defense, spAtk, spDef, speed;

        fscanf(file, "%s %s %s %f %f %f %f %f %f\n",
               name, type1, type2, &maxHP, &attack, &defense, &spAtk, &spDef, &speed);

        strcpy(pokemons[i].name, name);
        pokemons[i].maxHP = maxHP;
        pokemons[i].currentHP = maxHP;
        pokemons[i].attack = attack;
        pokemons[i].defense = defense;
        pokemons[i].spAtk = spAtk;
        pokemons[i].spDef = spDef;
        pokemons[i].speed = speed;

        Type *foundType1 = findTypeByName(types, type1);
        if (foundType1 != NULL) {
            pokemons[i].types[0] = *foundType1;
        }

        if (strcmp(type2, "-") == 0) {
            initializeNoneType(&pokemons[i].types[1]);
        } else {
            Type *foundType2 = findTypeByName(types, type2);
            if (foundType2 != NULL) {
                pokemons[i].types[1] = *foundType2;
            }
        }

        assignRandomMoves(&pokemons[i], moves);
    }
    fclose(file);
}
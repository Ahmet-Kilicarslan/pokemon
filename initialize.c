#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"


void initialize(
    Type types[],
    Move moves[],
    Pokemon pokemons[],
    Player *player1,
    Player *player2) {
    initializeTypes(types);
    initializeMoves(moves, types);
    initializePokemons(pokemons, moves, types);

    strcpy(player1->name, "Player 1");
    strcpy(player2->name, "Player 2");

    // Randomly assign 6 unique Pokemon to each player
    int used[1015] = {0}; // Track which Pokemon are used

    // Player 1
    for (int i = 0; i < 6; i++) {
        int index;
        do {
            index = rand() % 1015;
        } while (used[index]);

        used[index] = 1;
        player1->pokemons[i] = pokemons[index];
    }
    player1->currentIndex = 0; // Start with first Pokemon

    // Player 2
    for (int i = 0; i < 6; i++) {
        int index;
        do {
            index = rand() % 1015;
        } while (used[index]);

        used[index] = 1;
        player2->pokemons[i] = pokemons[index];
    }
    player2->currentIndex = 0;
}

void initializeTypes(Type types[]) {
    FILE *file = fopen("types.txt", "r");

    if (file == NULL) {
        printf("Error opening file types.txt\n");
        exit(1);
    }

    int typeIndex = 0;
    char line[100];
    while (fgets(line, sizeof(line), file) && typeIndex < 18) {
        //Remove new line
        line[strcspn(line, "\n")] = 0;

        if (strchr(line, ' ') == NULL) {
            strcpy(types[typeIndex].name, line);

            for (int i = 0; i < 18; i++) {
                char defName[20];
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
    FILE *file = fopen("moves.txt", "r");

    if (file == NULL) {
        printf("Error opening file moves.txt\n");
        exit(1);
    }

    for (int i = 0; i < 486; i++) {
        char name[50], typeName[20], category[20];
        float power;
        fscanf(file, "%s %s %s %f\n", name, typeName, category, &power);

        strcpy(moves[i].name, name);
        moves[i].power = power;

        for (int j = 0; j < 18; j++) {
            if (strcmp(types[j].name, typeName) == 0) {
                moves[i].type = types[j];
                break;
            }
        }

        if (strcmp(category, "Physical") == 0) {
            moves[i].category = PHYSICAL;
        } else {
            moves[i].category = SPECIAL;
        }
    }
    fclose(file);
}

void initializePokemons(Pokemon pokemons[], Move moves[], Type types[]) {
    FILE *file = fopen("pokemon.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open pokemon.txt\n");
        exit(1);
    }

    for (int i = 0; i < 1015; i++) {
        char name[50], type1[20], type2[20];
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

        // Find type1
        for (int j = 0; j < 18; j++) {
            if (strcmp(types[j].name, type1) == 0) {
                pokemons[i].types[0] = types[j];
                break;
            }
        }

        // Find type2 (or set to "None")
        if (strcmp(type2, "-") == 0) {
            strcpy(pokemons[i].types[1].name, "None");
            // Set all None effects to 1.0
            for (int j = 0; j < 18; j++) {
                pokemons[i].types[1].effects[j].multiplier = 1.0;
            }
        } else {
            for (int j = 0; j < 18; j++) {
                if (strcmp(types[j].name, type2) == 0) {
                    pokemons[i].types[1] = types[j];
                    break;
                }
            }
        }

        // Randomly assign 4 unique moves
        int usedMoves[486] = {0};
        for (int j = 0; j < 4; j++) {
            int moveIndex;
            do {
                moveIndex = rand() % 486;
            } while (usedMoves[moveIndex]);

            usedMoves[moveIndex] = 1;
            pokemons[i].moves[j] = moves[moveIndex];
        }
    }

    fclose(file);
}

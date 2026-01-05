#include <stdio.h>
#include "functions.h"

static void displayTeams(Player *player1, Player *player2) {
    printf("%s's team:\n", player1->name);
    for (int i = 0; i < POKEMON_PER_PLAYER; i++) {
        printf("%d. %s (HP: %.0f)\n", i + 1,
               player1->pokemons[i].name,
               player1->pokemons[i].maxHP);
    }

    printf("\n%s's team:\n", player2->name);
    for (int i = 0; i < POKEMON_PER_PLAYER; i++) {
        printf("%d. %s (HP: %.0f)\n", i + 1,
               player2->pokemons[i].name,
               player2->pokemons[i].maxHP);
    }
}

void displayBattleStatus(Player *player1, Player *player2) {
    Pokemon *p1Active = &player1->pokemons[player1->currentIndex];
    Pokemon *p2Active = &player2->pokemons[player2->currentIndex];

    printf("--- NEW ROUND ---\n");
    printf("%s's %s (HP: %.0f)\n", player1->name, p1Active->name, p1Active->currentHP);
    printf("%s's %s (HP: %.0f)\n\n", player2->name, p2Active->name, p2Active->currentHP);
}

int getPlayerAction(Player *player) {
    printf("%s, choose your action:\n", player->name);
    printf("1 - Attack\n");
    printf("2 - Change Pokemon\n");

    int choice;
    scanf("%d", &choice);
    return choice;
}

int getMoveChoice(Player *player) {
    Pokemon *current = &player->pokemons[player->currentIndex];

    printf("Select move:\n");
    for (int i = 0; i < MOVES_PER_POKEMON; i++) {
        printf("%d - %s\n", i + 1, current->moves[i].name);
    }

    int choice;
    scanf("%d", &choice);
    return choice - 1;
}

int getSwitchChoice(Player *player) {
    printf("Select Pokemon:\n");

    for (int i = 0; i < POKEMON_PER_PLAYER; i++) {
        if (player->pokemons[i].currentHP > 0) {
            printf("%d - %s (HP: %.0f)\n", i + 1,
                   player->pokemons[i].name,
                   player->pokemons[i].currentHP);
        }
    }

    int choice;
    scanf("%d", &choice);
    return choice - 1;
}



static void handleFaintedPokemon(Player *player) {
    Pokemon *current = &player->pokemons[player->currentIndex];

    if (current->currentHP <= 0) {
        printf("%s's %s fainted!\n", player->name, current->name);
        updateCurrentIndex(player);
    }
}

void game(Player *player1, Player *player2) {
    printf("=== POKEMON BATTLE ===\n");
    printf("%s vs %s\n\n", player1->name, player2->name);

    displayTeams(player1, player2);
    printf("\nBattle Start!\n\n");

    while (isAlive(player1) && isAlive(player2)) {
        gameRound(player1, player2);
    }

    if (isAlive(player1)) {
        printf("\n%s wins!\n", player1->name);
    } else {
        printf("\n%s wins!\n", player2->name);
    }
}

void gameRound(Player *player1, Player *player2) {
    displayBattleStatus(player1, player2);

    // Step 1: Ask P1 for Action (Attack or Switch)
    int p1Choice = getPlayerAction(player1);

    // Step 2: Ask P2 for Action (Attack or Switch)
    printf("\n");
    int p2Choice = getPlayerAction(player2);

    int p1MoveIndex = -1;
    int p2MoveIndex = -1;

    // Step 3: Ask P1 for Specifics (Which Move or Which Pokemon)
    printf("\n%s's turn details:\n", player1->name);
    if (p1Choice == 1) {
        p1MoveIndex = getMoveChoice(player1);
    } else {
        int newIndex = getSwitchChoice(player1);
        player1->currentIndex = newIndex;
    }

    // Step 4: Ask P2 for Specifics (Which Move or Which Pokemon)
    printf("\n%s's turn details:\n", player2->name);
    if (p2Choice == 1) {
        p2MoveIndex = getMoveChoice(player2);
    } else {
        int newIndex = getSwitchChoice(player2);
        player2->currentIndex = newIndex;
    }


    applyDamage(player1, player2, p1Choice, p1MoveIndex, p2Choice, p2MoveIndex);

    handleFaintedPokemon(player1);
    handleFaintedPokemon(player2);

    printf("\n");
}

int isAlive(Player *player) {
    for (int i = 0; i < POKEMON_PER_PLAYER; i++) {
        if (player->pokemons[i].currentHP > 0) {
            return 1;
        }
    }
    return 0;
}

void updateCurrentIndex(Player *player) {
    for (int i = 0; i < POKEMON_PER_PLAYER; i++) {
        if (player->pokemons[i].currentHP > 0) {
            player->currentIndex = i;
            printf("%s sends out %s!\n", player->name, player->pokemons[i].name);
            return;
        }
    }
}
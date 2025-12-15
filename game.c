#include <stdio.h>
#include "functions.h"

void game(Player *player1, Player *player2) {
    printf("=== POKEMON BATTLE ===\n");
    printf("%s vs %s\n\n", player1->name, player2->name);

    printf("%s's team:\n", player1->name);
    for (int i = 0; i < 6; i++) {
        printf("%d. %s (HP: %.0f)\n", i+1, player1->pokemons[i].name, player1->pokemons[i].maxHP);
    }

    printf("\n%s's team:\n", player2->name);
    for (int i = 0; i < 6; i++) {
        printf("%d. %s (HP: %.0f)\n", i+1, player2->pokemons[i].name, player2->pokemons[i].maxHP);
    }

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
    printf("--- NEW ROUND ---\n");
    printf("%s's %s (HP: %.0f)\n", player1->name,
           player1->pokemons[player1->currentIndex].name,
           player1->pokemons[player1->currentIndex].currentHP);
    printf("%s's %s (HP: %.0f)\n\n", player2->name,
           player2->pokemons[player2->currentIndex].name,
           player2->pokemons[player2->currentIndex].currentHP);

    // Player 1 choice
    printf("%s, choose your action:\n", player1->name);
    printf("1 - Attack\n2 - Change Pokemon\n");
    int p1Choice;
    scanf("%d", &p1Choice);

    int p1MoveIndex = -1;
    if (p1Choice == 1) {
        Pokemon *current = &player1->pokemons[player1->currentIndex];
        printf("Select move:\n");
        for (int i = 0; i < 4; i++) {
            printf("%d - %s\n", i+1, current->moves[i].name);
        }
        scanf("%d", &p1MoveIndex);
        p1MoveIndex--;
    } else {
        printf("Select Pokemon:\n");
        for (int i = 0; i < 6; i++) {
            if (player1->pokemons[i].currentHP > 0) {
                printf("%d - %s (HP: %.0f)\n", i+1, player1->pokemons[i].name, player1->pokemons[i].currentHP);
            }
        }
        int choice;
        scanf("%d", &choice);
        player1->currentIndex = choice - 1;
    }

    // Player 2 choice
    printf("\n%s, choose your action:\n", player2->name);
    printf("1 - Attack\n2 - Change Pokemon\n");
    int p2Choice;
    scanf("%d", &p2Choice);

    int p2MoveIndex = -1;
    if (p2Choice == 1) {
        Pokemon *current = &player2->pokemons[player2->currentIndex];
        printf("Select move:\n");
        for (int i = 0; i < 4; i++) {
            printf("%d - %s\n", i+1, current->moves[i].name);
        }
        scanf("%d", &p2MoveIndex);
        p2MoveIndex--;
    } else {
        printf("Select Pokemon:\n");
        for (int i = 0; i < 6; i++) {
            if (player2->pokemons[i].currentHP > 0) {
                printf("%d - %s (HP: %.0f)\n", i+1, player2->pokemons[i].name, player2->pokemons[i].currentHP);
            }
        }
        int choice;
        scanf("%d", &choice);
        player2->currentIndex = choice - 1;
    }

    applyDamage(player1, player2, p1Choice, p1MoveIndex, p2Choice, p2MoveIndex);

    // Check if Pokemon fainted and update index
    if (player1->pokemons[player1->currentIndex].currentHP <= 0) {
        printf("%s's %s fainted!\n", player1->name, player1->pokemons[player1->currentIndex].name);
        updateCurrentIndex(player1);
    }
    if (player2->pokemons[player2->currentIndex].currentHP <= 0) {
        printf("%s's %s fainted!\n", player2->name, player2->pokemons[player2->currentIndex].name);
        updateCurrentIndex(player2);
    }

    printf("\n");
}

int isAlive(Player *player) {
    for (int i = 0; i < 6; i++) {
        if (player->pokemons[i].currentHP > 0) {
            return 1;
        }
    }
    return 0;
}

void updateCurrentIndex(Player *player) {
    for (int i = 0; i < 6; i++) {
        if (player->pokemons[i].currentHP > 0) {
            player->currentIndex = i;
            printf("%s sends out %s!\n", player->name, player->pokemons[i].name);
            return;
        }
    }
}
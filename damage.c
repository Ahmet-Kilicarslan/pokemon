#include <stdio.h>
#include <string.h>
#include "functions.h"

void applyDamage(Player *player1, Player *player2, int p1Choice, int p1MoveIndex, int p2Choice, int p2MoveIndex) {
    Pokemon *p1 = &player1->pokemons[player1->currentIndex];
    Pokemon *p2 = &player2->pokemons[player2->currentIndex];

    // Determine who attacks first (faster Pokemon or if someone switched)
    int p1First = p1->speed >= p2->speed;

    // If either player switched, they go last
    if (p1Choice == 2) p1First = 0;
    if (p2Choice == 2) p1First = 1;
    if (p1Choice == 2 && p2Choice == 2) return; // Both switched, no damage

    Player *first = p1First ? player1 : player2;
    Player *second = p1First ? player2 : player1;
    int firstChoice = p1First ? p1Choice : p2Choice;
    int secondChoice = p1First ? p2Choice : p1Choice;
    int firstMoveIndex = p1First ? p1MoveIndex : p2MoveIndex;
    int secondMoveIndex = p1First ? p2MoveIndex : p1MoveIndex;

    // First attacker
    if (firstChoice == 1) {
        Pokemon *attacker = &first->pokemons[first->currentIndex];
        Pokemon *defender = &second->pokemons[second->currentIndex];
        Move *move = &attacker->moves[firstMoveIndex];

        float attack = (move->category == PHYSICAL) ? attacker->attack : attacker->spAtk;
        float defense = (move->category == PHYSICAL) ? defender->defense : defender->spDef;

        float typeEffect1 = getTypeEffectMultiplier(move->type, defender->types[0]);
        float typeEffect2 = (strcmp(defender->types[1].name, "None") == 0) ? 1.0 :
                            getTypeEffectMultiplier(move->type, defender->types[1]);

        float stab = (strcmp(move->type.name, attacker->types[0].name) == 0 ||
                      strcmp(move->type.name, attacker->types[1].name) == 0) ? 1.5 : 1.0;

        float damage = move->power * (attack / defense) * typeEffect1 * typeEffect2 * stab;

        defender->currentHP -= damage;
        printf("%s's %s used %s! (Damage: %.1f)\n", first->name, attacker->name, move->name, damage);

        if (defender->currentHP <= 0) {
            defender->currentHP = 0;
            return; // Defender fainted, can't attack back
        }
    }

    // Second attacker
    if (secondChoice == 1) {
        Pokemon *attacker = &second->pokemons[second->currentIndex];
        Pokemon *defender = &first->pokemons[first->currentIndex];
        Move *move = &attacker->moves[secondMoveIndex];

        float attack = (move->category == PHYSICAL) ? attacker->attack : attacker->spAtk;
        float defense = (move->category == PHYSICAL) ? defender->defense : defender->spDef;

        float typeEffect1 = getTypeEffectMultiplier(move->type, defender->types[0]);
        float typeEffect2 = (strcmp(defender->types[1].name, "None") == 0) ? 1.0 :
                            getTypeEffectMultiplier(move->type, defender->types[1]);

        float stab = (strcmp(move->type.name, attacker->types[0].name) == 0 ||
                      strcmp(move->type.name, attacker->types[1].name) == 0) ? 1.5 : 1.0;

        float damage = move->power * (attack / defense) * typeEffect1 * typeEffect2 * stab;

        defender->currentHP -= damage;
        printf("%s's %s used %s! (Damage: %.1f)\n", second->name, attacker->name, move->name, damage);

        if (defender->currentHP <= 0) {
            defender->currentHP = 0;
        }
    }
}

float getTypeEffectMultiplier(Type moveType, Type defenderType1) {
    for (int i = 0; i < 18; i++) {
        if (strcmp(moveType.effects[i].defName, defenderType1.name) == 0) {
            return moveType.effects[i].multiplier;
        }
    }
    return 1.0;
}
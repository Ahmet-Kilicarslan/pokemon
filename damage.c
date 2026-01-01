#include <stdio.h>
#include <string.h>
#include "functions.h"

#define STAB_BONUS 1.5f
#define NO_STAB 1.0f

float getTypeEffectMultiplier(Type moveType, Type defenderType) {
    for (int i = 0; i < NUM_TYPES; i++) {
        if (strcmp(moveType.effects[i].defName, defenderType.name) == 0) {
            return moveType.effects[i].multiplier;
        }
    }
    return 1.0f;
}

static float getSTABMultiplier(Move *move, Pokemon *attacker) {
    if (strcmp(move->type.name, attacker->types[0].name) == 0 ||
        strcmp(move->type.name, attacker->types[1].name) == 0) {
        return STAB_BONUS;
    }
    return NO_STAB;
}

static float getCombinedTypeEffect(Move *move, Pokemon *defender) {
    float typeEffect1 = getTypeEffectMultiplier(move->type, defender->types[0]);

    float typeEffect2;
    if (strcmp(defender->types[1].name, "None") == 0) {
        typeEffect2 = 1.0f;
    } else {
        typeEffect2 = getTypeEffectMultiplier(move->type, defender->types[1]);
    }

    return typeEffect1 * typeEffect2;
}

float calculateSingleDamage(Pokemon *attacker, Pokemon *defender, Move *move) {
    float attack, defense;

    if (move->category == PHYSICAL) {
        attack = attacker->attack;
        defense = defender->defense;
    } else {
        attack = attacker->spAtk;
        defense = defender->spDef;
    }

    float typeEffect = getCombinedTypeEffect(move, defender);
    float stab = getSTABMultiplier(move, attacker);

    return move->power * (attack / defense) * typeEffect * stab;
}

static int executeAttack(Player *attackerPlayer, Player *defenderPlayer, int moveIndex) {
    Pokemon *attacker = &attackerPlayer->pokemons[attackerPlayer->currentIndex];
    Pokemon *defender = &defenderPlayer->pokemons[defenderPlayer->currentIndex];
    Move *move = &attacker->moves[moveIndex];

    float damage = calculateSingleDamage(attacker, defender, move);
    defender->currentHP -= damage;

    printf("%s's %s used %s! (Damage: %.1f)\n",
           attackerPlayer->name, attacker->name, move->name, damage);

    if (defender->currentHP <= 0) {
        defender->currentHP = 0;
        return 1;
    }
    return 0;
}

static int determineFirstPlayer(Player *player1, Player *player2,
                                int p1Choice, int p2Choice) {
    Pokemon *p1 = &player1->pokemons[player1->currentIndex];
    Pokemon *p2 = &player2->pokemons[player2->currentIndex];

    int p1First = p1->speed >= p2->speed;

    if (p1Choice == 2) p1First = 0;
    if (p2Choice == 2) p1First = 1;

    return p1First;
}

void applyDamage(Player *player1, Player *player2,
                 int p1Choice, int p1MoveIndex,
                 int p2Choice, int p2MoveIndex) {

    if (p1Choice == 2 && p2Choice == 2) {
        return;
    }

    int p1First = determineFirstPlayer(player1, player2, p1Choice, p2Choice);

    Player *first = p1First ? player1 : player2;
    Player *second = p1First ? player2 : player1;
    int firstChoice = p1First ? p1Choice : p2Choice;
    int secondChoice = p1First ? p2Choice : p1Choice;
    int firstMoveIndex = p1First ? p1MoveIndex : p2MoveIndex;
    int secondMoveIndex = p1First ? p2MoveIndex : p1MoveIndex;

    int defenderFainted = 0;
    if (firstChoice == 1) {
        defenderFainted = executeAttack(first, second, firstMoveIndex);
    }

    if (secondChoice == 1 && !defenderFainted) {
        executeAttack(second, first, secondMoveIndex);
    }
}
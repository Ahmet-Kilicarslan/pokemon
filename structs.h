

#ifndef POKEMON_STRUCTS_H
#define POKEMON_STRUCTS_H

typedef enum {
    PHYSICAL,
    SPECIAL
} Category;

typedef struct {
    char atkName[20];
    char defName[20];
    float multiplier;
} TypeEffect;

typedef struct {
    char name[20];
    TypeEffect effects[18];
} Type;

typedef struct {
    char name[20];
    Type type;
    Category category;
    float power;
} Move;

typedef struct {
    char name[20];
    Type types[2];
    float maxHP;
    float currentHP;
    float attack;
    float defense;
    float spAtk;
    float spDef;
    float speed;
    Move moves[4];
}Pokemon;

typedef struct {
    char name[20];
    Pokemon pokemons[6];
    int currentIndex;

}Player;





#endif //POKEMON_STRUCTS_H

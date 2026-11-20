#ifndef PLAYER_H
#define PLAYER_H

#include "config.h"

/* Время игры описано в utils.h; здесь достаточно предварительного объявления */
struct GameTime;

/* Перечисление профессий (Вариант 1) */
enum Profession {
    PROF_NONE = 0,
    PROF_FARMER,      /* Фермер      — сбалансированный, +20% золота за этаж */
    PROF_LUMBERJACK,  /* Лесоруб     — много здоровья, +1 HP за шаг */
    PROF_MINER,       /* Шахтёр      — максимум здоровья, +30% золота за этаж */
    PROF_SCOUT        /* Разведчик   — много выносливости, 50% шанс бесплатного шага */
};

/* Структура игрока: все данные о персонаже в одном месте */
struct Player {
    char name[MAX_PLAYER_NAME];
    int hp;
    int max_hp;
    int stamina;
    int max_stamina;
    int gold;
    int x;                       /* Координаты на карте приключения */
    int y;
    enum Profession profession;
    int inventory[INVENTORY_SIZE];
};

/* Жизненный цикл */
struct Player* player_create(void);
void player_destroy(struct Player** player);

/* Профессии */
const char* profession_name(enum Profession profession);
const char* profession_description(enum Profession profession);
int profession_start_hp(enum Profession profession);
int profession_start_stamina(enum Profession profession);
int player_gold_percent(const struct Player* player);

/* Характеристики */
int player_has_stamina(const struct Player* player);
void player_spend_stamina(struct Player* player, int amount);
void player_add_gold(struct Player* player, int amount);
void player_restore_full(struct Player* player);

/* Инвентарь */
void player_show_inventory(const struct Player* player, char names[MAX_ITEMS][MAX_NAME_LEN]);
void player_put_item(struct Player* player);
void player_drop_item(struct Player* player);
void player_check_adjacent(const struct Player* player);
void player_search_by_name(const struct Player* player, char names[MAX_ITEMS][MAX_NAME_LEN]);

/* Карточка персонажа */
void player_print_card(const struct Player* player, char names[MAX_ITEMS][MAX_NAME_LEN], const struct GameTime* time);

#endif /* PLAYER_H */

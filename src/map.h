#ifndef MAP_H
#define MAP_H

#include "config.h"
#include "player.h"
#include "entity.h"

/* Структура карты приключения */
struct Map {
    char** cells;                 /* Двумерный динамический массив клеток */
    int height;
    int width;
    int floor;                    /* Счётчик пройденных этажей */
    int mirror_active;            /* 1 - зеркальная иллюзия активна, 0 - разбита */
    struct EntityList* entities;  /* Массив сущностей этажа (ЛР 6) */
};

/* Жизненный цикл карты */
struct Map* map_create(int height, int width);
void map_free_cells(struct Map* map);
void map_destroy(struct Map** map);

/* Генерация и отрисовка */
void map_generate_tower(struct Map* map, int* hero_x, int* hero_y);
void map_print(const struct Map* map, int hero_x, int hero_y);

/* Режим приключения */
void action_adventure(struct Player* player, struct GameTime* time);

#endif /* MAP_H */

#include "entity.h"
#include "config.h"

#include <stdio.h>
#include <stdlib.h>

/* Вспомогательный расчёт масштабирования характеристик по этажу */
static int scale_by_floor(int base_value, int floor) {
    int result = base_value;
    for (int i = 1; i < floor; i++) {
        result = (result * FLOOR_SCALE_PERCENT) / PERCENT_BASE;
    }
    return result;
}

/* Создание массива сущностей для этажа */
struct EntityList* entities_create(int map_height, int map_width, char** cells, int floor) {
    if (cells == NULL || map_height <= 0 || map_width <= 0) {
        return NULL;
    }

    struct EntityList* list = (struct EntityList*)malloc(sizeof(struct EntityList));
    if (list == NULL) {
        printf("Ошибка выделения памяти под список сущностей!\n");
        return NULL;
    }

    /* Подсчитаем количество свободных клеток пола */
    int free_cells = 0;
    for (int y = 0; y < map_height; y++) {
        for (int x = 0; x < map_width; x++) {
            if (cells[y][x] == CELL_FLOOR) {
                free_cells++;
            }
        }
    }

    /* Рассчитываем количество сущностей (% от свободных клеток) */
    int target_count = (free_cells * ENTITY_PERCENT) / PERCENT_BASE;
    /* Резервируем память с запасом под спавн Энтов */
    int capacity = target_count + ENT_SPAWN_COUNT + 1;
    if (capacity < 1) capacity = 1;

    list->data = (struct Entity*)malloc(capacity * sizeof(struct Entity));
    if (list->data == NULL) {
        printf("Ошибка выделения памяти под массив сущностей!\n");
        free(list);
        return NULL;
    }

    list->count = 0;
    list->total_resources = 0;
    list->destroyed_count = 0;

    /* Собираем координаты всех свободных клеток */
    int placed = 0;
    for (int y = 1; y < map_height - 1 && placed < target_count; y++) {
        for (int x = 1; x < map_width - 1 && placed < target_count; x++) {
            if (cells[y][x] != CELL_FLOOR) {
                continue;
            }

            /* Случайный выбор: размещать ли здесь сущность */
            if (rand() % PERCENT_BASE >= 40) {
                continue;
            }

            int roll = rand() % PERCENT_BASE;
            struct Entity* e = &list->data[list->count];
            e->x = x;
            e->y = y;
            e->is_alive = 1;

            if (roll < TREE_CHANCE) {
                /* Дерево */
                e->type = ENT_TREE;
                e->symbol = CELL_TREE;
                e->max_hp = scale_by_floor(BASE_RESOURCE_HP, floor);
                e->hp = e->max_hp;
                e->attack = 0;
                e->defense = 0;
                e->drop_item_id = ITEM_WOOD;
                e->gold_reward = 0;
                list->total_resources++;
            } else if (roll < TREE_CHANCE + ROCK_CHANCE) {
                /* Камень */
                e->type = ENT_ROCK;
                e->symbol = CELL_ROCK;
                e->max_hp = scale_by_floor(BASE_RESOURCE_HP, floor);
                e->hp = e->max_hp;
                e->attack = 0;
                e->defense = 0;
                e->drop_item_id = ITEM_STONE;
                e->gold_reward = 0;
                list->total_resources++;
            } else if (roll < TREE_CHANCE + ROCK_CHANCE + HEAL_CHANCE) {
                /* Целебный куст */
                e->type = ENT_HEAL;
                e->symbol = CELL_HEAL;
                e->max_hp = 1;
                e->hp = 1;
                e->attack = 0;
                e->defense = 0;
                e->drop_item_id = ITEM_NONE;
                e->gold_reward = 0;
            } else {
                /* Монстр */
                e->type = ENT_MONSTER;
                e->symbol = CELL_MONSTER;
                e->max_hp = scale_by_floor(BASE_MONSTER_HP, floor);
                e->hp = e->max_hp;
                e->attack = scale_by_floor(BASE_MONSTER_ATTACK, floor);
                e->defense = scale_by_floor(BASE_MONSTER_DEFENSE, floor);
                e->drop_item_id = (rand() % PERCENT_BASE < MONSTER_DROP_CHANCE) ? ITEM_GOLD : ITEM_NONE;
                e->gold_reward = scale_by_floor(BASE_MONSTER_GOLD, floor);
            }

            cells[y][x] = e->symbol;
            list->count++;
            placed++;
        }
    }

    return list;
}

/* Освобождение массива сущностей с требованием из методички */
void entities_destroy(struct EntityList** list) {
    if (list == NULL || *list == NULL) {
        return;
    }

    if ((*list)->data != NULL) {
        printf("Память массива сущностей по адресу %p освобождена\n", (void*)(*list)->data);
        free((*list)->data);
        (*list)->data = NULL;
    }

    free(*list);
    *list = NULL;
}

/* Поиск сущности по координатам на этаже */
struct Entity* entity_at(struct EntityList* list, int x, int y) {
    if (list == NULL || list->data == NULL) {
        return NULL;
    }

    for (int i = 0; i < list->count; i++) {
        if (list->data[i].is_alive && list->data[i].x == x && list->data[i].y == y) {
            return &list->data[i];
        }
    }

    return NULL;
}

/* Нанесение живых сущностей на карту */
void entities_place_on_map(const struct EntityList* list, char** cells) {
    if (list == NULL || list->data == NULL || cells == NULL) {
        return;
    }

    for (int i = 0; i < list->count; i++) {
        if (list->data[i].is_alive) {
            cells[list->data[i].y][list->data[i].x] = list->data[i].symbol;
        }
    }
}

/* Процент уничтоженных ресурсов (Вариант 5) */
int entities_destruction_percent(const struct EntityList* list) {
    if (list == NULL || list->total_resources == 0) {
        return 0;
    }
    return (list->destroyed_count * PERCENT_BASE) / list->total_resources;
}

/* Спавн агрессивных Энтов при Гневе Природы (>50% ресурсов срублено) */
void entities_spawn_ents(struct EntityList* list, int map_height, int map_width, char** cells, int floor) {
    if (list == NULL || list->data == NULL || cells == NULL) {
        return;
    }

    int spawned = 0;
    for (int y = 1; y < map_height - 1 && spawned < ENT_SPAWN_COUNT; y++) {
        for (int x = 1; x < map_width - 1 && spawned < ENT_SPAWN_COUNT; x++) {
            if (cells[y][x] == CELL_FLOOR) {
                struct Entity* e = &list->data[list->count];
                e->type = ENT_ENT;
                e->symbol = CELL_ENT;
                e->x = x;
                e->y = y;
                e->max_hp = scale_by_floor(BASE_MONSTER_HP * ENT_HP_MULTIPLIER, floor);
                e->hp = e->max_hp;
                e->attack = scale_by_floor(BASE_MONSTER_ATTACK * ENT_ATTACK_MULTIPLIER, floor);
                e->defense = scale_by_floor(BASE_MONSTER_DEFENSE * ENT_DEFENSE_MULTIPLIER, floor);
                e->drop_item_id = ITEM_WOOD;
                e->gold_reward = scale_by_floor(BASE_MONSTER_GOLD * ENT_GOLD_MULTIPLIER, floor);
                e->is_alive = 1;

                cells[y][x] = CELL_ENT;
                list->count++;
                spawned++;
            }
        }
    }
    if (spawned > 0) {
        printf("ГНЕВ ПРИРОДЫ ПРОБУДИЛСЯ! Появились агрессивные Энты (%d)!\n", spawned);
    }
}

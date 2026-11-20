#ifndef ENTITY_H
#define ENTITY_H

#include "config.h"

/* Типы сущностей на карте */
enum EntityType {
    ENT_NONE = 0,
    ENT_TREE,       /* Дерево (T) — добываемый ресурс */
    ENT_ROCK,       /* Камень (R) — добываемый ресурс */
    ENT_HEAL,       /* Целебный куст (+) — восстановление HP/стамины */
    ENT_MONSTER,    /* Монстр (M) — враждебное существо */
    ENT_ENT         /* Энт (E) — ожившее дерево, Гнев Природы (Вариант 5) */
};

/* Структура сущности: один объект на карте приключения */
struct Entity {
    enum EntityType type;
    char symbol;
    int x;
    int y;
    int hp;
    int max_hp;
    int attack;
    int defense;
    int drop_item_id;   /* ID предмета из items.txt, который выпадет */
    int gold_reward;    /* Награда золотом за уничтожение */
    int is_alive;       /* 1 — на карте, 0 — уничтожен/собран */
};

/* Массив сущностей этажа */
struct EntityList {
    struct Entity* data;    /* Динамический массив */
    int count;              /* Количество сущностей */
    int total_resources;    /* Сколько ресурсов было при генерации */
    int destroyed_count;    /* Сколько ресурсов/монстров уничтожено */
};

/* Жизненный цикл */
struct EntityList* entities_create(int map_height, int map_width, char** cells, int floor);
void entities_destroy(struct EntityList** list);

/* Поиск */
struct Entity* entity_at(struct EntityList* list, int x, int y);

/* Отрисовка: наносит живые сущности на карту перед печатью */
void entities_place_on_map(const struct EntityList* list, char** cells);

/* Гнев Природы (Вариант 5) */
int entities_destruction_percent(const struct EntityList* list);
void entities_spawn_ents(struct EntityList* list, int map_height, int map_width, char** cells, int floor);

#endif /* ENTITY_H */

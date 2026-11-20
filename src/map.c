#include "map.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==================== Выделение и освобождение памяти ==================== */

struct Map* map_create(int height, int width) {
    struct Map* map = (struct Map*)malloc(sizeof(struct Map));
    if (map == NULL) {
        printf("Ошибка: не удалось выделить память под структуру карты!\n");
        return NULL;
    }

    map->cells = (char**)malloc(height * sizeof(char*));
    if (map->cells == NULL) {
        printf("Ошибка выделения памяти под строки карты!\n");
        free(map);
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        map->cells[i] = (char*)malloc(width * sizeof(char));
        if (map->cells[i] == NULL) {
            printf("Ошибка выделения памяти под ячейки карты!\n");
            for (int j = 0; j < i; j++) {
                free(map->cells[j]);
            }
            free(map->cells);
            free(map);
            return NULL;
        }
    }

    map->height = height;
    map->width = width;
    map->floor = 1;
    map->mirror_active = 1;
    map->entities = NULL;

    return map;
}

void map_free_cells(struct Map* map) {
    if (map == NULL || map->cells == NULL) {
        return;
    }

    /* Освобождаем строки в обратном порядке выделению */
    for (int i = 0; i < map->height; i++) {
        if (map->cells[i] != NULL) {
            free(map->cells[i]);
            map->cells[i] = NULL;
        }
    }
    free(map->cells);
    map->cells = NULL;
}

void map_destroy(struct Map** map) {
    if (map == NULL || *map == NULL) {
        return;
    }

    if ((*map)->entities != NULL) {
        entities_destroy(&((*map)->entities));
    }

    printf("Память структуры по адресу %p освобождена\n", (void*)(*map));
    map_free_cells(*map);
    free(*map);
    *map = NULL;
}

/* ==================== Генерация башни (Вариант 8 из ЛР 4) ==================== */

static int random_map_side(int min_side) {
    return min_side + (rand() % MAP_SIDE_VARIANTS) * MAP_SIDE_STEP;
}

/* Прорубает Г-образный коридор между двумя клетками: стены превращаются в пол.
   Нужно, чтобы случайные препятствия никогда не отрезали S и U от старта. */
static void carve_path(struct Map* map, int x1, int y1, int x2, int y2) {
    int x = x1;
    int y = y1;

    while (x != x2) {
        x += (x2 > x) ? 1 : -1;
        if (map->cells[y][x] == CELL_WALL) {
            map->cells[y][x] = CELL_FLOOR;
        }
    }
    while (y != y2) {
        y += (y2 > y) ? 1 : -1;
        if (map->cells[y][x] == CELL_WALL) {
            map->cells[y][x] = CELL_FLOOR;
        }
    }
}

void map_generate_tower(struct Map* map, int* hero_x, int* hero_y, int bonus_heals) {
    int height = map->height;
    int width = map->width;
    int mid_x = width / 2;

    /* 1. Внешние стены и внутренний пол */
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (y == 0 || y == height - 1 || x == 0 || x == width - 1) {
                map->cells[y][x] = CELL_WALL;
            } else {
                map->cells[y][x] = CELL_FLOOR;
            }
        }
    }

    /* 2. Разделительная зеркальная стена строго посередине */
    for (int y = 1; y < height - 1; y++) {
        map->cells[y][mid_x] = CELL_MIRROR;
    }

    /* 3. Препятствия в левой половине */
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < mid_x; x++) {
            if (rand() % PERCENT_BASE < WALL_CHANCE_LEFT) {
                map->cells[y][x] = CELL_WALL;
            }
        }
    }

    /* 4. Реальные (другие!) препятствия в правой половине, скрытые за зеркалом */
    for (int y = 1; y < height - 1; y++) {
        for (int x = mid_x + 1; x < width - 1; x++) {
            if (rand() % PERCENT_BASE < WALL_CHANCE_RIGHT) {
                map->cells[y][x] = CELL_WALL;
            }
        }
    }

    /* 5. Лестница вниз D — точка старта в левой половине */
    int start_y = 1 + rand() % (height - 2);
    int start_x = 1 + rand() % (mid_x - 1);
    map->cells[start_y][start_x] = CELL_DOWN;

    *hero_y = start_y;
    *hero_x = start_x;

    /* 6. Переключатель S в левой половине */
    int sw_y, sw_x;
    do {
        sw_y = 1 + rand() % (height - 2);
        sw_x = 1 + rand() % (mid_x - 1);
    } while (sw_y == start_y && sw_x == start_x);
    map->cells[sw_y][sw_x] = CELL_SWITCH;

    /* 7. Лестница вверх U в правой половине */
    int u_y, u_x;
    do {
        u_y = 1 + rand() % (height - 2);
        u_x = (mid_x + 1) + rand() % (width - mid_x - 2);
    } while (map->cells[u_y][u_x] == CELL_WALL);
    map->cells[u_y][u_x] = CELL_UP;

    /* 8. Гарантия проходимости: если случайные стены отрезали переключатель
          или лестницу вверх, прорубаем к ним коридор. */
    carve_path(map, start_x, start_y, sw_x, sw_y);
    carve_path(map, start_x, start_y, u_x, u_y);

    /* 9. Генерация сущностей (ЛР 6): деревья, камни, кусты, монстры */
    if (map->entities != NULL) {
        entities_destroy(&map->entities);
    }
    map->entities = entities_create(height, width, map->cells, map->floor, bonus_heals);
}

/* ==================== Отрисовка ==================== */

void map_print(const struct Map* map, int hero_x, int hero_y) {
    int mid_x = map->width / 2;

    printf("\n=== БАШНЯ: ЭТАЖ %d %s ===\n", map->floor,
        map->mirror_active ? "[ЗЕРКАЛО АКТИВНО]" : "[ИЛЛЮЗИЯ РАССЕЯНА]");

    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            if (map->mirror_active && x > mid_x && x < map->width - 1 &&
                y > 0 && y < map->height - 1) {
                /* Правая половина — оптическое отражение левой */
                int left_x = mid_x - (x - mid_x);
                if (y == hero_y && left_x == hero_x) {
                    printf("%c", CELL_HERO);
                } else {
                    printf("%c", map->cells[y][left_x]);
                }
            } else if (y == hero_y && x == hero_x) {
                printf("%c", CELL_HERO);
            } else {
                printf("%c", map->cells[y][x]);
            }
        }
        printf("\n");
    }

    printf("----------------------------------------\n");
    printf("Управление: [W] - Вверх | [S] - Вниз | [A] - Влево | [D] - Вправо\n");
    printf("            [Q] - Вернуться домой (работает только на клетке D)\n");
    printf("Обозначения: @ - Герой, D - Вниз, U - Вверх, S - Рычаг, | - Зеркало\n");
    printf("             T - Дерево, R - Камень, + - Куст, M - Монстр, E - Энт\n");
}

/* ==================== Режим приключения ==================== */

static void adventure_print_hud(const struct Player* player, const struct Map* map, const struct GameTime* time) {
    int percent = (map->entities != NULL) ? entities_destruction_percent(map->entities) : 0;
    const char* status = (percent > NATURE_WRATH_THRESHOLD) ? " [ГНЕВ ПРИРОДЫ!]" :
                         (percent < NATURE_PEACE_THRESHOLD) ? " [Гармония]" : "";

    printf("\nHP: %d/%d | Выносливость: %d/%d | Золото: %d | Этаж: %d | Время: %02d:00\n",
        player->hp, player->max_hp,
        player->stamina, player->max_stamina,
        player->gold, map->floor, time->hour);

    if (map->entities != NULL && map->entities->total_resources > 0) {
        printf("Баланс этажа: добыто %d/%d ресурсов (%d%%)%s\n",
            map->entities->destroyed_count, map->entities->total_resources,
            percent, status);
    }
}

static char adventure_read_command(void) {
    fflush(stdout);
    char command = 0;
    if (scanf_s(" %c", &command, 1) != 1) {
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();
    return command;
}

/* Экстренная эвакуация: половина золота уходит спасателям */
static void adventure_evacuate(struct Map** map, struct Player* player, const struct GameTime* time, const char* reason) {
    printf("\n>>> %s <<<\n", reason);
    printf(">>> Герой падает без сил! Спасательная служба доставляет его домой. <<<\n");

    int floor_reached = (*map != NULL) ? (*map)->floor : 1;
    map_destroy(map);

    int fee = player->gold / RESCUE_FEE_DIVISOR;
    player->gold -= fee;
    printf("Спасатели забрали половину золота: -%d. Осталось золота: %d\n", fee, player->gold);

    diary_log_expedition(player, time, floor_reached, reason);
}

/* Трата выносливости за шаг с учётом пассивных бонусов профессий */
static void adventure_step_effects(struct Player* player) {
    int free_step = 0;

    if (player->profession == PROF_SCOUT && (rand() % PERCENT_BASE) < SCOUT_FREE_STEP_CHANCE) {
        free_step = 1;
        printf("(Разведчик: лёгкий шаг — выносливость не потрачена)\n");
    }

    if (!free_step) {
        player_spend_stamina(player, STEP_STAMINA_COST);
    }

    if (player->profession == PROF_LUMBERJACK) {
        if (player->hp < player->max_hp) {
            player->hp += LUMBERJACK_HEAL_PER_STEP;
            if (player->hp > player->max_hp) {
                player->hp = player->max_hp;
            }
        }
    }
}

void action_adventure(struct Player* player, struct GameTime* time) {
    if (player == NULL) {
        printf("Сначала создайте персонажа!\n");
        return;
    }
    if (player->stamina <= 0) {
        printf("Вы слишком устали для похода. Отдохните (лечь спать).\n");
        return;
    }
    if (time_is_night(time)) {
        printf("Сейчас 02:00 ночи. Ночью фермер должен спать, а не бродить по башне!\n");
        return;
    }

    int map_height = random_map_side(MAP_MIN_HEIGHT);
    int map_width = random_map_side(MAP_MIN_WIDTH);

    struct Map* map = map_create(map_height, map_width);
    if (map == NULL) {
        return;
    }

    int hero_x = 0;
    int hero_y = 0;
    map_generate_tower(map, &hero_x, &hero_y, 0);

    printf("\nВы входите в загадочную Башню...\n");

    while (1) {
        map_print(map, hero_x, hero_y);
        adventure_print_hud(player, map, time);

        printf("Ваш ход (W-вверх, S-вниз, A-влево, D-вправо, Q-домой): ");
        char command = adventure_read_command();

        if (command == 0) {
            printf("\nВвод завершён — герой покидает башню.\n");
            map_destroy(&map);
            return;
        }

        if (command == 'q' || command == 'Q') {
            if (map->cells[hero_y][hero_x] == CELL_DOWN) {
                printf("\nВы благополучно спустились по лестнице D и вернулись домой!\n");
                printf("Заработанное золото осталось при вас: %d\n", player->gold);
                diary_log_expedition(player, time, map->floor, "Успешное возвращение домой");
                map_destroy(&map);
                return;
            }
            printf("\nНельзя уйти отсюда! Вернуться домой можно только с лестницы вниз (D).\n");
            continue;
        }

        int new_x = hero_x;
        int new_y = hero_y;

        if (command == 'w' || command == 'W') {
            new_y--;
        } else if (command == 's' || command == 'S') {
            new_y++;
        } else if (command == 'a' || command == 'A') {
            new_x--;
        } else if (command == 'd' || command == 'D') {
            new_x++;
        } else {
            printf("\nНеизвестная команда! Используйте W, A, S, D или Q.\n");
            continue;
        }

        /* Проверка выхода за границы массива */
        if (new_y < 0 || new_y >= map->height || new_x < 0 || new_x >= map->width) {
            printf("\nВы не можете выйти за пределы башни!\n");
            continue;
        }

        char target_cell = map->cells[new_y][new_x];

        if (target_cell == CELL_WALL) {
            printf("\nВпереди глухая стена! Прохода нет.\n");
            continue;
        }
        if (target_cell == CELL_MIRROR) {
            printf("\nВпереди магическое зеркало! Найдите переключатель (S), чтобы разрушить иллюзию.\n");
            continue;
        }

        /* Добыча ресурсов (T, R) — методичка ЛР 6 */
        if (target_cell == CELL_TREE || target_cell == CELL_ROCK) {
            struct Entity* e = entity_at(map->entities, new_x, new_y);
            if (e != NULL && e->is_alive) {
                player_spend_stamina(player, ACTION_STAMINA_COST);
                e->hp -= player->attack;
                const char* rname = (target_cell == CELL_TREE) ? "дереву" : "камню";
                printf("\n>>> Удар по %s! Нанесено %d урона. Прочность: %d/%d <<<\n",
                    rname, player->attack, (e->hp > 0 ? e->hp : 0), e->max_hp);

                if (e->hp <= 0) {
                    e->is_alive = 0;
                    map->cells[new_y][new_x] = CELL_FLOOR;
                    map->entities->destroyed_count++;

                    int slot = player_add_item_to_free_slot(player, e->drop_item_id);
                    if (slot >= 0) {
                        printf(">>> Ресурс успешно добыт и помещён в слот [%d] рюкзака! <<<\n", slot);
                    } else {
                        printf(">>> Рюкзак полон! Добытый ресурс потерян. <<<\n");
                    }

                    /* Вариант 5: проверка пробуждения Гнева Природы */
                    if (entities_destruction_percent(map->entities) > NATURE_WRATH_THRESHOLD) {
                        entities_spawn_ents(map->entities, map->height, map->width, map->cells, map->floor);
                    }
                }

                if (player->stamina <= 0) {
                    adventure_evacuate(&map, player, time, "Выносливость упала до нуля при добыче ресурсов!");
                    return;
                }
            }
            continue;
        }

        /* Бой с монстром (M) или Энтом (E) — ЛР 6 */
        if (target_cell == CELL_MONSTER || target_cell == CELL_ENT) {
            struct Entity* e = entity_at(map->entities, new_x, new_y);
            if (e != NULL && e->is_alive) {
                player_spend_stamina(player, ACTION_STAMINA_COST);

                /* Урон героя по формуле: Атака - Защита/2 (минимум 1) */
                int hero_damage = player->attack - (e->defense / DEFENSE_DIVISOR);
                if (hero_damage < MIN_DAMAGE) {
                    hero_damage = MIN_DAMAGE;
                }
                e->hp -= hero_damage;

                const char* enemy_name = (target_cell == CELL_ENT) ? "Энту" : "монстру";
                printf("\n>>> Вы атаковали %s! Нанесено %d урона. HP врага: %d/%d <<<\n",
                    enemy_name, hero_damage, (e->hp > 0 ? e->hp : 0), e->max_hp);

                if (e->hp <= 0) {
                    e->is_alive = 0;
                    map->cells[new_y][new_x] = CELL_FLOOR;
                    map->entities->destroyed_count++;

                    player_add_gold(player, e->gold_reward);
                    printf(">>> Враг повержен! Награда: +%d золота (всего: %d) <<<\n",
                        e->gold_reward, player->gold);

                    if (e->drop_item_id != ITEM_NONE) {
                        int slot = player_add_item_to_free_slot(player, e->drop_item_id);
                        if (slot >= 0) {
                            printf(">>> Получен трофей в слот [%d]! <<<\n", slot);
                        }
                    }
                } else {
                    /* Ответный удар врага: Атака - Защита/2 (минимум 1) */
                    int monster_damage = e->attack - (player->defense / DEFENSE_DIVISOR);
                    if (monster_damage < MIN_DAMAGE) {
                        monster_damage = MIN_DAMAGE;
                    }
                    player->hp -= monster_damage;
                    printf(">>> Враг контратакует! Получено %d урона. Ваше HP: %d/%d <<<\n",
                        monster_damage, (player->hp > 0 ? player->hp : 0), player->max_hp);

                    if (player->hp <= 0) {
                        player->hp = 0;
                        adventure_evacuate(&map, player, time, "Герой пал в бою!");
                        return;
                    }
                }

                if (player->stamina <= 0) {
                    adventure_evacuate(&map, player, time, "Выносливость упала до нуля в бою!");
                    return;
                }
            }
            continue;
        }

        /* Сбор целебного куста (+) — методичка ЛР 6 */
        if (target_cell == CELL_HEAL) {
            struct Entity* e = entity_at(map->entities, new_x, new_y);
            if (e != NULL) {
                e->is_alive = 0;
            }
            map->cells[new_y][new_x] = CELL_FLOOR;

            player->hp += HEAL_HP_RESTORE;
            if (player->hp > player->max_hp) {
                player->hp = player->max_hp;
            }
            player->stamina += HEAL_STAMINA_RESTORE;
            if (player->stamina > player->max_stamina) {
                player->stamina = player->max_stamina;
            }

            printf("\n>>> Вы съели целебные ягоды с куста! +%d HP, +%d выносливости! <<<\n",
                HEAL_HP_RESTORE, HEAL_STAMINA_RESTORE);
            continue;
        }

        /* Успешный шаг */
        hero_x = new_x;
        hero_y = new_y;
        player->x = hero_x;
        player->y = hero_y;

        adventure_step_effects(player);

        if (player->stamina <= 0) {
            adventure_evacuate(&map, player, time, "Выносливость упала до нуля!");
            return;
        }

        /* Переключатель: зеркало разбивается, открывается реальная комната */
        if (target_cell == CELL_SWITCH) {
            printf("\n>>> ЩЁЛК! Вы нажали переключатель (S)! <<<\n");
            printf(">>> Зеркало разбилось! Иллюзия рассеялась, обнажив НАСТОЯЩИЕ препятствия! <<<\n");

            map->mirror_active = 0;
            int mid_x = map->width / 2;
            for (int y = 1; y < map->height - 1; y++) {
                if (map->cells[y][mid_x] == CELL_MIRROR) {
                    map->cells[y][mid_x] = CELL_FLOOR;
                }
            }
            map->cells[hero_y][hero_x] = CELL_FLOOR;
        }

        /* Переход на новый этаж */
        if (target_cell == CELL_UP) {
            player_spend_stamina(player, FLOOR_STAMINA_COST);
            time_add_hours(time, 1);

            /* Вариант 5: проверка бережливости (<10% ресурсов уничтожено) */
            int was_peaceful = (entities_destruction_percent(map->entities) < NATURE_PEACE_THRESHOLD);
            if (was_peaceful) {
                printf("\n>>> ПРИРОДА БЛАГОДАРНА: за бережное отношение на новом этаже кустов x2! <<<\n");
            }

            map->floor++;

            int reward = GOLD_PER_FLOOR * map->floor * player_gold_percent(player) / PERCENT_BASE;
            player_add_gold(player, reward);

            printf("\n>>> Вы поднялись на этаж %d! <<<\n", map->floor);
            printf(">>> Награда за этаж: +%d золота (всего: %d) <<<\n", reward, player->gold);

            if (time_is_night(time)) {
                adventure_evacuate(&map, player, time, "Наступило 02:00 ночи — пора спать!");
                return;
            }
            if (player->stamina <= 0) {
                adventure_evacuate(&map, player, time, "Выносливость упала до нуля!");
                return;
            }

            /* Пересоздаём карту нового этажа: старая память освобождается
               с обязательным отладочным сообщением внутри map_destroy */
            int next_floor = map->floor;

            map_destroy(&map);

            map = map_create(random_map_side(MAP_MIN_HEIGHT), random_map_side(MAP_MIN_WIDTH));
            if (map == NULL) {
                return;
            }

            map->floor = next_floor;
            map_generate_tower(map, &hero_x, &hero_y, was_peaceful);
            player->x = hero_x;
            player->y = hero_y;
        }
    }
}

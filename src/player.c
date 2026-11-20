#include "player.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==================== Профессии (Вариант 1) ==================== */

const char* profession_name(enum Profession profession) {
    switch (profession) {
    case PROF_FARMER:     return "Фермер";
    case PROF_LUMBERJACK: return "Лесоруб";
    case PROF_MINER:      return "Шахтёр";
    case PROF_SCOUT:      return "Разведчик";
    default:              return "Неизвестно";
    }
}

const char* profession_description(enum Profession profession) {
    switch (profession) {
    case PROF_FARMER:     return "сбалансированный, +20% золота за этаж";
    case PROF_LUMBERJACK: return "много здоровья, +1 HP за каждый шаг";
    case PROF_MINER:      return "максимум здоровья, +30% золота за этаж";
    case PROF_SCOUT:      return "много выносливости, 50% шанс бесплатного шага";
    default:              return "";
    }
}

int profession_start_hp(enum Profession profession) {
    switch (profession) {
    case PROF_LUMBERJACK: return LUMBERJACK_HP;
    case PROF_MINER:      return MINER_HP;
    case PROF_SCOUT:      return SCOUT_HP;
    case PROF_FARMER:
    default:              return FARMER_HP;
    }
}

int profession_start_stamina(enum Profession profession) {
    switch (profession) {
    case PROF_LUMBERJACK: return LUMBERJACK_STAMINA;
    case PROF_MINER:      return MINER_STAMINA;
    case PROF_SCOUT:      return SCOUT_STAMINA;
    case PROF_FARMER:
    default:              return FARMER_STAMINA;
    }
}

int profession_start_attack(enum Profession profession) {
    switch (profession) {
    case PROF_LUMBERJACK: return LUMBERJACK_ATTACK;
    case PROF_MINER:      return MINER_ATTACK;
    case PROF_SCOUT:      return SCOUT_ATTACK;
    case PROF_FARMER:
    default:              return FARMER_ATTACK;
    }
}

int profession_start_defense(enum Profession profession) {
    switch (profession) {
    case PROF_LUMBERJACK: return LUMBERJACK_DEFENSE;
    case PROF_MINER:      return MINER_DEFENSE;
    case PROF_SCOUT:      return SCOUT_DEFENSE;
    case PROF_FARMER:
    default:              return FARMER_DEFENSE;
    }
}

/* Множитель золота за этаж в процентах (пассивный бонус профессии) */
int player_gold_percent(const struct Player* player) {
    if (player == NULL) {
        return NORMAL_GOLD_PERCENT;
    }
    switch (player->profession) {
    case PROF_FARMER: return FARMER_GOLD_PERCENT;
    case PROF_MINER:  return MINER_GOLD_PERCENT;
    default:          return NORMAL_GOLD_PERCENT;
    }
}

/* ==================== Жизненный цикл ==================== */

static enum Profession choose_profession(void) {
    printf("\nВыберите профессию:\n");
    printf("[1] Фермер     — %s\n", profession_description(PROF_FARMER));
    printf("[2] Лесоруб    — %s\n", profession_description(PROF_LUMBERJACK));
    printf("[3] Шахтёр     — %s\n", profession_description(PROF_MINER));
    printf("[4] Разведчик  — %s\n", profession_description(PROF_SCOUT));

    int choice = 0;
    if (!read_int("Ваш выбор: ", PROF_FARMER, PROF_SCOUT, &choice)) {
        printf("Выбрана профессия по умолчанию: Фермер.\n");
        return PROF_FARMER;
    }

    /* Номера пунктов меню совпадают со значениями enum Profession */
    return (enum Profession)choice;
}

struct Player* player_create(void) {
    struct Player* player = (struct Player*)malloc(sizeof(struct Player));
    if (player == NULL) {
        printf("Ошибка: не удалось выделить память под структуру игрока!\n");
        return NULL;
    }

    char name[MAX_PLAYER_NAME];
    if (!read_word("\nВведите имя фермера (без пробелов): ", name, MAX_PLAYER_NAME)) {
        strcpy_s(name, MAX_PLAYER_NAME, "Фермер");
    }
    strcpy_s(player->name, MAX_PLAYER_NAME, name);

    player->profession = choose_profession();
    player->max_hp = profession_start_hp(player->profession);
    player->hp = player->max_hp;
    player->max_stamina = profession_start_stamina(player->profession);
    player->stamina = player->max_stamina;
    player->attack = profession_start_attack(player->profession);
    player->defense = profession_start_defense(player->profession);
    player->gold = START_GOLD;
    player->x = 0;
    player->y = 0;

    /* Стартовый набор предметов, чтобы можно было сразу играть */
    int start_items[INVENTORY_SIZE] = {
        ITEM_WOOD, ITEM_STONE, ITEM_SEEDS, ITEM_NONE, ITEM_WHEAT,
        ITEM_PEAR, ITEM_NONE, ITEM_WATER, ITEM_WOOD, ITEM_GOLD
    };
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        player->inventory[i] = start_items[i];
    }

    printf("\nПерсонаж создан! Память выделена по адресу %p\n", (void*)player);
    printf("Профессия: %s (%s)\n", profession_name(player->profession), profession_description(player->profession));
    printf("HP: %d/%d | Выносливость: %d/%d | Атака: %d | Защита: %d | Золото: %d\n",
        player->hp, player->max_hp, player->stamina, player->max_stamina,
        player->attack, player->defense, player->gold);

    return player;
}

void player_destroy(struct Player** player) {
    if (player == NULL || *player == NULL) {
        return;
    }

    printf("Память структуры по адресу %p освобождена\n", (void*)(*player));
    free(*player);
    *player = NULL;
}

/* ==================== Характеристики ==================== */

int player_has_stamina(const struct Player* player) {
    return player != NULL && player->stamina > 0;
}

void player_spend_stamina(struct Player* player, int amount) {
    if (player == NULL || amount <= 0) {
        return;
    }
    player->stamina -= amount;
    if (player->stamina < 0) {
        player->stamina = 0;
    }
}

void player_add_gold(struct Player* player, int amount) {
    if (player == NULL) {
        return;
    }
    player->gold += amount;
    if (player->gold < 0) {
        player->gold = 0;
    }
}

void player_restore_full(struct Player* player) {
    if (player == NULL) {
        return;
    }
    player->hp = player->max_hp;
    player->stamina = player->max_stamina;
}

/* ==================== Инвентарь ==================== */

void player_show_inventory(const struct Player* player, char names[MAX_ITEMS][MAX_NAME_LEN]) {
    printf("--- Инвентарь (%s) ---\n", player->name);
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int item_id = player->inventory[i];
        if (item_id >= 0 && item_id < MAX_ITEMS) {
            printf("Слот %d: [%d] - %s\n", i, item_id, names[item_id]);
        } else {
            printf("Слот %d: [%d] - Неизвестно\n", i, item_id);
        }
    }
}

void player_put_item(struct Player* player) {
    int slot = 0;
    if (!read_int("Слот (0-9): ", 0, INVENTORY_SIZE - 1, &slot)) {
        return;
    }

    int item_id = 0;
    if (!read_int("ID предмета (0-19): ", 0, MAX_ITEMS - 1, &item_id)) {
        return;
    }

    player->inventory[slot] = item_id;
    printf("В слот %d положен предмет с ID %d.\n", slot, item_id);
}

void player_drop_item(struct Player* player) {
    int slot = 0;
    if (!read_int("Слот для очистки (0-9): ", 0, INVENTORY_SIZE - 1, &slot)) {
        return;
    }

    player->inventory[slot] = 0;
    printf("Слот %d очищен.\n", slot);
}

void player_check_adjacent(const struct Player* player) {
    int id1 = 0;
    int id2 = 0;

    if (!read_int("ID первого предмета: ", 0, MAX_ITEMS - 1, &id1)) {
        return;
    }
    if (!read_int("ID второго предмета: ", 0, MAX_ITEMS - 1, &id2)) {
        return;
    }

    int found = 0;
    for (int i = 0; i < INVENTORY_SIZE - 1; i++) {
        if ((player->inventory[i] == id1 && player->inventory[i + 1] == id2) ||
            (player->inventory[i] == id2 && player->inventory[i + 1] == id1)) {
            printf("Предметы лежат рядом в слотах: %d и %d\n", i, i + 1);
            found = 1;
        }
    }

    if (!found) {
        printf("Предметы не лежат в соседних слотах.\n");
    }
}

void player_search_by_name(const struct Player* player, char names[MAX_ITEMS][MAX_NAME_LEN]) {
    char search_term[MAX_NAME_LEN];
    if (!read_word("Введите точное название предмета для поиска: ", search_term, MAX_NAME_LEN)) {
        return;
    }

    int target_id = -1;
    for (int i = 1; i < MAX_ITEMS; i++) {
        if (strcmp(names[i], search_term) == 0) {
            target_id = i;
            break;
        }
    }

    if (target_id == -1) {
        printf("Предмет '%s' не существует в каталоге.\n", search_term);
        return;
    }

    int count = 0;
    printf("Найден в слотах: ");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (player->inventory[i] == target_id) {
            printf("%d ", i);
            count++;
        }
    }

    if (count > 0) {
        printf("\nВсего штук в рюкзаке: %d\n", count);
    } else {
        printf("Нет в рюкзаке.\n");
    }
}

/* ==================== Карточка персонажа ==================== */

void player_print_card(const struct Player* player, char names[MAX_ITEMS][MAX_NAME_LEN], const struct GameTime* time) {
    printf("\n=== КАРТОЧКА ПЕРСОНАЖА ===\n");
    printf("Имя:          %s\n", player->name);
    printf("Профессия:    %s (%s)\n", profession_name(player->profession), profession_description(player->profession));
    printf("Здоровье:     %d/%d\n", player->hp, player->max_hp);
    printf("Выносливость: %d/%d\n", player->stamina, player->max_stamina);
    printf("Атака:        %d\n", player->attack);
    printf("Защита:       %d\n", player->defense);
    printf("Золото:       %d\n", player->gold);
    printf("Время:        День %d, %02d:00\n", time->day, time->hour);
    printf("---------------------------\n");
    player_show_inventory(player, names);
}

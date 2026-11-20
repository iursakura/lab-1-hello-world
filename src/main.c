#include "config.h"
#include "player.h"
#include "map.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Перечисление пунктов главного меню */
enum MenuOption {
    MENU_EXIT = 0,
    MENU_CREATE_PLAYER = 1,
    MENU_PLAYER_CARD,
    MENU_TIME_MENU,
    MENU_INVENTORY,
    MENU_PUT_ITEM,
    MENU_DROP_ITEM,
    MENU_CHECK_ADJACENT,
    MENU_SEARCH_ITEM,
    MENU_DIARY,
    MENU_QUESTS,
    MENU_ADVENTURE
};

/* Перечисление пунктов подменю времени */
enum TimeMenuOption {
    TIME_BACK = 0,
    TIME_SHOW = 1,
    TIME_WAIT,
    TIME_SLEEP
};

static void print_no_player(void) {
    printf("Сначала создайте персонажа (пункт [1])!\n");
}

static void display_menu(const struct Player* player) {
    printf("\n=== ВЕСЁЛЫЙ ФЕРМЕР ===\n");
    if (player == NULL) {
        printf("Персонаж: не создан\n");
    } else {
        printf("Персонаж: %s (%s) | HP %d/%d | Выносливость %d/%d | Золото %d\n",
            player->name, profession_name(player->profession),
            player->hp, player->max_hp,
            player->stamina, player->max_stamina,
            player->gold);
    }
    printf("[1] Создать / пересоздать персонажа\n");
    printf("[2] Карточка персонажа\n");
    printf("[3] Управление временем\n");
    printf("[4] Посмотреть инвентарь\n");
    printf("[5] Положить предмет в слот\n");
    printf("[6] Выбросить предмет\n");
    printf("[7] Проверка соседних ячеек (ЛР 2)\n");
    printf("[8] Поиск предмета по названию\n");
    printf("[9] Записать состояние в дневник (diary.txt)\n");
    printf("[10] Доска объявлений (Квесты)\n");
    printf("[11] Отправиться в приключение\n");
    printf("[0] Выход\n");
}

static void time_menu(struct GameTime* time, struct Player** player) {
    int back = 0;

    while (!back) {
        printf("\n=== УПРАВЛЕНИЕ ВРЕМЕНЕМ ===\n");
        printf("[1] Посмотреть на часы\n");
        printf("[2] Пропустить время (Ждать)\n");
        printf("[3] Отдых (Лечь спать)\n");
        printf("[0] Назад\n");

        int choice = 0;
        if (!read_int("Выберите пункт: ", TIME_BACK, TIME_SLEEP, &choice)) {
            continue;
        }

        switch (choice) {
        case TIME_SHOW:
            time_show(time);
            break;
        case TIME_WAIT:
            time_wait(time);
            break;
        case TIME_SLEEP:
            time_sleep(time, *player);
            break;
        case TIME_BACK:
        default:
            back = 1;
            break;
        }
    }
}

int main(void) {
    system("chcp 65001 > nul");
    srand((unsigned)time(NULL));

    struct GameTime time = { START_DAY, START_HOUR };
    struct Player* player = NULL;

    char item_names[MAX_ITEMS][MAX_NAME_LEN];
    load_item_names(item_names);

    int running = 1;

    while (running) {
        display_menu(player);

        int choice = 0;
        if (!read_int("Выберите пункт: ", MENU_EXIT, MENU_ADVENTURE, &choice)) {
            continue;
        }

        switch (choice) {
        case MENU_EXIT:
            printf("Выход из игры...\n");
            running = 0;
            break;

        case MENU_CREATE_PLAYER:
            if (player != NULL) {
                if (!ask_yes_no("Персонаж уже существует. Удалить его и создать нового? (1 - да, 0 - нет): ")) {
                    printf("Создание отменено.\n");
                    break;
                }
                player_destroy(&player);
            }
            player = player_create();
            break;

        case MENU_PLAYER_CARD:
            if (player == NULL) {
                print_no_player();
                break;
            }
            player_print_card(player, item_names, &time);
            break;

        case MENU_TIME_MENU:
            time_menu(&time, &player);
            break;

        case MENU_INVENTORY:
            if (player == NULL) {
                print_no_player();
                break;
            }
            player_show_inventory(player, item_names);
            break;

        case MENU_PUT_ITEM:
            if (player == NULL) {
                print_no_player();
                break;
            }
            player_put_item(player);
            break;

        case MENU_DROP_ITEM:
            if (player == NULL) {
                print_no_player();
                break;
            }
            player_drop_item(player);
            break;

        case MENU_CHECK_ADJACENT:
            if (player == NULL) {
                print_no_player();
                break;
            }
            player_check_adjacent(player);
            break;

        case MENU_SEARCH_ITEM:
            if (player == NULL) {
                print_no_player();
                break;
            }
            player_search_by_name(player, item_names);
            break;

        case MENU_DIARY:
            if (player == NULL) {
                print_no_player();
                break;
            }
            action_write_diary(player, &time, item_names);
            break;

        case MENU_QUESTS:
            if (player == NULL) {
                print_no_player();
                break;
            }
            action_notice_board(&time, player, item_names);
            break;

        case MENU_ADVENTURE:
            if (player == NULL) {
                print_no_player();
                break;
            }
            action_adventure(player, &time);
            break;

        default:
            printf("Неверный пункт меню!\n");
            break;
        }
    }

    player_destroy(&player);
    printf("Программа завершена. Вся динамическая память освобождена.\n");
    return 0;
}

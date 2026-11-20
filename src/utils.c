#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==================== Ввод пользователя ==================== */

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* выбрасываем всё до конца строки */
    }
}

int read_int(const char* prompt, int min_value, int max_value, int* out_value) {
    if (prompt != NULL) {
        printf("%s", prompt);
        fflush(stdout);
    }

    int value = 0;
    if (scanf_s("%d", &value) != 1) {
        if (feof(stdin)) {
            exit(0);
        }
        printf("Ошибка! Введите число.\n");
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();

    if (value < min_value || value > max_value) {
        printf("Ошибка! Допустимый диапазон: от %d до %d.\n", min_value, max_value);
        return 0;
    }

    *out_value = value;
    return 1;
}

int read_word(const char* prompt, char* out, int size) {
    if (prompt != NULL) {
        printf("%s", prompt);
        fflush(stdout);
    }

    if (scanf_s("%31s", out, (unsigned)size) != 1) {
        if (feof(stdin)) {
            exit(0);
        }
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();
    return 1;
}

int ask_yes_no(const char* question) {
    printf("%s", question);
    int answer = 0;
    if (scanf_s("%d", &answer) != 1) {
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();
    return answer == 1;
}

/* ==================== Время и режим дня ==================== */

void time_show(const struct GameTime* time) {
    printf("Текущее время: День %d, %02d:00\n", time->day, time->hour);
}

/* Сколько часов можно ждать до ближайших 02:00 ночи */
int time_hours_until_night(const struct GameTime* time) {
    int allowed = (NIGHT_LIMIT_HOUR - time->hour + HOURS_IN_DAY) % HOURS_IN_DAY;
    if (allowed == 0) {
        allowed = HOURS_IN_DAY;
    }
    return allowed;
}

void time_add_hours(struct GameTime* time, int hours) {
    if (hours <= 0) {
        return;
    }
    time->hour += hours;
    while (time->hour >= HOURS_IN_DAY) {
        time->hour -= HOURS_IN_DAY;
        time->day++;
    }
}

int time_is_night(const struct GameTime* time) {
    return time->hour == NIGHT_LIMIT_HOUR;
}

/* Пропустить время: нельзя перескочить через 02:00 ночи */
int time_wait(struct GameTime* time) {
    int allowed = time_hours_until_night(time);
    printf("Сейчас День %d, %02d:00. До 02:00 ночи можно подождать не более %d ч.\n",
        time->day, time->hour, allowed);

    int hours = 0;
    if (!read_int("Сколько часов ждать? ", 1, allowed, &hours)) {
        printf("Ожидание отменено.\n");
        return 0;
    }

    time_add_hours(time, hours);
    printf("Время промотано! Текущее время: День %d, %02d:00\n", time->day, time->hour);
    return 1;
}

/* Отдых: следующий день, 08:00, полное восстановление сил */
void time_sleep(struct GameTime* time, struct Player* player) {
    time->day++;
    time->hour = SLEEP_HOUR;

    printf("Вы хорошо выспались. Наступил День %d, %02d:00.\n", time->day, time->hour);

    if (player != NULL) {
        player_restore_full(player);
        printf("Здоровье и выносливость полностью восстановлены: HP %d/%d, Выносливость %d/%d.\n",
            player->hp, player->max_hp, player->stamina, player->max_stamina);
    } else {
        printf("Персонаж не создан, поэтому восстанавливать нечего.\n");
    }
}

/* ==================== Предметы ==================== */

void load_item_names(char names[MAX_ITEMS][MAX_NAME_LEN]) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        strcpy_s(names[i], MAX_NAME_LEN, "Неизвестно");
    }
    strcpy_s(names[ITEM_NONE], MAX_NAME_LEN, "Пусто");
    strcpy_s(names[ITEM_WOOD], MAX_NAME_LEN, "Дерево");
    strcpy_s(names[ITEM_STONE], MAX_NAME_LEN, "Камень");
    strcpy_s(names[ITEM_SEEDS], MAX_NAME_LEN, "Семена");
    strcpy_s(names[ITEM_WHEAT], MAX_NAME_LEN, "Пшеница");
    strcpy_s(names[ITEM_PEAR], MAX_NAME_LEN, "Яблоко");
    strcpy_s(names[ITEM_SHOVEL], MAX_NAME_LEN, "Лопата");
    strcpy_s(names[ITEM_WATER], MAX_NAME_LEN, "Вода");
    strcpy_s(names[ITEM_FERTILIZER], MAX_NAME_LEN, "Удобрение");
    strcpy_s(names[ITEM_GOLD], MAX_NAME_LEN, "Золото");

    FILE* f;
    if (fopen_s(&f, "items.txt", "r") != 0 || f == NULL) {
        printf("Предупреждение: файл items.txt не найден! Используются базовые названия.\n");
        return;
    }

    int id;
    char temp_name[MAX_NAME_LEN];
    while (fscanf_s(f, "%d %31s", &id, temp_name, (unsigned)MAX_NAME_LEN) == 2) {
        if (id >= 0 && id < MAX_ITEMS) {
            strcpy_s(names[id], MAX_NAME_LEN, temp_name);
        }
    }
    fclose(f);
}

/* ==================== Дневник ==================== */

void action_write_diary(const struct Player* player, const struct GameTime* time, char names[MAX_ITEMS][MAX_NAME_LEN]) {
    FILE* f;
    if (fopen_s(&f, "diary.txt", "a") != 0 || f == NULL) {
        printf("Ошибка: не удалось открыть diary.txt для записи!\n");
        return;
    }

    fprintf(f, "==============================\n");
    fprintf(f, "Фермер: %s (%s)\n", player->name, profession_name(player->profession));
    fprintf(f, "Запись от: День %d, %02d:00\n", time->day, time->hour);
    fprintf(f, "HP: %d/%d | Выносливость: %d/%d | Золото: %d\n",
        player->hp, player->max_hp, player->stamina, player->max_stamina, player->gold);
    fprintf(f, "Состояние рюкзака:\n");

    int empty_count = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (player->inventory[i] == 0) {
            empty_count++;
        } else {
            fprintf(f, " - Слот %d: %s\n", i, names[player->inventory[i]]);
        }
    }
    fprintf(f, "Пустых слотов: %d\n", empty_count);
    fprintf(f, "==============================\n\n");

    fclose(f);
    printf("Состояние успешно записано в diary.txt!\n");
}

/* Автоматическая запись отчёта об экспедиции (ЛР 6) */
void diary_log_expedition(const struct Player* player, const struct GameTime* time, int floor_reached, const char* outcome) {
    if (player == NULL || time == NULL) {
        return;
    }

    FILE* f = NULL;
    if (fopen_s(&f, "diary.txt", "a") != 0 || f == NULL) {
        return;
    }

    fprintf(f, "=== ОТЧЁТ ОБ ЭКСПЕДИЦИИ В БАШНЮ ===\n");
    fprintf(f, "Герой: %s (%s)\n", player->name, profession_name(player->profession));
    fprintf(f, "Время: День %d, %02d:00\n", time->day, time->hour);
    fprintf(f, "Достигнутый этаж: %d\n", floor_reached);
    fprintf(f, "Итог: %s\n", outcome != NULL ? outcome : "Завершено");
    fprintf(f, "Здоровье: %d/%d | Золото: %d\n", player->hp, player->max_hp, player->gold);
    fprintf(f, "====================================\n\n");

    fclose(f);
    printf("Отчёт об экспедиции автоматически сохранён в diary.txt\n");
}

/* ==================== Доска объявлений (Вариант 9 из ЛР 3) ==================== */

void action_notice_board(const struct GameTime* time, const struct Player* player, char names[MAX_ITEMS][MAX_NAME_LEN]) {
    FILE* fin = NULL;
    FILE* fout = NULL;

    if (fopen_s(&fin, "input.txt", "r") != 0 || fin == NULL) {
        printf("Ошибка: не удалось найти файл input.txt! Создайте его в проекте.\n");
        return;
    }

    if (fopen_s(&fout, "output.txt", "w") != 0 || fout == NULL) {
        printf("Ошибка: не удалось создать файл output.txt!\n");
        fclose(fin);
        return;
    }

    int q_day, q_reward;
    char q_cust[MAX_NAME_LEN], q_item[MAX_NAME_LEN];
    int active_count = 0;

    printf("\n--- ДОСТУПНЫЕ КВЕСТЫ (День %d) ---\n", time->day);
    fprintf(fout, "--- ДОСТУПНЫЕ КВЕСТЫ (День %d) ---\n", time->day);

    while (fscanf_s(fin, "%d | %31s | %31s | %d",
        &q_day, q_cust, (unsigned)MAX_NAME_LEN, q_item, (unsigned)MAX_NAME_LEN, &q_reward) == 4) {

        if (q_day > time->day) {
            continue;
        }

        active_count++;

        int found_in_inventory = 0;
        for (int i = 0; i < INVENTORY_SIZE; i++) {
            int item_id = player->inventory[i];
            if (item_id > 0 && item_id < MAX_ITEMS && strcmp(names[item_id], q_item) == 0) {
                found_in_inventory = 1;
                break;
            }
        }

        if (found_in_inventory) {
            printf("%s просит: %s (Награда: %d) -> [ГОТОВО К СДАЧЕ]\n", q_cust, q_item, q_reward);
            fprintf(fout, "%s просит: %s (Награда: %d) -> [ГОТОВО К СДАЧЕ]\n", q_cust, q_item, q_reward);
        } else {
            printf("%s просит: %s (Награда: %d) -> [В ПРОЦЕССЕ]\n", q_cust, q_item, q_reward);
            fprintf(fout, "%s просит: %s (Награда: %d) -> [В ПРОЦЕССЕ]\n", q_cust, q_item, q_reward);
        }
    }

    if (active_count == 0) {
        printf("На сегодня доступных заданий нет.\n");
        fprintf(fout, "На сегодня доступных заданий нет.\n");
    }

    fclose(fin);
    fclose(fout);

    printf("----------------------------------\n");
    printf("(Результат также успешно сохранен в файл output.txt для проверки)\n");
}

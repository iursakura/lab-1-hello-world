#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INVENTORY_SIZE 10
#define MAX_ITEMS 20
#define MAX_NAME_LEN 32
#define MAX_PLAYER_NAME 32

// --- ПРОТОТИПЫ ФУНКЦИЙ ---
void load_item_names(char names[MAX_ITEMS][MAX_NAME_LEN]);
void ask_player_name(char* name);
void display_menu(const char* name);
int get_valid_input();
void action_show_time(int day, int hour);
void action_work(int* day, int* hour);
void action_show_inventory(const int inv[], char names[MAX_ITEMS][MAX_NAME_LEN]);
void action_put_item(int inv[]);
void action_drop_item(int inv[]);
void action_check_adjacent(const int inv[]);
void action_search_by_name(const int inv[], char names[MAX_ITEMS][MAX_NAME_LEN]);
void action_write_diary(const char* name, int day, int hour, const int inv[], char names[MAX_ITEMS][MAX_NAME_LEN]);
void action_notice_board(int current_day, const int inv[], char names[MAX_ITEMS][MAX_NAME_LEN]);

// --- ОСНОВНАЯ ФУНКЦИЯ ---
int main() {
    system("chcp 65001 > nul");

    int current_day = 1;
    int current_hour = 8;
    int inventory[INVENTORY_SIZE] = { 1, 2, 3, 0, 4, 5, 0, 7, 1, 9 };

    char item_names[MAX_ITEMS][MAX_NAME_LEN];
    char player_name[MAX_PLAYER_NAME];

    load_item_names(item_names);
    ask_player_name(player_name);

    while (1) {
        display_menu(player_name);
        int choice = get_valid_input();

        switch (choice) {
        case 0: printf("Выход из игры...\n"); return 0;
        case 1: action_show_time(current_day, current_hour); break;
        case 2: action_work(&current_day, &current_hour); break;
        case 3: action_show_inventory(inventory, item_names); break;
        case 4: action_put_item(inventory); break;
        case 5: action_drop_item(inventory); break;
        case 6: action_check_adjacent(inventory); break;
        case 7: action_search_by_name(inventory, item_names); break;
        case 8: action_write_diary(player_name, current_day, current_hour, inventory, item_names); break;
        case 9: action_notice_board(current_day, inventory, item_names); break;
        default: printf("Неверный пункт меню!\n"); break;
        }
    }
    return 0;
}

// --- РЕАЛИЗАЦИЯ ФУНКЦИЙ ---

void load_item_names(char names[MAX_ITEMS][MAX_NAME_LEN]) {
    for (int i = 0; i < MAX_ITEMS; i++) strcpy_s(names[i], MAX_NAME_LEN, "Неизвестно");
    strcpy_s(names[0], MAX_NAME_LEN, "Пусто");
    strcpy_s(names[1], MAX_NAME_LEN, "Дерево");
    strcpy_s(names[2], MAX_NAME_LEN, "Камень");
    strcpy_s(names[3], MAX_NAME_LEN, "Семена");
    strcpy_s(names[4], MAX_NAME_LEN, "Пшеница");
    strcpy_s(names[5], MAX_NAME_LEN, "Яблоко");
    strcpy_s(names[6], MAX_NAME_LEN, "Лопата");
    strcpy_s(names[7], MAX_NAME_LEN, "Вода");
    strcpy_s(names[8], MAX_NAME_LEN, "Удобрение");
    strcpy_s(names[9], MAX_NAME_LEN, "Золото");

    FILE* f;
    if (fopen_s(&f, "items.txt", "r") != 0 || f == NULL) {
        printf("Предупреждение: файл items.txt не найден! Используются базовые названия.\n");
        return;
    }
    int id;
    char temp_name[MAX_NAME_LEN];
    while (fscanf_s(f, "%d %31s", &id, temp_name, (unsigned)MAX_NAME_LEN) == 2) {
        if (id >= 0 && id < MAX_ITEMS) strcpy_s(names[id], MAX_NAME_LEN, temp_name);
    }
    fclose(f);
}

void ask_player_name(char* name) {
    printf("\nДобро пожаловать! Введите имя фермера (без пробелов): ");
    if (scanf_s("%31s", name, (unsigned)MAX_PLAYER_NAME) != 1) {
        strcpy_s(name, MAX_PLAYER_NAME, "Фермер");
    }
    while (getchar() != '\n');
}

void display_menu(const char* name) {
    printf("\n=== ВЕСЕЛЫЙ ФЕРМЕР: %s ===\n", name);
    printf("[1] Посмотреть на часы\n");
    printf("[2] Промотать время (Поработать)\n");
    printf("[3] Посмотреть инвентарь\n");
    printf("[4] Положить предмет в слот\n");
    printf("[5] Выбросить предмет\n");
    printf("[6] Проверка соседних ячеек (ЛР 2)\n");
    printf("[7] Поиск предмета по названию\n");
    printf("[8] Записать состояние в дневник (diary.txt)\n");
    printf("[9] Доска объявлений (Квесты)\n");
    printf("[0] Выход\n");
    printf("Выберите пункт: ");
}

int get_valid_input() {
    int choice = -1;
    if (scanf_s("%d", &choice) != 1) {
        printf("Ошибка! Введите число.\n");
        while (getchar() != '\n');
        return -1;
    }
    return choice;
}

void action_show_time(int day, int hour) { printf("Текущее время: День %d, %02d:00\n", day, hour); }

void action_work(int* day, int* hour) {
    int hours_to_work = 0;
    printf("Сколько часов поработать? ");
    if (scanf_s("%d", &hours_to_work) != 1 || hours_to_work < 0) {
        printf("Ошибка ввода!\n"); while (getchar() != '\n'); return;
    }
    *hour += hours_to_work;
    if (*hour >= 24) { *day += *hour / 24; *hour = *hour % 24; }
    printf("Время промотано! Текущее время: День %d, %02d:00\n", *day, *hour);
}

void action_show_inventory(const int inv[], char names[MAX_ITEMS][MAX_NAME_LEN]) {
    printf("--- Инвентарь ---\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int item_id = inv[i];
        if (item_id >= 0 && item_id < MAX_ITEMS) printf("Слот %d: [%d] - %s\n", i, item_id, names[item_id]);
        else printf("Слот %d: [%d] - Неизвестно\n", i, item_id);
    }
}

void action_put_item(int inv[]) {
    int slot = -1, item_id = 0;
    printf("Слот (0-9): ");
    if (scanf_s("%d", &slot) != 1 || slot < 0 || slot >= INVENTORY_SIZE) { while (getchar() != '\n'); return; }
    printf("ID предмета: ");
    if (scanf_s("%d", &item_id) != 1) { while (getchar() != '\n'); return; }
    inv[slot] = item_id;
}

void action_drop_item(int inv[]) {
    int slot = -1;
    printf("Слот для очистки (0-9): ");
    if (scanf_s("%d", &slot) != 1 || slot < 0 || slot >= INVENTORY_SIZE) { while (getchar() != '\n'); return; }
    inv[slot] = 0;
}

void action_check_adjacent(const int inv[]) {
    int id1 = 0, id2 = 0, found = 0;
    printf("ID первого предмета: "); if (scanf_s("%d", &id1) != 1) { while (getchar() != '\n'); return; }
    printf("ID второго предмета: "); if (scanf_s("%d", &id2) != 1) { while (getchar() != '\n'); return; }

    for (int i = 0; i < INVENTORY_SIZE - 1; i++) {
        if ((inv[i] == id1 && inv[i + 1] == id2) || (inv[i] == id2 && inv[i + 1] == id1)) {
            printf("Предметы лежат рядом в слотах: %d и %d\n", i, i + 1); found = 1;
        }
    }
    if (!found) printf("Предметы не лежат в соседних слотах.\n");
}

void action_search_by_name(const int inv[], char names[MAX_ITEMS][MAX_NAME_LEN]) {
    char search_term[MAX_NAME_LEN];
    printf("Введите точное название предмета для поиска: ");
    if (scanf_s("%31s", search_term, (unsigned)MAX_NAME_LEN) != 1) {
        while (getchar() != '\n');
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
        if (inv[i] == target_id) {
            printf("%d ", i);
            count++;
        }
    }
    if (count > 0) printf("\nВсего штук в рюкзаке: %d\n", count);
    else printf("Нет в рюкзаке.\n");
}

void action_write_diary(const char* name, int day, int hour, const int inv[], char names[MAX_ITEMS][MAX_NAME_LEN]) {
    FILE* f;
    if (fopen_s(&f, "diary.txt", "a") != 0 || f == NULL) {
        printf("Ошибка: не удалось открыть diary.txt для записи!\n");
        return;
    }

    fprintf(f, "==============================\n");
    fprintf(f, "Фермер: %s\n", name);
    fprintf(f, "Запись от: День %d, %02d:00\n", day, hour);
    fprintf(f, "Состояние рюкзака:\n");

    int empty_count = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inv[i] == 0) {
            empty_count++;
        }
        else {
            fprintf(f, " - Слот %d: %s\n", i, names[inv[i]]);
        }
    }
    fprintf(f, "Пустых слотов: %d\n", empty_count);
    fprintf(f, "==============================\n\n");

    fclose(f);
    printf("Состояние успешно записано в diary.txt!\n");
}

// --- ПРАВИЛЬНЫЙ ВАРИАНТ 9 (Чтение input.txt, запись в output.txt и на экран) ---
void action_notice_board(int current_day, const int inv[], char names[MAX_ITEMS][MAX_NAME_LEN]) {
    FILE* fin, * fout;

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

    // Выводим заголовок И на экран, И в файл
    printf("\n--- ДОСТУПНЫЕ КВЕСТЫ (День %d) ---\n", current_day);
    fprintf(fout, "--- ДОСТУПНЫЕ КВЕСТЫ (День %d) ---\n", current_day);

    // Читаем строки из input.txt в формате: День | Заказчик | Предмет | Награда
    while (fscanf_s(fin, "%d | %31s | %31s | %d", &q_day, q_cust, (unsigned)MAX_NAME_LEN, q_item, (unsigned)MAX_NAME_LEN, &q_reward) == 4) {

        // По заданию: отфильтровать только те задания, которые уже доступны игроку
        if (q_day <= current_day) {
            active_count++;

            // Проверить по названиям предметов, лежит ли Требуемый_предмет в инвентаре
            int found_in_inventory = 0;
            for (int i = 0; i < INVENTORY_SIZE; i++) {
                int item_id = inv[i];
                if (item_id > 0 && item_id < MAX_ITEMS) {
                    if (strcmp(names[item_id], q_item) == 0) {
                        found_in_inventory = 1;
                        break;
                    }
                }
            }

            // Выводим статус И на экран, И в файл (как требует задание)
            if (found_in_inventory) {
                printf("%s просит: %s (Награда: %d) -> [ГОТОВО К СДАЧЕ]\n", q_cust, q_item, q_reward);
                fprintf(fout, "%s просит: %s (Награда: %d) -> [ГОТОВО К СДАЧЕ]\n", q_cust, q_item, q_reward);
            }
            else {
                printf("%s просит: %s (Награда: %d) -> [В ПРОЦЕССЕ]\n", q_cust, q_item, q_reward);
                fprintf(fout, "%s просит: %s (Награда: %d) -> [В ПРОЦЕССЕ]\n", q_cust, q_item, q_reward);
            }
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
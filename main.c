#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Добавили библиотеку для работы со строками

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

// --- ОСНОВНАЯ ФУНКЦИЯ ---
int main() {
    system("chcp 65001 > nul");

    int current_day = 1;
    int current_hour = 8;
    int inventory[INVENTORY_SIZE] = { 1, 2, 3, 0, 4, 5, 0, 7, 1, 9 };

    char item_names[MAX_ITEMS][MAX_NAME_LEN];
    char player_name[MAX_PLAYER_NAME];

    // Инициализация (Шаг 2)
    load_item_names(item_names);
    ask_player_name(player_name);

    while (1) {
        display_menu(player_name);
        int choice = get_valid_input();

        switch (choice) {
        case 0:
            printf("Выход из игры...\n");
            return 0;
        case 1: action_show_time(current_day, current_hour); break;
        case 2: action_work(&current_day, &current_hour); break;
        case 3: action_show_inventory(inventory, item_names); break;
        case 4: action_put_item(inventory); break;
        case 5: action_drop_item(inventory); break;
        case 6: action_check_adjacent(inventory); break;
        default:
            printf("Неверный пункт меню!\n");
            break;
        }
    }
    return 0;
}

// --- РЕАЛИЗАЦИЯ ФУНКЦИЙ ---

void load_item_names(char names[MAX_ITEMS][MAX_NAME_LEN]) {
    // Базовые названия (защита, если файла нет)
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
    printf("Каталог предметов успешно загружен из items.txt.\n");
}

void ask_player_name(char* name) {
    printf("\nДобро пожаловать! Введите имя фермера (без пробелов): ");
    if (scanf_s("%31s", name, (unsigned)MAX_PLAYER_NAME) != 1) {
        strcpy_s(name, MAX_PLAYER_NAME, "Фермер");
    }
    while (getchar() != '\n'); // Очистка буфера
}

void display_menu(const char* name) {
    printf("\n=== ВЕСЕЛЫЙ ФЕРМЕР: %s ===\n", name);
    printf("[1] Посмотреть на часы\n");
    printf("[2] Промотать время (Поработать)\n");
    printf("[3] Посмотреть инвентарь\n");
    printf("[4] Положить предмет в слот\n");
    printf("[5] Выбросить предмет\n");
    printf("[6] Проверка соседних ячеек (ЛР 2)\n");
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

void action_show_time(int day, int hour) {
    printf("Текущее время: День %d, %02d:00\n", day, hour);
}

void action_work(int* day, int* hour) {
    int hours_to_work = 0;
    printf("Сколько часов поработать? ");
    if (scanf_s("%d", &hours_to_work) != 1 || hours_to_work < 0) {
        printf("Ошибка ввода! Введите положительное число.\n");
        while (getchar() != '\n');
        return;
    }
    *hour += hours_to_work;
    if (*hour >= 24) { *day += *hour / 24; *hour = *hour % 24; }
    printf("Время промотано! Текущее время: День %d, %02d:00\n", *day, *hour);
}

void action_show_inventory(const int inv[], char names[MAX_ITEMS][MAX_NAME_LEN]) {
    printf("--- Инвентарь ---\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int item_id = inv[i];
        if (item_id >= 0 && item_id < MAX_ITEMS) {
            printf("Слот %d: [%d] - %s\n", i, item_id, names[item_id]);
        }
        else {
            printf("Слот %d: [%d] - Неизвестный предмет\n", i, item_id);
        }
    }
}

void action_put_item(int inv[]) {
    int slot = -1, item_id = 0;
    printf("Введите индекс слота (0-9): ");
    if (scanf_s("%d", &slot) != 1 || slot < 0 || slot >= INVENTORY_SIZE) {
        printf("Ошибка: неверный слот!\n");
        while (getchar() != '\n');
        return;
    }
    printf("Введите ID предмета: ");
    if (scanf_s("%d", &item_id) != 1) {
        printf("Ошибка ввода!\n");
        while (getchar() != '\n');
        return;
    }
    inv[slot] = item_id;
    printf("Предмет добавлен в слот %d!\n", slot);
}

void action_drop_item(int inv[]) {
    int slot = -1;
    printf("Введите индекс слота для очистки (0-9): ");
    if (scanf_s("%d", &slot) != 1 || slot < 0 || slot >= INVENTORY_SIZE) {
        printf("Ошибка: такого слота не существует!\n");
        while (getchar() != '\n');
        return;
    }
    inv[slot] = 0;
    printf("Слот %d очищен!\n", slot);
}

void action_check_adjacent(const int inv[]) {
    int id1 = 0, id2 = 0, found = 0;
    printf("Введите ID первого предмета (0-9): ");
    if (scanf_s("%d", &id1) != 1) { printf("Ошибка!\n"); while (getchar() != '\n'); return; }
    printf("Введите ID второго предмета (0-9): ");
    if (scanf_s("%d", &id2) != 1) { printf("Ошибка!\n"); while (getchar() != '\n'); return; }

    for (int i = 0; i < INVENTORY_SIZE - 1; i++) {
        if ((inv[i] == id1 && inv[i + 1] == id2) || (inv[i] == id2 && inv[i + 1] == id1)) {
            printf("Предметы лежат рядом в слотах: %d и %d\n", i, i + 1);
            found = 1;
        }
    }
    if (!found) printf("Предметы не лежат в соседних слотах.\n");
}
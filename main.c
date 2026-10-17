#include <stdio.h>
#include <stdlib.h>

#define INVENTORY_SIZE 10

// --- ПРОТОТИПЫ ФУНКЦИЙ ---
void display_menu();
int get_valid_input();
void action_show_time(int day, int hour);
void action_work(int* day, int* hour);
void action_show_inventory(const int inv[]);
void action_put_item(int inv[]);
void action_drop_item(int inv[]);
void action_check_adjacent(const int inv[]);

// --- ОСНОВНАЯ ФУНКЦИЯ ---
int main() {
    system("chcp 65001 > nul");

    int current_day = 1;
    int current_hour = 8;
    int inventory[INVENTORY_SIZE] = { 1, 2, 3, 0, 4, 5, 0, 7, 1, 9 };

    // Главный цикл стал чистым и коротким!
    while (1) {
        display_menu();
        int choice = get_valid_input();

        switch (choice) {
        case 0:
            printf("Выход из игры...\n");
            return 0;
        case 1: action_show_time(current_day, current_hour); break;
        case 2: action_work(&current_day, &current_hour); break;
        case 3: action_show_inventory(inventory); break;
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

void display_menu() {
    printf("\n=== ВЕСЕЛЫЙ ФЕРМЕР ===\n");
    printf("[1] Посмотреть на часы\n");
    printf("[2] Промотать время (Поработать)\n");
    printf("[3] Посмотреть инвентарь\n");
    printf("[4] Положить предмет в слот\n");
    printf("[5] Выбросить предмет\n");
    printf("[6] Проверка соседних ячеек (Вариант ЛР 2)\n");
    printf("[0] Выход\n");
    printf("Выберите пункт: ");
}

int get_valid_input() {
    int choice = -1;
    if (scanf_s("%d", &choice) != 1) {
        printf("Ошибка! Введите число.\n");
        while (getchar() != '\n'); // Очистка буфера
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
    if (*hour >= 24) {
        *day += *hour / 24;
        *hour = *hour % 24;
    }
    printf("Время промотано! Текущее время: День %d, %02d:00\n", *day, *hour);
}

void action_show_inventory(const int inv[]) {
    printf("--- Инвентарь ---\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        printf("Слот %d: [%d] (", i, inv[i]);
        switch (inv[i]) {
        case 0: printf("Пусто"); break;
        case 1: printf("Дерево"); break;
        case 2: printf("Камень"); break;
        case 3: printf("Семена"); break;
        case 4: printf("Пшеница"); break;
        case 5: printf("Яблоко"); break;
        case 6: printf("Лопата"); break;
        case 7: printf("Вода"); break;
        case 8: printf("Удобрение"); break;
        case 9: printf("Золото"); break;
        default: printf("Неизвестный предмет"); break;
        }
        printf(")\n");
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
#include <stdio.h>
#include <stdlib.h>

#define INVENTORY_SIZE 10

int main() {
    // === НАСТРОЙКА И ИНИЦИАЛИЗАЦИЯ ===
    system("chcp 65001 > nul");

    int current_day = 1;
    int current_hour = 8;
    int inventory[INVENTORY_SIZE] = { 1, 2, 3, 0, 4, 5, 0, 7, 1, 9 };
    int choice = -1;

    // === ГЛАВНЫЙ ИГРОВОЙ ЦИКЛ ===
    while (1) {
        // --- Вывод меню ---
        printf("\n=== ВЕСЕЛЫЙ ФЕРМЕР ===\n");
        printf("[1] Посмотреть на часы\n");
        printf("[2] Промотать время (Поработать)\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет в слот\n");
        printf("[5] Выбросить предмет\n");
        printf("[6] Проверка соседних ячеек\n");
        printf("[0] Выход\n");
        printf("Выберите пункт: ");

        // --- Защита от ввода символов ---
        if (scanf_s("%d", &choice) != 1) {
            printf("Ошибка! Введите число.\n");
            while (getchar() != '\n');
            continue;
        }

        // --- Обработка команд пользователя ---
        switch (choice) {
        case 0:
            printf("Выход из игры...\n");
            return 0;

        case 1:
            printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
            break;

        case 2: {
            int hours_to_work = 0;
            printf("Сколько часов поработать? ");

            if (scanf_s("%d", &hours_to_work) != 1 || hours_to_work < 0) {
                printf("Ошибка ввода! Введите положительное число.\n");
                while (getchar() != '\n');
                break;
            }

            current_hour += hours_to_work;

            if (current_hour >= 24) {
                current_day += current_hour / 24;
                current_hour = current_hour % 24;
            }

            printf("Время промотано! Текущее время: День %d, %02d:00\n", current_day, current_hour);
            break;
        }

        case 3: {
            printf("--- Инвентарь ---\n");
            for (int i = 0; i < INVENTORY_SIZE; i++) {
                printf("Слот %d: [%d] (", i, inventory[i]);

                switch (inventory[i]) {
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
            break;
        }

        case 4: {
            int slot = -1;
            int item_id = 0;

            printf("Введите индекс слота (0-9): ");
            if (scanf_s("%d", &slot) != 1) {
                printf("Ошибка ввода!\n");
                while (getchar() != '\n');
                break;
            }

            if (slot < 0 || slot >= INVENTORY_SIZE) {
                printf("Ошибка: такого слота не существует!\n");
                break;
            }

            printf("Введите ID предмета (0-9): ");
            if (scanf_s("%d", &item_id) != 1) {
                printf("Ошибка ввода!\n");
                while (getchar() != '\n');
                break;
            }

            inventory[slot] = item_id;
            printf("Предмет добавлен в слот %d!\n", slot);
            break;
        }

        case 5: {
            int slot = -1;

            printf("Введите индекс слота для очистки (0-9): ");
            if (scanf_s("%d", &slot) != 1) {
                printf("Ошибка ввода!\n");
                while (getchar() != '\n');
                break;
            }

            if (slot < 0 || slot >= INVENTORY_SIZE) {
                printf("Ошибка: такого слота не существует!\n");
                break;
            }

            inventory[slot] = 0;
            printf("Слот %d очищен!\n", slot);
            break;
        }

        case 6: {
            int id1 = 0, id2 = 0;
            int found = 0;

            printf("Введите ID первого предмета (0-9): ");
            if (scanf_s("%d", &id1) != 1) {
                printf("Ошибка ввода!\n");
                while (getchar() != '\n');
                break;
            }

            printf("Введите ID второго предмета (0-9): ");
            if (scanf_s("%d", &id2) != 1) {
                printf("Ошибка ввода!\n");
                while (getchar() != '\n');
                break;
            }

            // Проходим по массиву до предпоследнего элемента
            for (int i = 0; i < INVENTORY_SIZE - 1; i++) {
                // Проверяем совпадение в любом порядке: (id1, id2) или (id2, id1)
                if ((inventory[i] == id1 && inventory[i + 1] == id2) ||
                    (inventory[i] == id2 && inventory[i + 1] == id1)) {
                    printf("Предметы лежат рядом в слотах: %d и %d\n", i, i + 1);
                    found = 1;
                }
            }

            if (!found) {
                printf("Предметы не лежат в соседних слотах.\n");
            }

            break;
        }

        default:
            printf("Неверный пункт меню!\n");
            break;
        }
    }

    return 0;
}

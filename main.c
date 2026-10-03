#include <stdio.h>
#include <stdlib.h>
#define INVENTORY_SIZE 10
int main() {
    system("chcp 65001 > nul");
    int current_day = 1;
    int current_hour = 8;
    int inventory[INVENTORY_SIZE] = { 1, 2, 3, 0, 0, 1, 2, 0, 0, 0 };
    int choice = -1;

    while (1) {
        printf("\n=== ВЕСЕЛЫЙ ФЕРМЕР ===\n");
        printf("[1] Посмотреть на часы\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет в слот\n");
        printf("[5] Выбросить предмет\n");
        printf("[0] Выход\n");
        printf("Выберите пункт: ");

        scanf_s("%d", &choice);

        switch (choice) {
        case 0:
            printf("Выход из игры...\n");
            return 0;

        case 1:
            printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
            break;

        case 3: {
            printf("--- Инвентарь ---\n");
            for (int i = 0; i < INVENTORY_SIZE; i++) {
                printf("Слот %d: [%d]\n", i, inventory[i]);
            }
            break;
        }

        case 4: {
            int slot = -1;
            int item_id = 0;

            printf("Введите индекс слота (0-9): ");
            scanf_s("%d", &slot);

            if (slot < 0 || slot >= INVENTORY_SIZE) {
                printf("Ошибка: такого слота не существует!\n");
                break;
            }

            printf("Введите ID предмета: ");
            scanf_s("%d", &item_id);

            inventory[slot] = item_id;
            printf("Предмет добавлен в слот %d!\n", slot);
            break;
        }

        case 5: {
            int slot = -1;

            printf("Введите индекс слота для очистки (0-9): ");
            scanf_s("%d", &slot);

            if (slot < 0 || slot >= INVENTORY_SIZE) {
                printf("Ошибка: такого слота не существует!\n");
                break;
            }

            inventory[slot] = 0;
            printf("Слот %d очищен!\n", slot);
            break;
        }

        default:
            printf("Неверный пункт меню!\n");
            break;
        }
    }

    return 0;
}
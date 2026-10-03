#include <stdio.h>
#include <stdlib.h>

int main() {
    system("chcp 65001 > nul");

    int current_day = 1;
    int current_hour = 8;
    int choice = -1;

    while (1) {
        printf("\n=== ВЕСЕЛЫЙ ФЕРМЕР ===\n");
        printf("[1] Посмотреть на часы\n");
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

        default:
            printf("Неверный пункт меню!\n");
            break;
        }
    }

    return 0;
}

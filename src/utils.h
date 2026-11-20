#ifndef UTILS_H
#define UTILS_H

#include "config.h"
#include "player.h"

/* Игровое время */
struct GameTime {
    int day;
    int hour;
};

/* ==================== Ввод пользователя ==================== */
void clear_input_buffer(void);
int read_int(const char* prompt, int min_value, int max_value, int* out_value);
int read_word(const char* prompt, char* out, int size);
int ask_yes_no(const char* question);

/* ==================== Время и режим дня ==================== */
void time_show(const struct GameTime* time);
int time_hours_until_night(const struct GameTime* time);
void time_add_hours(struct GameTime* time, int hours);
int time_wait(struct GameTime* time);
void time_sleep(struct GameTime* time, struct Player* player);
int time_is_night(const struct GameTime* time);

/* ==================== Файлы и предметы ==================== */
void load_item_names(char names[MAX_ITEMS][MAX_NAME_LEN]);
void action_write_diary(const struct Player* player, const struct GameTime* time, char names[MAX_ITEMS][MAX_NAME_LEN]);
void diary_log_expedition(const struct Player* player, const struct GameTime* time,
                          int floor_reached, int resources_gathered, int monsters_defeated,
                          const char* outcome);
void action_notice_board(const struct GameTime* time, const struct Player* player, char names[MAX_ITEMS][MAX_NAME_LEN]);

#endif /* UTILS_H */

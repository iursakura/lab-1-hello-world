#ifndef CONFIG_H
#define CONFIG_H

/* ==================== Размеры и длины строк ==================== */
#define INVENTORY_SIZE   10   /* Количество слотов в рюкзаке */
#define MAX_ITEMS        20   /* Максимальное количество видов предметов (ID 0..19) */
#define MAX_NAME_LEN     32   /* Максимальная длина названия предмета */
#define MAX_PLAYER_NAME  32   /* Максимальная длина имени игрока */

/* ==================== Стартовые значения ==================== */
#define START_DAY          1
#define START_HOUR         8
#define START_GOLD         50
#define DEFAULT_MAX_HP     100
#define DEFAULT_MAX_STAMINA 50

/* ==================== Время ==================== */
#define HOURS_IN_DAY       24
#define SLEEP_HOUR         8   /* Во сколько фермер просыпается */
#define NIGHT_LIMIT_HOUR   2   /* Ночью (02:00) спать, приключения запрещены */

/* ==================== Приключение ==================== */
#define STEP_STAMINA_COST          1   /* Расход выносливости за шаг */
#define FLOOR_STAMINA_COST         5   /* Расход выносливости за переход на этаж */
#define GOLD_PER_FLOOR            10   /* Базовый множитель награды за этаж */
#define SCOUT_FREE_STEP_CHANCE    50   /* Разведчик: шанс бесплатного шага, % */
#define LUMBERJACK_HEAL_PER_STEP   1   /* Лесоруб: восстановление HP за шаг */
#define FARMER_GOLD_PERCENT      120   /* Фермер: +20% золота за этаж */
#define MINER_GOLD_PERCENT       130   /* Шахтёр: +30% золота за этаж */
#define NORMAL_GOLD_PERCENT      100   /* Обычный множитель золота */

/* ==================== Генерация карты ==================== */
#define MAP_MIN_HEIGHT        11   /* Минимальная высота этажа (нечётная) */
#define MAP_MIN_WIDTH         19   /* Минимальная ширина этажа (нечётная) */
#define MAP_SIDE_STEP          2   /* Шаг изменения размера карты */
#define MAP_SIDE_VARIANTS      3   /* Количество вариантов размера */
#define WALL_CHANCE_LEFT      18   /* Шанс стены в левой половине, % */
#define WALL_CHANCE_RIGHT     22   /* Шанс стены в правой половине, % */

/* ==================== Коды символов карты ==================== */
#define CELL_WALL         '#'
#define CELL_FLOOR        '.'
#define CELL_MIRROR       '|'
#define CELL_SWITCH       'S'
#define CELL_DOWN         'D'
#define CELL_UP           'U'
#define CELL_HERO         '@'

#endif /* CONFIG_H */

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

/* ==================== Профессии (Вариант 1) ==================== */
#define FARMER_HP          DEFAULT_MAX_HP
#define FARMER_STAMINA     DEFAULT_MAX_STAMINA
#define LUMBERJACK_HP      120
#define LUMBERJACK_STAMINA  45
#define MINER_HP           130
#define MINER_STAMINA       40
#define SCOUT_HP            90
#define SCOUT_STAMINA       70

/* ==================== Коды предметов (порядок как в items.txt) ==================== */
#define ITEM_NONE         0
#define ITEM_WOOD         1
#define ITEM_STONE        2
#define ITEM_SEEDS        3
#define ITEM_WHEAT        4
#define ITEM_PEAR         5
#define ITEM_SHOVEL       6
#define ITEM_WATER        7
#define ITEM_FERTILIZER   8
#define ITEM_GOLD         9

/* ==================== Общие расчёты ==================== */
#define PERCENT_BASE      100   /* База для процентов */
#define RESCUE_FEE_DIVISOR  2   /* Спасатели забирают 1/2 золота */

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
#define CELL_TREE         'T'
#define CELL_ROCK         'R'
#define CELL_HEAL         '+'
#define CELL_MONSTER      'M'
#define CELL_ENT          'E'

/* ==================== Сущности (ЛР 6) ==================== */
#define ENTITY_PERCENT        15   /* % площади, заполняемой сущностями */
#define TREE_CHANCE           35   /* Шанс дерева среди сущностей, % */
#define ROCK_CHANCE           25   /* Шанс камня, % */
#define HEAL_CHANCE           15   /* Шанс аптечки, % */
#define MONSTER_CHANCE        25   /* Шанс монстра, % */

#define BASE_RESOURCE_HP      10   /* Базовая прочность ресурса */
#define BASE_MONSTER_HP       15   /* Базовое HP монстра */
#define BASE_MONSTER_ATTACK    4   /* Базовая атака монстра */
#define BASE_MONSTER_DEFENSE   1   /* Базовая защита монстра */
#define BASE_MONSTER_GOLD     15   /* Базовое золото за монстра */
#define FLOOR_SCALE_PERCENT  120   /* Множитель характеристик за этаж, % */
#define HEAL_HP_RESTORE       20   /* Аптечка: восстановление HP */
#define HEAL_STAMINA_RESTORE  10   /* Аптечка: восстановление стамины */
#define ACTION_STAMINA_COST    2   /* Расход стамины за взаимодействие */
#define MIN_DAMAGE             1   /* Минимальный урон за удар */
#define MONSTER_DROP_CHANCE   30   /* Шанс трофея с монстра, % */
#define DEFENSE_DIVISOR        2   /* Делитель защиты в формуле урона */

/* ==================== Гнев Природы (Вариант 5, ЛР 6) ==================== */
#define NATURE_WRATH_THRESHOLD  50   /* >50% ресурсов — Энты */
#define NATURE_PEACE_THRESHOLD  10   /* <10% ресурсов — x2 аптечек */
#define ENT_HP_MULTIPLIER        3   /* Энт: HP = монстр × 3 */
#define ENT_ATTACK_MULTIPLIER    2   /* Энт: атака = монстр × 2 */
#define ENT_DEFENSE_MULTIPLIER   3   /* Энт: защита = монстр × 3 */
#define ENT_GOLD_MULTIPLIER      2   /* Энт: золото = монстр × 2 */
#define ENT_SPAWN_COUNT          3   /* Сколько Энтов спавнится */
#define HEAL_BONUS_MULTIPLIER    2   /* Множитель аптечек за бережливость */

/* ==================== Игрок: боевые параметры (ЛР 6) ==================== */
#define DEFAULT_ATTACK         5
#define DEFAULT_DEFENSE        2
#define FARMER_ATTACK          DEFAULT_ATTACK
#define FARMER_DEFENSE         DEFAULT_DEFENSE
#define LUMBERJACK_ATTACK      7
#define LUMBERJACK_DEFENSE     DEFAULT_DEFENSE
#define MINER_ATTACK           6
#define MINER_DEFENSE          4
#define SCOUT_ATTACK           4
#define SCOUT_DEFENSE          1

#endif /* CONFIG_H */

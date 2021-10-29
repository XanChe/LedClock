#ifndef LibEnums_h
#define LibEnums_h
enum clockStates {
    CUR_TIME,
    CUR_T_OUTDOOR,
    CUR_T_INDOOR,
    MENU_MINUTES,
    MENU_HOUR,
};

enum showingLedEffects {
    DAYLY,
    NIGHTLY,
    BLINK,
    OFF,
    SUB_ZERO,
    PLUS_ZERO,
};

enum icons {
    MAX_T,
    MIN_T,
    OUTDOOR_T,
    INDOOR_T,
    SWITCH_BUTTON,
    STAT_BUTTON,
    ALL,
    FADE_ALL,
};


#endif
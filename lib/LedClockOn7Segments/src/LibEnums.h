#ifndef LibEnums_h
#define LibEnums_h
enum clockStates {
    CUR_TIME,
    CUR_T_OUTDOOR,
    CUR_T_INDOOR,
    MENU,
    /*MENU_MINUTES,
    MENU_HOUR,
    MENU_COLOR,
    MENU_SUB_COLOR,
    MENU_PLUS_COLOR,
    MENU_BRIGHTNESS,
    MENU_SAVE,*/
};

enum menuStates {    
    MENU_MINUTES,
    MENU_HOUR,
    MENU_COLOR,
    MENU_SUB_COLOR,
    MENU_PLUS_COLOR,
    MENU_BRIGHTNESS,
    MENU_SAVE,
};

enum showingLedEffects {
    DAYLY,
    NIGHTLY,
    BLINK,
    OFF,
    SUB_ZERO,
    PLUS_ZERO,
    CUSTOM_COLOR,
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
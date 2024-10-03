#include "user_interface.h"

typedef struct{
    MENU_ID id;
    char lines[8][15];
} Menu;


const Menu start_menu = (Menu) {
    .id = START_MENU,
    .lines[0] = "Start Game",
    .lines[1] = "Element 1",
    .lines[2] = "Element 2",
    .lines[3] = "Element 3",
    .lines[4] = "Stop Game",
    .lines[5] = "Calibrate",
    .lines[6] = "Random",
    .lines[7] = "Hello World! :)"
};

const Menu template = (Menu) {
    .id = TEMPLATE,
    .lines[0] = "",
    .lines[1] = "",
    .lines[2] = "",
    .lines[3] = "",
    .lines[4] = "",
    .lines[5] = "",
    .lines[6] = "",
    .lines[7] = ""
};
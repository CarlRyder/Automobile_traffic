#pragma once
// SIZES
#define DEFAULT_ERROR -1
#define WINDOW_NAME "Car Traffic Simulator"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define BUTTON_WIDTH 350
#define BUTTON_HEIGHT 50
#define MINI_BUTTON_WIDTH 70
#define MINI_BUTTON_HEIGHT 20
#define CAR_HEIGHT 44
#define CAR_WIDTH 24
#define MIN_DISTANCE 65.0
#define LIGHT_SIZE 15

// TEXTURES
#define MAP_ONE "map_texture/motorway_1.txt"
#define MAP_TWO "map_texture/crossroad_2.txt"
#define MAP_THREE "map_texture/mult_crossroad_3.txt"
#define TOTAL_CARS 20

// COLORS
#define COLOR_MENU_RED 0.102
#define COLOR_MENU_GREEN 0.082
#define COLOR_MENU_BLUE 0.247
#define COLOR_BUTTON_RED 138
#define COLOR_BUTTON_GREEN 43
#define COLOR_BUTTON_BLUE 226
#define COLOR_WHITE 255

// KEYS
#define VK_A 0x41
#define VK_Q 0x51
#define VK_S 0x53

// COORDS
#define MID_COORD 400
#define MENU_BUTTON_LEFT 225
#define MENU_BUTTON_RIGHT 575
#define MENU_BUTTON_BOTTOM 375
#define MENU_BUTTON_TOP 425

// TIMERS
#define TRAFFIC_LIGHT_INTERVAL 15
#define AUTOSAVE_INTERVAL 30
#define STOP_CAR_INTERVAL 30
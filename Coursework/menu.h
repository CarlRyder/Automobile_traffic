#pragma once
#include "pincludes.h"

struct menu
{
    bool menu_active;
    bool model_active;
    bool pause_active;
    bool load_error;
} active;

struct button
{
    bool menu_button_1;
    bool menu_button_2;
    bool menu_button_3;
    bool map_button;
} buttons;

struct maps
{
    bool map_1;
    bool map_2;
    bool map_3;
} enable;

struct stats
{
    unsigned short line;
    unsigned short car_counts;
    unsigned int time;
    unsigned int stop_time;
    float interval;
    bool autosave;
    bool add_car;
    bool traffic_light1;
    bool traffic_light2;
} model;

void menu();
void menu_line();
void drawstring(float x, float y, char* string);
void processing_buttons(int button);
void settings_buttons();
void menu_buttons(int h);
void text_buttons(int flag);
void draw_next_buttons(int a, int b);
void draw_welcome_text();
void draw_mini_buttons(int w, int h, int flag);
void map_choose();
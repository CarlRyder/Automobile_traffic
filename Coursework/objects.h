#pragma once
#include "pincludes.h"
#include "menu.h"

typedef struct car_stats
{
    float x[2];
    float y[2];
    float speed;
    bool turn;
    bool stop;
    unsigned short direction;
    unsigned short line;
    unsigned short texture_id;
} car_model;

typedef struct node
{
    car_model car;
    struct node* next_car;
} Cars;

void points(int x1, int x2, int y1, int y2);
void control_lines(int line);
void map_show(int value);
void pause_background();
void car_static_show(Cars* head);
void pause(int map, Cars* head, bool flag);
void car_position(car_model* car, int first, int second, bool vertically);
void car_draw(car_model* car);
void light(int x, int y);
void light_draw(int x, int y1, int y2, int y3, bool flag);
void traffic_lights();
void mult_traffic_lights();
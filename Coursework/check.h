#pragma once
#include "pincludes.h"
#include "moves.h"

int count_car(Cars* head);
void info(int max, Cars* head);
void change_line(int map, Cars* head);
void start_car(Cars* head);
void stop_car(Cars* head);
void check_light(Cars* head);
void check_cars(Cars* head);
void mult_check_light(Cars* head);
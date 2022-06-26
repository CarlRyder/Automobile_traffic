#pragma once
#include "pincludes.h"
#include "objects.h"

void car_coords(car_model* car, int x, int y);
void init_car(car_model* car);
Cars* remove_all(Cars* head);
Cars* create_car();
Cars* delete_headcar(Cars* head);
Cars* delete_car(Cars* current, Cars* head);
void push_car(Cars* head);
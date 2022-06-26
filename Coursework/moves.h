#pragma once
#include "pincludes.h"
#include "cars.h"

void fill_array_cars(Cars* tmp, unsigned short* range_dir, Cars** car_dir);
int overtaking(int count_line, Cars** car_dir, int num_ccar, unsigned short* range_dir, int num_dir, char mode_coord);
void find_dist(char mode_coord, float coord_diff, Cars** car_dir, int num_dcar, int num_ccar, int count_line, unsigned short* range_dir1, int num_dir);
void check_distance(Cars* head);
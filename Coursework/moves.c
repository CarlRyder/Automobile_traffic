#include "moves.h"
#include "defines.h"

void fill_array_cars(Cars* tmp, unsigned short* range_dir, Cars** car_dir)
{
    if (tmp->car.line == 1)
    {
        car_dir[range_dir[0]] = tmp;
        range_dir[0]++;
    }
    else if (tmp->car.line == 2)
    {
        car_dir[range_dir[1]] = tmp;
        range_dir[1]++;
    }
    else if (tmp->car.line == 3)
    {
        car_dir[range_dir[2]] = tmp;
        range_dir[2]++;
    }
}

int overtaking(int count_line, Cars** car_dir, int num_ccar, unsigned short* range_dir, int num_dir, char mode_coord)
{
    int start_right, end_right, start_left, end_left;
    bool possible_changing_r = false, possible_changing_l = false;
    bool can_changing_r = true, can_changing_l = true;
    int count = 0, max_count = 0;
    if (count_line == 1)
    {
        start_right = model.car_counts;
        end_right = range_dir[1];
        possible_changing_r = true;
        count = 0; max_count = 1;
    }
    else if (count_line == 2)
    {
        if ((num_dir == 3 || num_dir == 4) && enable.map_3 == true)
        {
            start_left = 0;
            end_left = range_dir[0];
            possible_changing_l = true;
            max_count = 2;
            count = 1;
        }
        else
        {
            start_left = 0;
            end_left = range_dir[0];
            start_right = model.car_counts * 2;
            end_right = range_dir[2];
            possible_changing_r = true;
            possible_changing_l = true;
            max_count = 2;
            count = rand() % 1 + 0;
        }
    }
    else if (count_line == 3)
    {
        start_left = model.car_counts;
        end_left = range_dir[1];
        possible_changing_l = true;
        count = 1; max_count = 2;
    }
    bool possible_changing_tmp = false, can_changing_tmp = false;
    int end_tmp = 0, start_tmp = 0;
    while (count < max_count)
    {
        if (count == 0 && possible_changing_r == true) //0 r
        {
            possible_changing_tmp = possible_changing_r;
            end_tmp = end_right;
            start_tmp = start_right;
            can_changing_tmp = can_changing_r;
        }
        else if (count == 1 && possible_changing_l == true) // 1 l
        {
            possible_changing_tmp = possible_changing_l;
            end_tmp = end_left;
            start_tmp = start_left;
            can_changing_tmp = can_changing_l;
        }
        if (possible_changing_tmp == true)
        {
            if (end_tmp - start_tmp == 0)
            {
                if ((num_dir == 3 || num_dir == 4) && enable.map_3 == true)
                {
                    if (num_dir == 3 && count == 1) { car_dir[num_ccar]->car.y[0] = 215; car_dir[num_ccar]->car.line--; }
                    else if (num_dir == 3 && count == 0) { car_dir[num_ccar]->car.y[0] = 255; car_dir[num_ccar]->car.line++; }
                    else if (num_dir == 4 && count == 1) { car_dir[num_ccar]->car.y[0] = 135; car_dir[num_ccar]->car.line--; }
                    else if (num_dir == 4 && count == 0) { car_dir[num_ccar]->car.y[0] = 175; car_dir[num_ccar]->car.line++; }
                }
                else
                {
                    if (count_line == 2 && count == 0) //r
                    {
                        if (num_dir == 1)  car_dir[num_ccar]->car.x[0] = 510;
                        else if (num_dir == 2) car_dir[num_ccar]->car.x[0] = 373;
                        else if (num_dir == 3 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 525;
                        else if (num_dir == 4 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 386;
                        else if (num_dir == 5) car_dir[num_ccar]->car.y[0] = 685;
                        else if (num_dir == 6) car_dir[num_ccar]->car.y[0] = 565;
                        car_dir[num_ccar]->car.line += 1;
                    }
                    else if (count_line == 2 && count == 1)//l
                    {
                        if (num_dir == 1) car_dir[num_ccar]->car.x[0] = 420;
                        else if (num_dir == 2)car_dir[num_ccar]->car.x[0] = 283;
                        else if (num_dir == 3 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 435;
                        else if (num_dir == 4 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 296;
                        else if (num_dir == 5) car_dir[num_ccar]->car.y[0] = 605;
                        else if (num_dir == 6) car_dir[num_ccar]->car.y[0] = 485;
                        car_dir[num_ccar]->car.line -= 1;
                    }
                    else if (count == 0 || count == 1) //r
                    {
                        if (num_dir == 1) car_dir[num_ccar]->car.x[0] = 465;
                        else if (num_dir == 2)car_dir[num_ccar]->car.x[0] = 328;
                        else if (num_dir == 3 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 475;
                        else if (num_dir == 4 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 341;
                        else if (num_dir == 5) car_dir[num_ccar]->car.y[0] = 640;
                        else if (num_dir == 6) car_dir[num_ccar]->car.y[0] = 525;

                        if (count == 1)  car_dir[num_ccar]->car.line--;
                        else if (count == 0) car_dir[num_ccar]->car.line++;
                    }
                }
                return 1;
            }
            else
            {
                for (int num_dcar = start_tmp; num_dcar < end_tmp; num_dcar++)
                {
                    if ((num_dir == 3 || num_dir == 4) && enable.map_3 == true)
                    {
                        if (car_dir[num_dcar]->car.y[0] - car_dir[num_ccar]->car.y[0] > 0 &&
                            car_dir[num_dcar]->car.y[0] - car_dir[num_ccar]->car.y[0] < 100.0)
                        {
                            can_changing_tmp = false;
                        }
                        if (car_dir[num_dcar]->car.y[0] - car_dir[num_ccar]->car.y[0] < 0 &&
                            abs(car_dir[num_dcar]->car.y[0] - car_dir[num_ccar]->car.y[0]) < 100.0)
                        {
                            can_changing_tmp = false;
                        }
                    }
                    else
                    {
                        if (num_dir == 1 || num_dir == 2)
                        {
                            if (car_dir[num_dcar]->car.y[0] - car_dir[num_ccar]->car.y[0] > 0 &&
                                car_dir[num_dcar]->car.y[0] - car_dir[num_ccar]->car.y[0] < 100.0)
                            {
                                can_changing_tmp = false;
                            }
                            if (car_dir[num_dcar]->car.y[0] - car_dir[num_ccar]->car.y[0] < 0 &&
                                abs(car_dir[num_dcar]->car.y[0] - car_dir[num_ccar]->car.y[0]) < 100.0)
                            {
                                can_changing_tmp = false;
                            }
                        }
                        else if (num_dir == 5 || num_dir == 6 || num_dir == 3 || num_dir == 4)
                        {
                            if (car_dir[num_dcar]->car.x[0] - car_dir[num_ccar]->car.x[0] > 0 &&
                                car_dir[num_dcar]->car.x[0] - car_dir[num_ccar]->car.x[0] < 100.0)
                            {
                                can_changing_tmp = false;
                            }
                            if (car_dir[num_dcar]->car.x[0] - car_dir[num_ccar]->car.x[0] < 0 &&
                                abs(car_dir[num_dcar]->car.x[0] - car_dir[num_ccar]->car.x[0]) < 100.0)
                            {
                                can_changing_tmp = false;
                            }
                        }
                    }
                }
                if (can_changing_tmp == true)
                {
                    if ((num_dir == 3 || num_dir == 4) && enable.map_3 == true)
                    {
                        if (num_dir == 3 && count == 1) { car_dir[num_ccar]->car.y[0] = 215; car_dir[num_ccar]->car.line--; }
                        else if (num_dir == 3 && count == 0) { car_dir[num_ccar]->car.y[0] = 255; car_dir[num_ccar]->car.line++; }
                        else if (num_dir == 4 && count == 1) { car_dir[num_ccar]->car.y[0] = 135; car_dir[num_ccar]->car.line--; }
                        else if (num_dir == 4 && count == 0) { car_dir[num_ccar]->car.y[0] = 175; car_dir[num_ccar]->car.line++; }
                    }
                    else
                    {
                        if (count_line == 2 && count == 0) //r
                        {
                            if (num_dir == 1)  car_dir[num_ccar]->car.x[0] = 510;
                            else if (num_dir == 2) car_dir[num_ccar]->car.x[0] = 373;
                            else if (num_dir == 3 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 525;
                            else if (num_dir == 4 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 386;
                            else if (num_dir == 5) car_dir[num_ccar]->car.y[0] = 685;
                            else if (num_dir == 6) car_dir[num_ccar]->car.y[0] = 565;
                            car_dir[num_ccar]->car.line++;
                        }
                        else if (count_line == 2 && count == 1)//l
                        {
                            if (num_dir == 1) car_dir[num_ccar]->car.x[0] = 420;
                            else if (num_dir == 2)car_dir[num_ccar]->car.x[0] = 283;
                            else if (num_dir == 3 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 435;
                            else if (num_dir == 4 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 296;
                            else if (num_dir == 5) car_dir[num_ccar]->car.y[0] = 605;
                            else if (num_dir == 6) car_dir[num_ccar]->car.y[0] = 485;
                            car_dir[num_ccar]->car.line--;
                        }
                        else if (count == 0 || count == 1) //r
                        {
                            if (num_dir == 1) car_dir[num_ccar]->car.x[0] = 465;
                            else if (num_dir == 2) car_dir[num_ccar]->car.x[0] = 328;
                            else if (num_dir == 3 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 475;
                            else if (num_dir == 4 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 341;
                            else if (num_dir == 5) car_dir[num_ccar]->car.y[0] = 640;
                            else if (num_dir == 6) car_dir[num_ccar]->car.y[0] = 525;
                            if (count == 1) car_dir[num_ccar]->car.line--;
                            else if (count == 0) car_dir[num_ccar]->car.line++;
                        }
                    }
                    return 1;
                }
            }
        }
        count++;
    }
    return 0;
}

void find_dist(char mode_coord, float coord_diff, Cars** car_dir, int num_dcar, int num_ccar, int count_line, unsigned short* range_dir1, int num_dir)
{
    float dcar = 0.0, ccar = 0.0;
    if (mode_coord == 'y')
    {
        dcar = car_dir[num_dcar]->car.y[0];
        ccar = car_dir[num_ccar]->car.y[0];
    }
    else if (mode_coord == 'x')
    {
        dcar = car_dir[num_dcar]->car.x[0];
        ccar = car_dir[num_ccar]->car.x[0];
    }
    float diff = dcar - ccar;
    if ((coord_diff == 44.0 && diff > 0 && diff <= MIN_DISTANCE) || (coord_diff == -44.0 && diff < 0 && fabsf(diff) <= MIN_DISTANCE))
    {
        if (overtaking(count_line, car_dir, num_ccar, range_dir1, num_dir, mode_coord) == 0)
        {
            car_dir[num_ccar]->car.speed = car_dir[num_dcar]->car.speed;
        }
    }
}

void check_distance(Cars* head)
{
    Cars* tmp = head;

    Cars** car_dir_1 = (Cars**)malloc(model.car_counts * 3 * sizeof(Cars*));
    Cars** car_dir_2 = (Cars**)malloc(model.car_counts * 3 * sizeof(Cars*));
    Cars** car_dir_3 = (Cars**)malloc(model.car_counts * 3 * sizeof(Cars*));
    Cars** car_dir_4 = (Cars**)malloc(model.car_counts * 3 * sizeof(Cars*));
    Cars** car_dir_5 = (Cars**)malloc(model.car_counts * 3 * sizeof(Cars*));
    Cars** car_dir_6 = (Cars**)malloc(model.car_counts * 3 * sizeof(Cars*));

    unsigned short range_dir1[3] = { 0, model.car_counts, model.car_counts * 2 };
    unsigned short range_dir2[3] = { 0, model.car_counts, model.car_counts * 2 };
    unsigned short range_dir3[3] = { 0, model.car_counts, model.car_counts * 2 };
    unsigned short range_dir4[3] = { 0, model.car_counts, model.car_counts * 2 };
    unsigned short range_dir5[3] = { 0, model.car_counts, model.car_counts * 2 };
    unsigned short range_dir6[3] = { 0, model.car_counts, model.car_counts * 2 };

    while (tmp != NULL)
    {
        if (tmp->car.direction == 1) fill_array_cars(tmp, range_dir1, car_dir_1);
        else if (tmp->car.direction == 2) fill_array_cars(tmp, range_dir2, car_dir_2);
        else if (tmp->car.direction == 3) fill_array_cars(tmp, range_dir3, car_dir_3);
        else if (tmp->car.direction == 4) fill_array_cars(tmp, range_dir4, car_dir_4);
        else if (tmp->car.direction == 5) fill_array_cars(tmp, range_dir5, car_dir_5);
        else if (tmp->car.direction == 6) fill_array_cars(tmp, range_dir6, car_dir_6);
        tmp = tmp->next_car;
    }

    int num_ccar = 0, num_dcar = 0, count_line = 1, end_arr = 0, start_num_arr = 0, count_dir = 1;
    while (count_dir < 7)
    {
        count_line = 1;
        while (count_line < 4)
        {
            if (count_line == 1)
            {
                start_num_arr = 0;
                if (count_dir == 1) end_arr = range_dir1[0];
                else if (count_dir == 2) end_arr = range_dir2[0];
                else if (count_dir == 3) end_arr = range_dir3[0];
                else if (count_dir == 4) end_arr = range_dir4[0];
                else if (count_dir == 5) end_arr = range_dir5[0];
                else if (count_dir == 6) end_arr = range_dir6[0];
            }
            else if (count_line == 2)
            {
                start_num_arr = model.car_counts;
                if (count_dir == 1) end_arr = range_dir1[1];
                else if (count_dir == 2) end_arr = range_dir2[1];
                else if (count_dir == 3) end_arr = range_dir3[1];
                else if (count_dir == 4) end_arr = range_dir4[1];
                else if (count_dir == 5) end_arr = range_dir5[1];
                else if (count_dir == 6) end_arr = range_dir6[1];
            }
            else if (count_line == 3)
            {
                start_num_arr = model.car_counts * 2;
                if (count_dir == 1) end_arr = range_dir1[2];
                else if (count_dir == 2) end_arr = range_dir2[2];
                else if (count_dir == 3) end_arr = range_dir3[2];
                else if (count_dir == 4) end_arr = range_dir4[2];
                else if (count_dir == 5) end_arr = range_dir5[2];
                else if (count_dir == 6) end_arr = range_dir6[2];
            }

            for (num_ccar = start_num_arr; num_ccar < end_arr; num_ccar++)
            {
                for (num_dcar = start_num_arr; num_dcar < end_arr; num_dcar++)
                {
                    if (count_dir == 1 && car_dir_1[num_dcar] != car_dir_1[num_ccar])
                    {
                        find_dist('y', 44.0, car_dir_1, num_dcar, num_ccar, count_line, range_dir1, count_dir);
                    }
                    else if (count_dir == 2 && car_dir_2[num_dcar] != car_dir_2[num_ccar])
                    {
                        find_dist('y', -44.0, car_dir_2, num_dcar, num_ccar, count_line, range_dir2, count_dir);
                    }
                    else if (count_dir == 3 && car_dir_3[num_dcar] != car_dir_3[num_ccar])
                    {
                        find_dist('x', -44.0, car_dir_3, num_dcar, num_ccar, count_line, range_dir3, count_dir);
                    }
                    else if (count_dir == 4 && car_dir_4[num_dcar] != car_dir_4[num_ccar])
                    {
                        find_dist('x', 44.0, car_dir_4, num_dcar, num_ccar, count_line, range_dir4, count_dir);
                    }
                    else if (count_dir == 5 && car_dir_5[num_dcar] != car_dir_5[num_ccar])
                    {
                        find_dist('x', -44.0, car_dir_5, num_dcar, num_ccar, count_line, range_dir5, count_dir);
                    }
                    else if (count_dir == 6 && car_dir_6[num_dcar] != car_dir_6[num_ccar])
                    {
                        find_dist('x', 44.0, car_dir_6, num_dcar, num_ccar, count_line, range_dir6, count_dir);
                    }
                }
            }
            _asm
            {
                inc count_line
            }
        }
        _asm
        {
            inc count_dir
        }
    }
    free(car_dir_1);
    free(car_dir_2);
    free(car_dir_3);
    free(car_dir_4);
    free(car_dir_5);
    free(car_dir_6);
    tmp = NULL;
}
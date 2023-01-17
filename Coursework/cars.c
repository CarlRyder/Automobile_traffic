#include "cars.h"
#include "defines.h"

void car_coords(car_model* car, int x, int y)
{
    car->x[0] = x;
    car->x[1] = x + CAR_WIDTH;
    car->y[0] = y;
    car->y[1] = y + CAR_HEIGHT;
    if (car->direction == 3 || car->direction == 4 || car->direction == 5 || car->direction == 6)
    {
        car->x[1] = x - CAR_HEIGHT;
        car->y[1] = y - CAR_WIDTH;
    }
}

void init_car(car_model* car)
{
    model.car_counts++;
    car->turn = false;
    car->stop = false;
    if (enable.map_1) car->direction = rand() % 2 + 1;
    else if (enable.map_2) car->direction = rand() % 4 + 1;
    else if (enable.map_3) car->direction = rand() % 6 + 1;
    car->line = rand() % 3 + 1;
    if (model.add_car == true)
    {
        if (enable.map_1 == true)
        {
            if (model.line < 4) car->direction = 2;
            else car->direction = 1;
            if (car->direction == 2) car->line = model.line;
            else car->line = model.line - 3;
        }
        else if (enable.map_2 == true)
        {
            if (model.line < 4) car->direction = 2;
            else if (model.line > 3 && model.line < 7) car->direction = 1;
            else if (model.line > 6 && model.line < 10) car->direction = 4;
            else if (model.line > 9 && model.line < 13) car->direction = 3;

            if (car->direction == 2) car->line = model.line;
            else if (car->direction == 1) car->line = model.line - 3;
            else if (car->direction == 4) car->line = model.line - 6;
            else if (car->direction == 3) car->line = model.line - 9;
        }
        else if (enable.map_3 == true)
        {
            if (model.line < 4) car->direction = 2;
            else if (model.line > 3 && model.line < 7) car->direction = 1;
            else if (model.line > 6 && model.line < 9) car->direction = 4;
            else if (model.line > 8 && model.line < 11) car->direction = 3;
            else if (model.line > 10 && model.line < 14) car->direction = 6;
            else if (model.line > 13 && model.line < 17) car->direction = 5;

            if (car->direction == 2) car->line = model.line;
            else if (car->direction == 1) car->line = model.line - 3;
            else if (car->direction == 4) car->line = model.line - 6;
            else if (car->direction == 3) car->line = model.line - 8;
            else if (car->direction == 6) car->line = model.line - 10;
            else if (car->direction == 5) car->line = model.line - 13;
        }
    }
    car->speed = rand() % 90;
    car->speed += 150;
    if (car->direction == 1)
    {
        if (car->line == 1) car_coords(car, 420, -44);
        else if (car->line == 2) car_coords(car, 465, -44);
        else if (car->line == 3) car_coords(car, 510, -44);
    }
    else if (car->direction == 2)
    {
        if (car->line == 1) car_coords(car, 283, 844);
        else if (car->line == 2) car_coords(car, 328, 844);
        else if (car->line == 3) car_coords(car, 373, 844);
    }
    else if (car->direction == 3)
    {
        if (enable.map_2 == true)
        {
            if (car->line == 1) car_coords(car, 844, 435);
            else if (car->line == 2) car_coords(car, 844, 475);
            else if (car->line == 3) car_coords(car, 844, 525);
        }
        else if (enable.map_3 == true)
        {
            car->line = rand() % 2 + 1;
            if (car->line == 1) car_coords(car, 844, 215);
            else if (car->line == 2) car_coords(car, 844, 255);
        }
    }
    else if (car->direction == 4)
    {
        if (enable.map_2 == true)
        {
            if (car->line == 1) car_coords(car, -44, 296);
            else if (car->line == 2) car_coords(car, -44, 341);
            else if (car->line == 3) car_coords(car, -44, 386);
        }
        else if (enable.map_3 == true)
        {
            car->line = rand() % 2 + 1;
            if (car->line == 1) car_coords(car, -44, 135);
            else if (car->line == 2) car_coords(car, -44, 175);
        }
    }
    else if (car->direction == 5)
    {
        if (car->line == 1) car_coords(car, 844, 605);
        else if (car->line == 2) car_coords(car, 844, 643);
        else if (car->line == 3) car_coords(car, 844, 685);
    }
    else if (car->direction == 6)
    {
        if (car->line == 1) car_coords(car, -44, 485);
        else if (car->line == 2) car_coords(car, -44, 525);
        else if (car->line == 3) car_coords(car, -44, 565);
    }
    car->texture_id = rand() % 19;
    
}

Cars* remove_all(Cars* head)
{
    while (head != NULL)
    {
        Cars* ptr = head;
        head = head->next_car;
        free(ptr);
    }
    return NULL;
}

Cars* create_car()
{
    Cars* node = (Cars*)malloc(sizeof(Cars));
    if (node != NULL)
    {
        init_car(&node->car);
        node->next_car = NULL;
        return node;
    }
    else
    {
        printf("Error: Out of memory!\n");
        exit(DEFAULT_ERROR);
        return NULL;
    }
}

Cars* delete_headcar(Cars* head)
{
    Cars* temp = head->next_car;
    free(head);
    return temp;
}

Cars* delete_car(Cars* current, Cars* head)
{
    Cars* temp = head, * ptr = NULL;
    if (temp == NULL) return NULL;
    else if (current == head)
    {
        temp = head->next_car;
        free(head);
        return temp;
    }
    else
    {
        while (temp->next_car != current)
        {
            temp = temp->next_car;
        }
        temp = current->next_car;
        free(current);
        return temp;
    }
}

void push_car(Cars* head)
{
    Cars* new_node = create_car();
    Cars* temp = head;
    while (temp->next_car != NULL) temp = temp->next_car;
    temp->next_car = new_node;
}
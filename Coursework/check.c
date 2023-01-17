#define _CRT_SECURE_NO_WARNINGS

#include "check.h"
#include "defines.h"

int count_car(Cars* head)
{
    Cars* temp = head;
    int counter = 0, car_line = 0;
    while (temp != NULL)
    {
        if (temp->car.direction == 2) car_line = temp->car.line;
        else if (temp->car.direction == 1) car_line = temp->car.line + 3;
        if (enable.map_2 == true)
        {
            if (temp->car.direction == 4) car_line = temp->car.line + 6;
            else if (temp->car.direction == 3) car_line = temp->car.line + 9;
        }
        else if (enable.map_3 == true)
        {
            if (temp->car.direction == 4) car_line = temp->car.line + 6;
            else if (temp->car.direction == 3) car_line = temp->car.line + 8;
            else if (temp->car.direction == 6) car_line = temp->car.line + 10;
            else if (temp->car.direction == 5) car_line = temp->car.line + 13;
        }
        if (car_line == model.line) counter++;
        temp = temp->next_car;
    }
    return counter;
}

void info(int max, Cars* head)
{
    //glColor3ub(239, 211, 52);
    //drawstring(5, WINDOW_HEIGHT - 20, "SERVICE INFORMATION");

    glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
    //drawstring(5, WINDOW_HEIGHT - 40, "Current number of cars:");
    char num[5] = { 0 };
    _itoa(model.car_counts, num, 10);
    drawstring(204, WINDOW_HEIGHT - 40, num);

    //drawstring(5, WINDOW_HEIGHT - 60, "in the selected lane:");
    int car_line = count_car(head);
    char number[5] = { 0 };
    _itoa(car_line, number, 10);
    drawstring(174, WINDOW_HEIGHT - 60, number);

    //drawstring(5, WINDOW_HEIGHT - 80, "Bandwidth:");
    char value[5] = { 0 };
    _itoa(max, value, 10);
    drawstring(100, WINDOW_HEIGHT - 80, value);

    //if (enable.map_3 == true) drawstring(5, WINDOW_HEIGHT - 720, "Simulation time (sec):");
    //else drawstring(5, WINDOW_HEIGHT - 100, "Simulation time (sec):");
    char model_time[10] = { 0 };
    _itoa(model.time, model_time, 10);
    if (enable.map_3 == true) drawstring(184, WINDOW_HEIGHT - 720, model_time);
    else drawstring(184, WINDOW_HEIGHT - 100, model_time);

    /*if (enable.map_3 == true) drawstring(5, WINDOW_HEIGHT - 740, "Interval time (sec):");
    else drawstring(5, WINDOW_HEIGHT - 120, "Interval time (sec):");*/
    if (enable.map_3 == true)
    {
        if (model.interval == 0.75) drawstring(160, WINDOW_HEIGHT - 740, "0.75");
        else if (model.interval == 1) drawstring(160, WINDOW_HEIGHT - 740, "1");
        else if (model.interval == 1.25) drawstring(160, WINDOW_HEIGHT - 740, "1.25");
    }
    else
    {
        if (model.interval == 0.75) drawstring(160, WINDOW_HEIGHT - 120, "0.75");
        else if (model.interval == 1) drawstring(160, WINDOW_HEIGHT - 120, "1");
        else if (model.interval == 1.25) drawstring(160, WINDOW_HEIGHT - 120, "1.25");
    }

    /*if (enable.map_3 == true) drawstring(5, WINDOW_HEIGHT - 760, "Autosave:");
    else drawstring(5, WINDOW_HEIGHT - 140, "Autosave:");*/
    if (enable.map_3 == true)
    {
        if (model.autosave == true) drawstring(90, WINDOW_HEIGHT - 760, "On");
        else drawstring(90, WINDOW_HEIGHT - 760, "Off");
    }
    else
    {
        if (model.autosave == true) drawstring(90, WINDOW_HEIGHT - 140, "On");
        else drawstring(90, WINDOW_HEIGHT - 140, "Off");
    }
}

void change_line(int map, Cars* head)
{
    while (1)
    {
        map_show(map);
        if (head != NULL) car_static_show(head);
        control_lines(model.line);
        glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
        drawstring(MID_COORD - 200, WINDOW_HEIGHT - 40, "In order to provoke an accident, press the F5 key");
        drawstring(MID_COORD - 300, WINDOW_HEIGHT - 760, "To change the selected lane for an accident, use the arrows on the keyboard (<- ->)");
        glutSwapBuffers();
        if (GetAsyncKeyState(VK_LEFT) & 0x1 && model.line > 1) model.line--;
        if (enable.map_1 == true)
        {
            if (GetAsyncKeyState(VK_RIGHT) & 0x1 && model.line < 6) model.line++;
        }
        else if (enable.map_2 == true)
        {
            if (GetAsyncKeyState(VK_RIGHT) & 0x1 && model.line < 12) model.line++;
        }
        else if (enable.map_3 == true)
        {
            if (GetAsyncKeyState(VK_RIGHT) & 0x1 && model.line < 16) model.line++;
        }
        if (GetAsyncKeyState(VK_SPACE) & 0x1) break;
    }
}

void start_car(Cars* head)
{
    Cars* temp = head;
    while (temp != NULL)
    {
        if (temp->car.stop == true && temp->car.speed == 0)
        {
            temp->car.speed = rand() % 30;
            temp->car.speed += 60;
            temp->car.stop = false;
            break;
        }
        temp = temp->next_car;
    }
}

void stop_car(Cars* head)
{
    Cars* temp = head;
    int car_line = 0;
    while (temp != NULL)
    {
        if (temp->car.direction == 2) car_line = temp->car.line;
        else if (temp->car.direction == 1) car_line = temp->car.line + 3;
        if (enable.map_2 == true)
        {
            if (temp->car.direction == 4) car_line = temp->car.line + 6;
            else if (temp->car.direction == 3) car_line = temp->car.line + 9;
        }
        else if (enable.map_3 == true)
        {
            if (temp->car.direction == 4) car_line = temp->car.line + 6;
            else if (temp->car.direction == 3) car_line = temp->car.line + 8;
            else if (temp->car.direction == 6) car_line = temp->car.line + 10;
            else if (temp->car.direction == 5) car_line = temp->car.line + 13;
        }
        if (car_line == model.line)
        {
            temp->car.stop = true;
            temp->car.speed = 0;
            break;
        }
        temp = temp->next_car;
    }
}

void check_light(Cars* head)
{
    Cars* temp = head;
    if (model.traffic_light1 == false && model.traffic_light2 == true)
    {
        while (temp != NULL)
        {
            if (temp->car.direction == 3 || temp->car.direction == 4)
            {
                if (temp->car.stop == false)
                {
                    if (temp->car.speed == 0)
                    {
                        temp->car.speed = rand() % 30;
                        temp->car.speed += 60;
                    }
                    temp->car.speed += 0.005;
                }
            }
            else if (temp->car.direction == 1)
            {
                if (temp->car.y[0] < 230)
                {
                    if (230 - (temp->car.y[0] + CAR_HEIGHT) < 20 && 230 - (temp->car.y[0] + CAR_HEIGHT) > 0)
                    {
                        temp->car.speed = 0;
                    }
                    else
                    {
                        if (temp->car.speed > 5) temp->car.speed -= 0.0015;
                    }
                }
                else if (temp->car.stop == false) temp->car.speed += 0.025;
            }
            else if (temp->car.direction == 2)
            {
                if (temp->car.y[0] > 575)
                {
                    if (fabsf(575 - temp->car.y[0]) < 20 && fabsf(575 - temp->car.y[0]) > 0)
                    {
                        temp->car.speed = 0;
                    }
                    else
                    {
                        if (temp->car.speed > 5) temp->car.speed -= 0.0015;
                    }
                }
                else if (temp->car.stop == false) temp->car.speed += 0.025;
            }
            temp = temp->next_car;
        }
    }
    else if (model.traffic_light1 == true && model.traffic_light2 == false)
    {
        while (temp != NULL)
        {
            if (temp->car.direction == 1 || temp->car.direction == 2)
            {
                if (temp->car.stop == false)
                {
                    if (temp->car.speed == 0)
                    {
                        temp->car.speed = rand() % 30;
                        temp->car.speed += 60;
                    }
                    temp->car.speed += 0.005;
                }
            }
            else if (temp->car.direction == 3)
            {
                if (temp->car.x[0] > 580)
                {
                    if (fabsf(580 - (temp->car.x[0] - CAR_HEIGHT)) < 20 && fabsf(580 - (temp->car.x[0] - CAR_HEIGHT)) > 0)
                    {
                        temp->car.speed = 0;
                    }
                    else
                    {
                        if (temp->car.speed > 5) temp->car.speed -= 0.0015;
                    }
                }
                else if (temp->car.stop == false) temp->car.speed += 0.025;
            }
            else if (temp->car.direction == 4)
            {
                if (temp->car.x[0] < 240)
                {
                    if (240 - (temp->car.x[0]) < 20 && 240 - (temp->car.x[0]) > 0)
                    {
                        temp->car.speed = 0;
                    }
                    else
                    {
                        if (temp->car.speed > 5) temp->car.speed -= 0.0015;
                    }
                }
                else if (temp->car.stop == false) temp->car.speed += 0.025;
            }
            temp = temp->next_car;
        }
    }
}

void check_cars(Cars* head)
{
    Cars* temp = head, * ptr = head;
    while (temp != NULL)
    {
        if (temp->car.direction == 1)
        {
            while (ptr != NULL)
            {
                if (ptr->car.direction == 3 || ptr->car.direction == 4 || ptr->car.direction == 5 || ptr->car.direction == 6)
                {
                    if ((temp->car.y[0] + CAR_HEIGHT) - (ptr->car.y[0] - CAR_WIDTH) < 0 && fabsf((temp->car.y[0] + CAR_HEIGHT) - (ptr->car.y[0] - CAR_WIDTH)) <= CAR_WIDTH / 2)
                    {
                        if ((ptr->car.x[0] - CAR_HEIGHT) - (temp->car.x[0] + CAR_WIDTH) < 0 && fabsf((ptr->car.x[0] - CAR_HEIGHT) - (temp->car.x[0] + CAR_WIDTH)) <= CAR_WIDTH + CAR_HEIGHT)
                        {
                            temp->car.speed = 0;
                        }
                    }
                }
                ptr = ptr->next_car;
            }
        }
        else if (temp->car.direction == 2)
        {
            while (ptr != NULL)
            {
                if (ptr->car.direction == 3 || ptr->car.direction == 4 || ptr->car.direction == 5 || ptr->car.direction == 6)
                {
                    if (temp->car.y[0] - ptr->car.y[0] > 0 && temp->car.y[0] - ptr->car.y[0] <= CAR_WIDTH / 2)
                    {
                        if ((ptr->car.x[0] - CAR_HEIGHT) - (temp->car.x[0] + CAR_WIDTH) < 0 && fabsf((ptr->car.x[0] - CAR_HEIGHT) - (temp->car.x[0] + CAR_WIDTH)) <= CAR_HEIGHT + CAR_WIDTH)
                        {
                            temp->car.speed = 0;
                        }
                    }
                }
                ptr = ptr->next_car;
            }
        }
        else if (temp->car.direction == 3 || temp->car.direction == 5)
        {
            while (ptr != NULL)
            {
                if (ptr->car.direction == 1 || ptr->car.direction == 2)
                {
                    if ((temp->car.x[0] - CAR_HEIGHT) - (ptr->car.x[0] + CAR_WIDTH) > 0 && (temp->car.x[0] - CAR_HEIGHT) - (ptr->car.x[0] + CAR_WIDTH) <= CAR_WIDTH / 2)
                    {
                        if (ptr->car.y[0] - temp->car.y[0] < 0 && fabsf(ptr->car.y[0] - temp->car.y[0]) <= CAR_HEIGHT + CAR_WIDTH)
                        {
                            temp->car.speed = 0;
                        }
                    }
                }
                ptr = ptr->next_car;
            }
        }
        else if (temp->car.direction == 4 || temp->car.direction == 6)
        {
            while (ptr != NULL)
            {
                if (ptr->car.direction == 1 || ptr->car.direction == 2)
                {
                    if (temp->car.x[0] - ptr->car.x[0] < 0 && fabsf(temp->car.x[0] - ptr->car.x[0]) <= CAR_WIDTH / 2)
                    {
                        if (ptr->car.y[0] - temp->car.y[0] < 0 && fabsf(ptr->car.y[0] - temp->car.y[0]) <= CAR_HEIGHT + CAR_WIDTH)
                        {
                            temp->car.speed = 0;
                        }
                    }
                }
                ptr = ptr->next_car;
            }
        }
        ptr = head;
        temp = temp->next_car;
    }
}

void mult_check_light(Cars* head)
{
    Cars* temp = head;
    if (model.traffic_light1 == false && model.traffic_light2 == true)
    {
        while (temp != NULL)
        {
            if (temp->car.direction == 3 || temp->car.direction == 4 || temp->car.direction == 5 || temp->car.direction == 6)
            {
                if (temp->car.stop == false)
                {
                    if (temp->car.speed == 0)
                    {
                        temp->car.speed = rand() % 30;
                        temp->car.speed += 60;
                    }
                    temp->car.speed += 0.005;
                }
            }
            else if (temp->car.direction == 1)
            {
                if (temp->car.y[0] < 85)
                {
                    if (85 - (temp->car.y[0] + CAR_HEIGHT) < 20 && 85 - (temp->car.y[0] + CAR_HEIGHT) > 0) temp->car.speed = 0;
                    else
                    {
                        if (temp->car.speed > 1) temp->car.speed -= 0.005;
                    }
                }
                else if (temp->car.y[0] > 280 && temp->car.y[0] < 420)
                {
                    if (420 - (temp->car.y[0] + CAR_HEIGHT) < 20 && 420 - (temp->car.y[0] + CAR_HEIGHT) > 0) temp->car.speed = 0;
                    else
                    {
                        if (temp->car.speed > 1) temp->car.speed -= 0.005;
                    }
                }
                else if (temp->car.stop == false) temp->car.speed += 0.04;
            }
            else if (temp->car.direction == 2)
            {
                if (temp->car.y[0] > 725)
                {
                    if (fabsf(725 - temp->car.y[0]) < 20 && fabsf(725 - temp->car.y[0]) > 0) temp->car.speed = 0;
                    else
                    {
                        if (temp->car.speed > 1) temp->car.speed -= 0.005;
                    }
                }
                else if (temp->car.y[0] > 280 && temp->car.y[0] < 420)
                {
                    if (fabsf(280 - temp->car.y[0]) < 20 && fabsf(280 - temp->car.y[0]) > 0) temp->car.speed = 0;
                    else
                    {
                        if (temp->car.speed > 1) temp->car.speed -= 0.005;
                    }
                }
                else if (temp->car.stop == false) temp->car.speed += 0.04;
            }
            temp = temp->next_car;
        }
    }
    else if (model.traffic_light1 == true && model.traffic_light2 == false)
    {
        while (temp != NULL)
        {
            if (temp->car.direction == 1 || temp->car.direction == 2)
            {
                if (temp->car.stop == false)
                {
                    if (temp->car.speed == 0)
                    {
                        temp->car.speed = rand() % 30;
                        temp->car.speed += 60;
                    }
                    temp->car.speed += 0.005;
                }
            }
            else if (temp->car.direction == 3 || temp->car.direction == 5)
            {
                if (temp->car.x[0] > 575)
                {
                    if (fabsf(575 - (temp->car.x[0] - CAR_HEIGHT)) < 20 && fabsf(575 - (temp->car.x[0] - CAR_HEIGHT)) > 0)
                    {
                        temp->car.speed = 0;
                    }
                    else
                    {
                        if (temp->car.speed > 1) temp->car.speed -= 0.005;
                    }
                }
                else if (temp->car.stop == false) temp->car.speed += 0.04;
            }
            else if (temp->car.direction == 4 || temp->car.direction == 6)
            {
                if (temp->car.x[0] < 240)
                {
                    if (240 - (temp->car.x[0]) < 20 && 240 - (temp->car.x[0]) > 0)
                    {
                        temp->car.speed = 0;
                    }
                    else
                    {
                        if (temp->car.speed > 5) temp->car.speed -= 0.005;
                    }
                }
                else if (temp->car.stop == false) temp->car.speed += 0.04;
            }
            temp = temp->next_car;
        }
    }
}
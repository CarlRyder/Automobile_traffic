#include "defines.h"
#include "textures.h"
#include "objects.h"

void points(int x1, int x2, int y1, int y2)
{
    glVertex2i(x1, y1);
    glVertex2i(x1, y2);
    glVertex2i(x2, y2);
    glVertex2i(x2, y1);
}

void control_lines(int line)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glColor4f(0.93, 0.86, 0.5, 0.3);
    glBegin(GL_QUADS);
    if (line == 1) points(275, 316, 0, 800);
    else if (line == 2) points(321, 359, 0, 800);
    else if (line == 3) points(365, 403, 0, 800);
    else if (line == 4) points(409, 450, 0, 800);
    else if (line == 5) points(456, 497, 0, 800);
    else if (line == 6) points(499, 541, 0, 800);
    if (enable.map_2 == true)
    {
        if (line == 7) points(0, 800, 303, 267);
        else if (line == 8) points(0, 800, 349, 306);
        else if (line == 9) points(0, 800, 394, 352);
        else if (line == 10) points(0, 800, 440, 399);
        else if (line == 11) points(0, 800, 483, 444);
        else if (line == 12) points(0, 800, 532, 487);
    }
    if (enable.map_3 == true)
    {
        if (line == 7) points(0, 800, 141, 107);
        else if (line == 8) points(0, 800, 181, 148);
        else if (line == 9) points(0, 800, 221, 187);
        else if (line == 10) points(0, 800, 261, 228);
        else if (line == 11) points(0, 800, 491, 456);
        else if (line == 12) points(0, 800, 526, 496);
        else if (line == 13) points(0, 800, 568, 532);
        else if (line == 14) points(0, 800, 610, 574);
        else if (line == 15) points(0, 800, 646, 616);
        else if (line == 16) points(0, 800, 692, 652);
    }
    glEnd();
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

void map_show(int value)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, map_tex.texture[value]);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex2f(0, 800); // LV
    glTexCoord2f(1, 1); glVertex2f(800, 800); // RV
    glTexCoord2f(1, 0); glVertex2f(800, 0); // RN
    glTexCoord2f(0, 0); glVertex2f(0, 0); // LN
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    control_lines(model.line);
}

void pause_background()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glColor4f(0.31, 0.31, 0.31, 0.7);
    glBegin(GL_QUADS);
    points(0, 800, 0, 800);
    glEnd();
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

void car_static_show(Cars* head)
{
    Cars* tmp = head;
    while (tmp != NULL)
    {
        car_draw(&tmp->car);
        tmp = tmp->next_car;
    }
}

void pause(int map, Cars* head, bool flag)
{
    map_show(map);
    if (head != NULL) car_static_show(head);
    pause_background();
    glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
    drawstring(MID_COORD - 24, WINDOW_HEIGHT - 250, "PAUSE");
    drawstring(MID_COORD - 140, WINDOW_HEIGHT - 350, "To select, press the appropriate key:");
    drawstring(MID_COORD - 131, WINDOW_HEIGHT - 380, "F1 - exit to the menu without saving");
    drawstring(MID_COORD - 133, WINDOW_HEIGHT - 410, "F2 - save the current model to a file");
    drawstring(MID_COORD - 129, WINDOW_HEIGHT - 440, "F3 - add a car to the selected lane");
    if (flag == true) drawstring(MID_COORD - 190, WINDOW_HEIGHT - 50, "The model has been successfully saved to a file!");
    if (model.add_car == true)
    {
        drawstring(MID_COORD - 210, WINDOW_HEIGHT - 50, "The car was successfully created on the lane you selected!");
        model.add_car = false;
    }
    glutSwapBuffers();
}

void car_position(car_model* car, int first, int second, bool vertically)
{
    if (vertically == true)
    {
        glTexCoord2f(first, second); glVertex2f(car->x[0], car->y[0] + CAR_HEIGHT); // LV
        glTexCoord2f(second, second); glVertex2f(car->x[0] + CAR_WIDTH, car->y[0] + CAR_HEIGHT); // RV
        glTexCoord2f(second, first); glVertex2f(car->x[0] + CAR_WIDTH, car->y[0]); // RN
        glTexCoord2f(first, first); glVertex2f(car->x[0], car->y[0]); // LN
    }
    else
    {
        glTexCoord2f(first, first); glVertex2f(car->x[0] - CAR_HEIGHT, car->y[0]); // LV
        glTexCoord2f(first, second); glVertex2f(car->x[0], car->y[0]); // RV
        glTexCoord2f(second, second); glVertex2f(car->x[0], car->y[0] - CAR_WIDTH); // RN
        glTexCoord2f(second, first); glVertex2f(car->x[0] - CAR_HEIGHT, car->y[0] - CAR_WIDTH); // LN
    }
}

void car_draw(car_model* car)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, car_tex.texture[car->texture_id]);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    if (enable.map_1 == true)
    {
        if (car->direction == 1) car_position(car, 0, 1, true);
        else if (car->direction == 2) car_position(car, 1, 0, true);
    }
    else if (enable.map_2 == true)
    {
        if (car->direction == 1)
        {
            if (car->line == 3 && car->turn == false)
            {
                if (car->y[0] > 230 && car->y[0] < 250)
                {
                    car_position(car, 0, 1, true);
                    if (active.pause_active == false) car->x[0] += 0.02;
                }
                else if (car->y[0] > 250 && car->y[0] < 255)
                {
                    car->direction = 4;
                    car->x[0] = 545;
                    car->y[0] = 296;
                    car->line = 1;
                    car->turn = true;
                }
                else car_position(car, 0, 1, true);
            }
            else car_position(car, 0, 1, true);
        }
        else if (car->direction == 2)
        {
            if (car->line == 1 && car->turn == false)
            {
                if (car->y[0] > 515 && car->y[0] < 535)
                {
                    car_position(car, 1, 0, true);
                    if (active.pause_active == false) car->x[0] -= 0.02;
                }
                else if (car->y[0] < 515 && car->y[0] > 510)
                {
                    car->direction = 3;
                    car->x[0] = 275;
                    car->y[0] = 525;
                    car->line = 3;
                    car->turn = true;
                }
                else car_position(car, 1, 0, true);
            }
            else car_position(car, 1, 0, true);
        }
        else if (car->direction == 3)
        {
            if (car->line == 3 && car->turn == false)
            {
                if (car->x[0] > 560 && car->x[0] < 580)
                {
                    car_position(car, 1, 0, false);
                    if (active.pause_active == false) car->y[0] += 0.02;
                }
                else if (car->x[0] < 560 && car->x[0] > 555)
                {
                    car->direction = 1;
                    car->x[0] = 510;
                    car->y[0] = 540;
                    car->line = 3;
                    car->turn = true;
                }
                else car_position(car, 1, 0, false);
            }
            else car_position(car, 1, 0, false);
        }
        else if (car->direction == 4)
        {
            if (car->line == 1 && car->turn == false)
            {
                if (car->x[0] > 280 && car->x[0] < 300)
                {
                    car_position(car, 0, 1, false);
                    if (active.pause_active == false) car->y[0] -= 0.02;
                }
                else if (car->x[0] > 300 && car->x[0] < 305)
                {
                    car->direction = 2;
                    car->x[0] = 283;
                    car->y[0] = 245;
                    car->line = 1;
                    car->turn = true;
                }
                else car_position(car, 0, 1, false);
            }
            else car_position(car, 0, 1, false);
        }
    }
    else if (enable.map_3 == true)
    {
        if (car->direction == 1)
        {
            if (car->line == 3)
            {
                if (car->y[0] > 60 && car->y[0] < 80)
                {
                    car_position(car, 0, 1, true);
                    if (active.pause_active == false) car->x[0] += 0.02;
                }
                else if (car->y[0] > 80 && car->y[0] < 85)
                {
                    car->direction = 4;
                    car->x[0] = 560;
                    car->y[0] = 135;
                    car->line = 1;
                    car->turn = true;
                }
                else if (car->y[0] > 420 && car->y[0] < 440)
                {
                    car_position(car, 0, 1, true);
                    if (active.pause_active == false) car->x[0] += 0.02;
                }
                else if (car->y[0] > 440 && car->y[0] < 445)
                {
                    car->direction = 6;
                    car->x[0] = 560;
                    car->y[0] = 485;
                    car->line = 1;
                    car->turn = true;
                }
                else car_position(car, 0, 1, true);
            }
            else car_position(car, 0, 1, true);
        }
        else if (car->direction == 2)
        {
            if (car->line == 1)
            {
                if (car->y[0] + CAR_HEIGHT > 725 && car->y[0] + CAR_HEIGHT < 745)
                {
                    car_position(car, 1, 0, true);
                    if (active.pause_active == false) car->x[0] -= 0.02;
                }
                else if (car->y[0] + CAR_HEIGHT < 725 && car->y[0] + CAR_HEIGHT > 720)
                {
                    car->direction = 5;
                    car->x[0] = 270;
                    car->y[0] = 685;
                    car->line = 3;
                    car->turn = true;
                }
                else if (car->y[0] + CAR_HEIGHT > 280 && car->y[0] + CAR_HEIGHT < 300)
                {
                    car_position(car, 1, 0, true);
                    if (active.pause_active == false) car->x[0] -= 0.02;
                }
                else if (car->y[0] + CAR_HEIGHT < 280 && car->y[0] + CAR_HEIGHT > 275)
                {
                    car->direction = 3;
                    car->x[0] = 270;
                    car->y[0] = 255;
                    car->line = 2;
                    car->turn = true;
                }
                else car_position(car, 1, 0, true);
            }
            else car_position(car, 1, 0, true);
        }
        else if (car->direction == 3)
        {
            if (car->line == 2)
            {
                if (car->x[0] > 550 && car->x[0] < 570)
                {
                    car_position(car, 1, 0, false);
                    if (active.pause_active == false) car->y[0] += 0.02;
                }
                else if (car->x[0] < 550 && car->x[0] > 545)
                {
                    car->direction = 1;
                    car->x[0] = 510;
                    car->y[0] = 240;
                    car->line = 3;
                    car->turn = true;
                }
                else car_position(car, 1, 0, false);
            }
            else car_position(car, 1, 0, false);
        }
        else if (car->direction == 4)
        {
            if (car->line == 1)
            {
                if (car->x[0] > 280 && car->x[0] < 300)
                {
                    car_position(car, 0, 1, false);
                    if (active.pause_active == false) car->y[0] -= 0.02;
                }
                else if (car->x[0] > 300 && car->x[0] < 305)
                {
                    car->direction = 2;
                    car->x[0] = 283;
                    car->y[0] = 100;
                    car->line = 1;
                    car->turn = true;
                }
                else car_position(car, 0, 1, false);
            }
            else car_position(car, 0, 1, false);
        }
        else if (car->direction == 5)
        {
            if (car->line == 3)
            {
                if (car->x[0] > 550 && car->x[0] < 570)
                {
                    car_position(car, 1, 0, false);
                    if (active.pause_active == false) car->y[0] += 0.02;
                }
                else if (car->x[0] < 550 && car->x[0] > 545)
                {
                    car->direction = 1;
                    car->x[0] = 510;
                    car->y[0] = 695;
                    car->line = 3;
                    car->turn = true;
                }
                else car_position(car, 1, 0, false);
            }
            else car_position(car, 1, 0, false);
        }
        else if (car->direction == 6)
        {
            if (car->line == 1)
            {
                if (car->x[0] > 290 && car->x[0] < 310)
                {
                    car_position(car, 0, 1, false);
                    if (active.pause_active == false) car->y[0] -= 0.02;
                }
                else if (car->x[0] > 310 && car->x[0] < 315)
                {
                    car->direction = 2;
                    car->x[0] = 283;
                    car->y[0] = 450;
                    car->line = 1;
                    car->turn = true;
                }
                else car_position(car, 0, 1, false);
            }
            else car_position(car, 0, 1, false);
        }
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void light(int x, int y)
{
    glBegin(GL_QUADS);
    glVertex2i(x, WINDOW_HEIGHT - y);
    glVertex2i(x, WINDOW_HEIGHT - y + LIGHT_SIZE);
    glVertex2i(x + LIGHT_SIZE, WINDOW_HEIGHT - y + LIGHT_SIZE);
    glVertex2i(x + LIGHT_SIZE, WINDOW_HEIGHT - y);
    glEnd();
}

void light_draw(int x, int y1, int y2, int y3, bool flag)
{
    if (flag == true)
    {
        glColor3ub(0, 255, 0);
        light(x, y1);
        glColor3ub(70, 68, 81);
        light(x, y2);
        light(x, y3);
    }
    else
    {
        glColor3ub(70, 68, 81);
        light(x, y1);
        light(x, y2);
        glColor3ub(255, 0, 0);
        light(x, y3);
    }
}

void traffic_lights()
{
    if (model.traffic_light1 == true && model.traffic_light2 == false)
    {
        light_draw(565, 615, 592, 568, true);
        light_draw(229, 214, 191, 169, true);
        light_draw(217, 615, 592, 568, false);
        light_draw(565, 214, 191, 169, false);
    }
    else if (model.traffic_light1 == false && model.traffic_light2 == true)
    {
        light_draw(565, 615, 592, 568, false);
        light_draw(229, 214, 191, 169, false);
        light_draw(217, 615, 592, 568, true);
        light_draw(565, 214, 191, 169, true);
    }
}

void mult_traffic_lights()
{
    if (model.traffic_light1 == true && model.traffic_light2 == false)
    {
        light_draw(567, 765, 745, 724, true);
        light_draw(566, 502, 482, 461, false);
        light_draw(568, 416, 395, 375, true);
        light_draw(580, 78, 58, 37, false);
        light_draw(231, 765, 745, 724, false);
        light_draw(230, 502, 482, 461, true);
        light_draw(231, 416, 395, 375, false);
        light_draw(228, 78, 58, 37, true);
    }
    else if (model.traffic_light1 == false && model.traffic_light2 == true)
    {
        light_draw(567, 765, 745, 724, false);
        light_draw(566, 502, 482, 461, true);
        light_draw(568, 416, 395, 375, false);
        light_draw(580, 78, 58, 37, true);
        light_draw(231, 765, 745, 724, true);
        light_draw(230, 502, 482, 461, false);
        light_draw(231, 416, 395, 375, true);
        light_draw(228, 78, 58, 37, false);
    }
}
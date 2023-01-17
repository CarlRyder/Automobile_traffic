#define _CRT_SECURE_NO_WARNINGS

#include "menu.h"
#include "defines.h"

void drawstring(float x, float y, char* string)
{
    glRasterPos2i(x, y);
    for (char* c = string; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

void menu_line()
{
    glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
    glBegin(GL_QUADS);
    glVertex2i(285, 591);
    glVertex2i(285, 593);
    glVertex2i(520, 593);
    glVertex2i(520, 591);
    glEnd();
}

void menu()
{
    // If model load return error
    if (active.load_error == true)
    {
        glColor3ub(239, 211, 52);
        drawstring(MID_COORD - 130, 50, "Error loading the finished model!");
        active.load_error = false;
    }
    menu_line();
    srand(time(NULL));
    // Drawing menu buttons
    menu_buttons(-8);
    menu_buttons(-5);
    menu_buttons(-2);
    menu_buttons(1);
    // Rendering text in the menu
    for (int i = 0; i < 5; i++) text_buttons(i);
    active.menu_active = true;
}

void processing_buttons(int button)
{
    if (button == 3)
    {
        buttons.menu_button_3 = true;
        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        menu_line();
        glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
        drawstring(285, 600, "CAR TRAFFIC SIMULATOR");
        drawstring(353, 500, "Developers:");
        drawstring(260, 470, "Sendov Yakov Denisovich | @cryder");
        drawstring(240, 440, "Babaev Salavat Shakirovich | @lilbabidjon");
        drawstring(337, 360, "4831001/10003");
        drawstring(200, 330, "Institute of Cybersecurity and Information Protection");
        drawstring(133, 300, "Copyright (c) 2022 Saint-Petersburg Polytechnic University, Russia");
        menu_buttons(-7);
        glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
        drawstring(380, 220, "Back");
        active.menu_active = false;
        glutSwapBuffers();
    }
    else if (button == 2)
    {
        buttons.menu_button_2 = true;
        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        menu_line();
        glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
        drawstring(285, 600, "CAR TRAFFIC SIMULATOR");
        drawstring(363, 500, "Settings:");
        drawstring(46, 440, "Spawn interval of machines (seconds):");
        drawstring(280, 410, "Autosave:");
        settings_buttons();
        menu_buttons(-7);
        glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
        drawstring(380, 220, "Back");
        active.menu_active = false;
        glutSwapBuffers();
    }
    else if (button == 1)
    {
        buttons.menu_button_1 = true;
        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        draw_welcome_text();
        draw_next_buttons(-290, 0);
        draw_next_buttons(0, 80);
        glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
        drawstring(235, 220, "Back");
        drawstring(415, 220, "Next");
        active.menu_active = false;
        glutSwapBuffers();
    }
}

void settings_buttons()
{
    // Interval buttons
    if (model.interval == 0.75)
    {
        draw_mini_buttons(0, 365, 1);
        draw_mini_buttons(90, 365, 0);
        draw_mini_buttons(180, 365, 0);
    }
    else if (model.interval == 1)
    {
        draw_mini_buttons(0, 365, 0);
        draw_mini_buttons(90, 365, 1);
        draw_mini_buttons(180, 365, 0);
    }
    else if (model.interval == 1.25)
    {
        draw_mini_buttons(0, 365, 0);
        draw_mini_buttons(90, 365, 0);
        draw_mini_buttons(180, 365, 1);
    }
    // Autosave buttons
    if (model.autosave == true)
    {
        draw_mini_buttons(0, 395, 1);
        draw_mini_buttons(90, 395, 0);
    }
    else if (model.autosave == false)
    {
        draw_mini_buttons(0, 395, 0);
        draw_mini_buttons(90, 395, 1);
    }
}

void draw_mini_buttons(int w, int h, int flag)
{
    if (flag == 0) glColor3ub(COLOR_BUTTON_RED, COLOR_BUTTON_GREEN, COLOR_BUTTON_BLUE);
    else if (flag == 1) glColor3ub(0, 153, 0);
    glBegin(GL_QUADS);
    glVertex2i(MID_COORD + w, WINDOW_HEIGHT - h);
    glVertex2i(MID_COORD + w, WINDOW_HEIGHT - h + MINI_BUTTON_HEIGHT);
    glVertex2i(MID_COORD + MINI_BUTTON_WIDTH + w, WINDOW_HEIGHT - h + MINI_BUTTON_HEIGHT);
    glVertex2i(MID_COORD + MINI_BUTTON_WIDTH + w, WINDOW_HEIGHT - h);
    glEnd();
    if (h == 365)
    {
        glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
        if (w == 0) drawstring(w + MID_COORD + (MINI_BUTTON_WIDTH >> 1) - 18, WINDOW_HEIGHT - h + 4, "0.75");
        else if (w == 90) drawstring(w + MID_COORD + (MINI_BUTTON_WIDTH >> 1) - 5, WINDOW_HEIGHT - h + 4, "1");
        else if (w == 180) drawstring(w + MID_COORD + (MINI_BUTTON_WIDTH >> 1) - 18, WINDOW_HEIGHT - h + 4, "1.25");
    }
    else if (h == 395)
    {
        glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
        if (w == 0) drawstring(w + MID_COORD + (MINI_BUTTON_WIDTH >> 1) - 15, WINDOW_HEIGHT - h + 5, "yes");
        else if (w == 90) drawstring(w + MID_COORD + (MINI_BUTTON_WIDTH >> 1) - 10, WINDOW_HEIGHT - h + 5, "no");
    }
}

void draw_welcome_text()
{
    glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
    drawstring(305, 600, "AUTOMOBILE TRAFFIC");
    drawstring(200, 530, "Good afternoon! A Car Traffic Simulator welcomes you.");
    drawstring(220, 500, "There are several types of roads available to you:");
    drawstring(130, 470, "a motorway, a road with an intersection, a road with several intersections.");
    drawstring(200, 440, "You can simulate an accident in one of the traffic lanes.");
    drawstring(205, 410, "To do this, you need to select one of the bands using");
    drawstring(250, 380, "the arrows on the keyboard (<- and ->).");
    glColor3ub(239, 211, 52);
    drawstring(220, 350, "The strip you selected will be highlighted in yellow!");
    glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
    drawstring(160, 320, "To simulate an accident situation, you need to press the space bar.");
    drawstring(320, 290, "To continue, click next.");
}

void draw_next_buttons(int a, int b)
{
    glColor3ub(COLOR_BUTTON_RED, COLOR_BUTTON_GREEN, COLOR_BUTTON_BLUE);
    glBegin(GL_QUADS);
    glVertex2i(MENU_BUTTON_LEFT + b, MENU_BUTTON_BOTTOM + (-7) * (BUTTON_HEIGHT >> 1));
    glVertex2i(MENU_BUTTON_LEFT + b, MENU_BUTTON_TOP + (-7) * (BUTTON_HEIGHT >> 1));
    glVertex2i(MENU_BUTTON_RIGHT + a, MENU_BUTTON_TOP + (-7) * (BUTTON_HEIGHT >> 1));
    glVertex2i(MENU_BUTTON_RIGHT + a, MENU_BUTTON_BOTTOM + (-7) * (BUTTON_HEIGHT >> 1));
    glEnd();
}

void text_buttons(int flag)
{
    glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
    if (flag == 0) drawstring(285, 600, "CAR TRAFFIC SIMULATOR");
    else if (flag == 1) drawstring(330, 420, "Automobile traffic");
    else if (flag == 2) drawstring(360, 345, "Settings");
    else if (flag == 3) drawstring(360, 270, "About us");
    else if (flag == 4) drawstring(380, 195, "Exit");
}

void map_choose()
{
    buttons.map_button = true;
    glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    menu_buttons(-8);
    menu_buttons(-5);
    menu_buttons(-2);
    menu_buttons(1);
    menu_buttons(4);
    glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
    drawstring(MID_COORD - 95, WINDOW_HEIGHT - 200, "AUTOMOBILE TRAFFIC");
    drawstring(MID_COORD - 40, WINDOW_HEIGHT - 305, "Motorway");
    drawstring(MID_COORD - 100, WINDOW_HEIGHT - 380, "Road with an intersection");
    drawstring(MID_COORD - 120, WINDOW_HEIGHT - 455, "Road with several intersections");
    drawstring(MID_COORD - 80, WINDOW_HEIGHT - 530, "Load a saved model");
    drawstring(MID_COORD - 20, WINDOW_HEIGHT - 605, "Back");
    active.menu_active = false;
    glutSwapBuffers();
}

void menu_buttons(int h)
{
    glColor3ub(COLOR_BUTTON_RED, COLOR_BUTTON_GREEN, COLOR_BUTTON_BLUE);
    glBegin(GL_QUADS);
    glVertex2i(MENU_BUTTON_LEFT, MENU_BUTTON_BOTTOM + h * (BUTTON_HEIGHT >> 1));
    glVertex2i(MENU_BUTTON_LEFT, MENU_BUTTON_TOP + h * (BUTTON_HEIGHT >> 1));
    glVertex2i(MENU_BUTTON_RIGHT, MENU_BUTTON_TOP + h * (BUTTON_HEIGHT >> 1));
    glVertex2i(MENU_BUTTON_RIGHT, MENU_BUTTON_BOTTOM + h * (BUTTON_HEIGHT >> 1));
    glEnd();
}
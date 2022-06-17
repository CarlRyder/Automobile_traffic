/* Copyright (c) Yakov D. Sendov & Salavat S. Babaev, 2022 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <GL/glut.h>

#define WINDOW_NAME "Car Traffic Simulator"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define BUTTON_WIDTH 350
#define BUTTON_HEIGHT 50

// COLORS
#define COLOR_MENU_RED 0.102
#define COLOR_MENU_GREEN 0.082
#define COLOR_MENU_BLUE 0.247 

GLint Width = 800, Height = 800;
bool mouse_status = 0;
int mouse_x = 0;
int mouse_y = 0;

struct menu_button
{
    int x_coords[4];
    int y_coords[4];
};

void drawstring(float x, float y, char* string)
{
    glRasterPos2i(x, y);
    for (char* c = string; *c != '\0'; c++) 
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void menu_buttons(struct menu_button* button, int h)
{
    int delta = BUTTON_HEIGHT;
    int midX = Width / 2;
    int midY = Height / 2;
    int left, right, top, bottom;
    left = midX - BUTTON_WIDTH / 2;
    right = left + BUTTON_WIDTH;
    bottom = midY - BUTTON_HEIGHT / 2;
    top = bottom + BUTTON_HEIGHT;

    glColor3ub(138, 43, 226);
    glBegin(GL_QUADS);
    glVertex2i(left, bottom + (h - 3) * delta / 2);
    glVertex2i(left, top + (h - 3) * delta / 2);
    glVertex2i(right, top + (h - 3) * delta / 2);
    glVertex2i(right, bottom + (h - 3) * delta / 2);
    glEnd();

    button->x_coords[0] = left;
    button->y_coords[0] = 800 - bottom + (h - 3) * delta / 2;
    button->x_coords[1] = button->x_coords[0];
    button->y_coords[1] = 800 - top + (h - 3) * delta / 2;
    button->x_coords[2] = right;
    button->y_coords[2] = button->y_coords[1];
    button->x_coords[3] = button->x_coords[2];
    button->y_coords[3] = button->y_coords[0];
}

void text_buttons(struct menu_button* button, int flag)
{
    glColor3ub(255, 255, 255);
    if (flag == 0) drawstring(Width / 2 - 115, Height - 200, "CAR TRAFFIC SIMULATOR");
    if (button != NULL)
    {
        int delta = (button->x_coords[2] - button->x_coords[0]);
        // button->x_coords[0] - 76
        if (flag == 1) drawstring(delta - 20, button->y_coords[0] - 0.6 * BUTTON_HEIGHT, "Automobile traffic");
        else if (flag == 2) drawstring(delta + 15, button->y_coords[0] - 0.6 * BUTTON_HEIGHT, "Settings");
        else if (flag == 3) drawstring(delta + 15, button->y_coords[0] - 0.6 * BUTTON_HEIGHT, "About us");
        else if (flag == 4) drawstring(delta + 31, button->y_coords[0] - 0.6 * BUTTON_HEIGHT, "Exit");
    }
}


void display()
{
    glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    struct menu_button first_button, second_button, third_button, fourth_button;
    // Drawing menu buttons
    menu_buttons(&first_button, 4); 
    menu_buttons(&second_button, 1);
    menu_buttons(&third_button, -2); 
    menu_buttons(&fourth_button, -5);

    // Rendering text in the menu
    text_buttons(NULL, 0);
    text_buttons(&first_button, 1);
    text_buttons(&second_button, 2);
    text_buttons(&third_button, 3);
    text_buttons(&fourth_button, 4);

    glColor3ub(255, 255, 255);
    glBegin(GL_LINES);
    glVertex2i(400, 0);
    glVertex2i(400, 800);
    glVertex2i(0, 400);
    glVertex2i(800, 400);
    glEnd();

    glFinish();
}

void reshape(GLint w, GLint h)
{
    Width = w;
    Height = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouse_move(int ax, int ay)
{
    glutSetWindowTitle("MOVE");
}

void mouse_pressed(int button, int state, int x, int y)
{
    switch (button)
    {
    case GLUT_LEFT_BUTTON: glutSetWindowTitle("LEFT BUTTON"); break;
    case GLUT_RIGHT_BUTTON: glutSetWindowTitle("RIGHT BUTTON"); break;
    }
}

void keyboard(unsigned char key, int x, int y)
{
    #define ESCAPE '\033'
    if (key == ESCAPE)
        exit(0);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_NAME);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(mouse_move);
    glutMouseFunc(mouse_pressed);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
}
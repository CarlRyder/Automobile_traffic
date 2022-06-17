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
bool menu_activity = false;
bool menu_button_3 = false;

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

void coord_lines()
{
    glColor3ub(255, 255, 255);
    glBegin(GL_LINES);
    glVertex2i(400, 0);
    glVertex2i(400, 800);
    glVertex2i(0, 400);
    glVertex2i(800, 400);
    glEnd();
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
        if (flag == 1) drawstring(delta - 20, button->y_coords[0] - 0.6 * BUTTON_HEIGHT, "Automobile traffic");
        else if (flag == 2) drawstring(delta + 15, button->y_coords[0] - 0.6 * BUTTON_HEIGHT, "Settings");
        else if (flag == 3) drawstring(delta + 15, button->y_coords[0] - 0.6 * BUTTON_HEIGHT, "About us");
        else if (flag == 4) drawstring(delta + 31, button->y_coords[0] - 0.6 * BUTTON_HEIGHT, "Exit");
    }
}

void processing_buttons(int button)
{
    int delta = BUTTON_HEIGHT;
    int midX = Width / 2;
    int midY = Height / 2;
    int left, right, top, bottom;
    left = midX - BUTTON_WIDTH / 2;
    right = left + BUTTON_WIDTH;
    bottom = midY - BUTTON_HEIGHT / 2;
    top = bottom + BUTTON_HEIGHT;
    if (button == 3)
    {
        menu_button_3 = true;
        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3ub(255, 255, 255);
        drawstring(Width / 2 - 115, Height - 200, "CAR TRAFFIC SIMULATOR");
        drawstring(Width / 2 - 47, Height - 300, "Developers:");
        drawstring(Width / 2 - 140, Height - 330, "Sendov Yakov Denisovich | @cryder");
        drawstring(Width / 2 - 160, Height - 360, "Babaev Salavat Shakirovich | @lilbabidjon");
        drawstring(Width / 2 - 63, Height - 440, "4831001/10003");
        drawstring(Width / 4, Height - 470, "Institute of Cybersecurity and Information Protection");
        drawstring(Width / 6, Height - 500, "Copyright (c) 2022 Saint-Petersburg Polytechnic University, Russia");

        glColor3ub(138, 43, 226);
        glBegin(GL_QUADS);
        glVertex2i(left, bottom + (-7) * delta / 2);
        glVertex2i(left, top + (-7) * delta / 2);
        glVertex2i(right, top + (-7) * delta / 2);
        glVertex2i(right, bottom + (-7) * delta / 2);
        glEnd();

        glColor3ub(255, 255, 255);
        drawstring(Width / 2 - 20, Height - 580, "Back");
        menu_activity = false;
        glutSwapBuffers();
    }
    else if (button == 2)
    {

    }
    else if (button == 1)
    {

    }
}

void menu()
{
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
    menu_activity = true;
}

void display()
{
    glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    /* -------------------------- */

    menu();
    //coord_lines();

    /* -------------------------- */
    glutSwapBuffers();
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
    //glutSetWindowTitle("MOVE");
}

void mouse_pressed(int button, int state, int x, int y)
{
    switch (button)
    {
    case GLUT_LEFT_BUTTON: 
    {
        printf("%d | %d\n", x, y);
        if (state == GLUT_DOWN && menu_activity == true)
        {
            if (x <= 575 && x >= 275 && y >= 575 && y <= 625) exit(0);
            else if (x <= 575 && x >= 275 && y >= 500 && y <= 550) processing_buttons(3);
        }
        else if (state == GLUT_DOWN && menu_button_3 == true)
        {
            if (x <= 575 && x >= 275 && y >= 550 && y <= 600)
            {
                menu_button_3 == false;
                glutPostRedisplay();
            }
        }
        break;
    }
    case GLUT_RIGHT_BUTTON: break;
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
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_NAME);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(mouse_move);
    glutMouseFunc(mouse_pressed);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
}
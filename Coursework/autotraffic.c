// Made by Y. Sendov and S. Babaev. Start work - March 2022.

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>

#define WINDOW_NAME "Car Traffic Simulator"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define BUTTON_WIDTH 350
#define BUTTON_HEIGHT 50

GLint Width = 800, Height = 800;

void drawstring(float x, float y, char* string)
{
    glRasterPos2i(x, y);
    for (char* c = string; *c != '\0'; c++) 
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void menu_buttons(int h)
{
    int midX = Width / 2;
    int midY = Height / 2;

    int left, right, top, bottom;
    left = midX - BUTTON_WIDTH / 2;
    right = left + BUTTON_WIDTH;
    bottom = midY - BUTTON_HEIGHT / 2;
    top = bottom + BUTTON_HEIGHT;

    glColor3ub(138, 43, 226);
    glBegin(GL_QUADS);
    glVertex2i(left, bottom + (h - 3) * BUTTON_HEIGHT / 2);
    glVertex2i(left, top + (h - 3) * BUTTON_HEIGHT / 2);
    glVertex2i(right, top + (h - 3) * BUTTON_HEIGHT / 2);
    glVertex2i(right, bottom + (h - 3) * BUTTON_HEIGHT / 2);
    glEnd();
}

void text_buttons(int flag)
{
    glColor3ub(255, 255, 255);
    if (flag == 0) drawstring(Width / 2 - 115, Height - 200, "CAR TRAFFIC SIMULATOR");
    else if (flag == 1) drawstring(Width / 2 - 76, Height - 356, "Automobile traffic");
    else if (flag == 2) drawstring(Width / 2 - 38, Height - 432, "Settings");
    else if (flag == 3) drawstring(Width / 2 - 38, Height - 506, "About us");
    else if (flag == 4) drawstring(Width / 2 - 15, Height - 581, "Exit");
}

void display()
{
    glClearColor(0.102, 0.082, 0.247, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // Buttons
    for (int i = 5; i > -5; i -= 3) menu_buttons(i);
    // Text buttons
    for (int i = 0; i < 5; i++) text_buttons(i);

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
    glutKeyboardFunc(keyboard);

    glutMainLoop();
}
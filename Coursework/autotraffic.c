// Made by Y. Sendov and S. Babaev. Start work - March 2022.

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>

#define WINDOW_NAME "Car Traffic Simulator"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

/* ширина и высота окна */
GLint Width = 800, Height = 800;

const int button_width = 350;
const int button_height = 50;

void menu_buttons(int h)
{
    int middle = Width / 2;

    int left, right, top, bottom;
    left = middle - button_width / 2;
    right = left + button_width;
    bottom = (Height - button_height) / 2;
    top = bottom + button_height;

    glColor3ub(138, 43, 226);

    glBegin(GL_QUADS);
    glVertex2i(left, bottom);
    glVertex2i(left, top);
    glVertex2i(right, top);
    glVertex2i(right, bottom);
    glEnd();
}

/* управляет всем выводом на экран */
void Display(void)
{
    glClearColor(0.102, 0.082, 0.247, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    menu_buttons(10);

    glFinish();
}

/* Вызывается при изменении размеров окна */
void Reshape(GLint w, GLint h)
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

/* Обрабатывает сообщения от клавиатуры */
void Keyboard(unsigned char key, int x, int y)
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

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);

    glutMainLoop();
}
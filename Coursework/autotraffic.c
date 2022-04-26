// Made by Y. Sendov and S. Babaev. Start work - March 2022.

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <GL/glut.h>

void Reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

// Функция чек

void Draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.0f, 1.0f);
    glLineWidth(1);

    glBegin(GL_LINES);
    glVertex2f(0, 0.5f);
    glVertex2f(0, -0.5f);
    glEnd();

    glFlush();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(0, 0);

    glutInitDisplayMode(GLUT_RGB);
    glutCreateWindow("Simulation of automobile traffic");

    glutReshapeFunc(Reshape);
    glutDisplayFunc(Draw);
    glClearColor(0, 0, 0, 0);

    glutMainLoop();
    return 0;
}
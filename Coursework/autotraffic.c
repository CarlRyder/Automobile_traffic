/* Copyright (c) Yakov D. Sendov & Salavat S. Babaev, 2022 */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <GL/glut.h>

#define WINDOW_NAME "Car Traffic Simulator"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define BUTTON_WIDTH 350
#define BUTTON_HEIGHT 50
#define MINI_BUTTON_WIDTH 70
#define MINI_BUTTON_HEIGHT 20
#define CAR_HEIGHT 50
#define CAR_WIDTH 30

// COLORS
#define COLOR_MENU_RED 0.102
#define COLOR_MENU_GREEN 0.082
#define COLOR_MENU_BLUE 0.247 

GLint Width = 800, Height = 800;
bool menu_activity = false;
bool menu_button_3 = false;
bool menu_button_2 = false;
bool menu_button_1 = false;
bool menu_changes_2 = false;
bool map_button = false;
bool model_active = false;

struct menu_button
{
    int x_coords[4];
    int y_coords[4];
};

struct settings
{
    int interval;
    int count_lanes;
    bool autosave;
} settings;

typedef struct
{
    float x[2];
    float y[2];
    int line;
    int color[3];
} Tcar;

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

//void draw_map()
//{
//    int x_start = 0;
//    int y_start = 800;
//    int i = 0;
//    while (i < Width * Height * 3)
//    {
//        glColor3ub(data[i], data[i + 1], data[i + 2]);
//        glPointSize(1);
//        glBegin(GL_POINTS);
//        glVertex2i(x_start, y_start);
//        glEnd();
//        x_start++;
//        if (x_start >= 800)
//        {
//            y_start--;
//            x_start = 0;
//        }
//        i += 3;
//    }
//}
//
//void get_bmp(char* filename)
//{
//    unsigned char RGB[3] = { 0 };
//    FILE* file = fopen(filename, "rb");
//    fseek(file, 54, 0);
//    fread(data, Width * Height * 3, sizeof(unsigned char), file);
//    fclose(file);
//}

void draw_newmap()
{
    // Grass
    glColor3ub(34, 139, 34);
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(0, 800);
    glVertex2i(800, 800);
    glVertex2i(800, 0);
    glEnd();
    // Road
    glColor3ub(70, 68, 81);
    glBegin(GL_QUADS);
    glVertex2i(250, 0);
    glVertex2i(250, 800);
    glVertex2i(550, 800);
    glVertex2i(550, 0);
    glEnd();
    // Road lines
    glColor3ub(255, 255, 255);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0X00FF);
    glBegin(GL_LINES);
    glVertex2i(300, 4);
    glVertex2i(300, 800);
    glVertex2i(350, 4);
    glVertex2i(350, 800);
    glVertex2i(450, 4);
    glVertex2i(450, 800);
    glVertex2i(500, 4);
    glVertex2i(500, 800);
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    glBegin(GL_LINES);
    glVertex2i(250, 0);
    glVertex2i(250, 800);
    glVertex2i(400, 0);
    glVertex2i(400, 800);
    glVertex2i(550, 0);
    glVertex2i(550, 800);
    glEnd();
}

void pause()
{
    draw_newmap();
    glColor3ub(255, 255, 255);
    drawstring(Width / 2 - 24, Height - 300, "PAUSE");
    glutSwapBuffers();
}

void mouse_pressed(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);

void draw_car(Tcar* car)
{
    draw_newmap();
    glColor3ub(car->color[0], car->color[1], car->color[2]);
    glBegin(GL_QUADS);
    glVertex2f(car->x[0], car->y[0]);
    glVertex2f(car->x[0], car->y[0] + CAR_HEIGHT);
    glVertex2f(car->x[0] + CAR_WIDTH, car->y[0] + CAR_HEIGHT);
    glVertex2f(car->x[0] + CAR_WIDTH, car->y[0]);
    glEnd();
    glutMouseFunc(mouse_pressed);
    glutKeyboardFunc(keyboard);
    glutSwapBuffers();
}

void init_car(Tcar* car)
{
    srand(time(NULL));
    car->line = rand() % 3 + 1;
    if (car->line == 1)
    {
        car->x[0] = 410;
        car->y[0] = 0;
    }
    else if (car->line == 2)
    {
        car->x[0] = 460;
        car->y[0] = 0;
    }
    else if (car->line == 3)
    {
        car->x[0] = 510;
        car->y[0] = 0;
    }
    for (int i = 0; i < 3; i++) car->color[i] = rand() % 255;
}

int x_pos = 416;
int y_pos = 0;

void timer(int value)
{
    if (model_active == true && y_pos < 820)
    {
        printf("%d\n", y_pos);
        y_pos += 5;
    }
    else y_pos = 0;
    glutTimerFunc(1000 / 60, timer, 0);
}

int car_pos(Tcar* car)
{
    if (car->y[0] < 810)
    {
        draw_car(car);
        car->y[0] = y_pos;
        return 1;
    }
    else return 0;
}

void motorway()
{
    if (model_active == true)
    {
        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        draw_newmap();
        glutSwapBuffers();

        time_t start = 0, end = 0;
        Tcar car;
        init_car(&car);
        //car_pos(&car);
        while (car.y[0] < 820)
        {
            draw_car(&car);
            car.y[0] += 0.05;
        }

        /*start = time(NULL);
        double diff = 0;
        do
        {
            end = time(NULL);
            diff = difftime(end, start);
        } while (diff < 3);*/
    }

    glutIdleFunc(motorway);
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

    if (button != NULL)
    {
        button->x_coords[0] = left;
        button->y_coords[0] = 800 - bottom + (h - 3) * delta / 2;
        button->x_coords[1] = button->x_coords[0];
        button->y_coords[1] = 800 - top + (h - 3) * delta / 2;
        button->x_coords[2] = right;
        button->y_coords[2] = button->y_coords[1];
        button->x_coords[3] = button->x_coords[2];
        button->y_coords[3] = button->y_coords[0];
    }
}

void map_choose()
{
    map_button = true;
    glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3ub(255, 255, 255);
    //coord_lines();
    drawstring(Width / 2 - 95, Height - 200, "AUTOMOBILE TRAFFIC");
    menu_buttons(NULL, 7);
    menu_buttons(NULL, 4);
    menu_buttons(NULL, 1);
    menu_buttons(NULL, -5);

    glColor3ub(255, 255, 255);
    drawstring(Width / 2 - 40, Height - 305, "Motorway");
    drawstring(Width / 2 - 100, Height - 380, "Road with an intersection");
    drawstring(Width / 2 - 120, Height - 455, "Road with several intersections");
    drawstring(Width / 2 - 20, Height - 605, "Back"); // motorway, road with an intersection, road with several intersections
    menu_activity = false;
    glutSwapBuffers();
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

void draw_mini_buttons(int w, int h, int flag)
{
    if (flag == 0) glColor3ub(138, 43, 226);
    else if (flag == 1) glColor3ub(0, 153, 0);
    glBegin(GL_QUADS);
    glVertex2i(Width / 2 + w, Height - h);
    glVertex2i(Width / 2 + w, Height - h + MINI_BUTTON_HEIGHT);
    glVertex2i(Width / 2 + MINI_BUTTON_WIDTH + w, Height - h + MINI_BUTTON_HEIGHT);
    glVertex2i(Width / 2 + MINI_BUTTON_WIDTH + w, Height - h);
    glEnd();
    if (h == 365)
    {
        glColor3ub(255, 255, 255);
        if (w == 0) drawstring(w + (Width / 2) + (MINI_BUTTON_WIDTH / 2) - 5, Height - h + 5, "3");
        else if (w == 90) drawstring(w + (Width / 2) + (MINI_BUTTON_WIDTH / 2) - 5, Height - h + 5, "5");
        else if (w == 180) drawstring(w + (Width / 2) + (MINI_BUTTON_WIDTH / 2) - 5, Height - h + 5, "7");
    }
    else if (h == 395)
    {
        glColor3ub(255, 255, 255);
        if (w == 0) drawstring(w + (Width / 2) + (MINI_BUTTON_WIDTH / 2) - 5, Height - h + 5, "2");
        else if (w == 90) drawstring(w + (Width / 2) + (MINI_BUTTON_WIDTH / 2) - 5, Height - h + 5, "3");
    }
    else if (h == 425)
    {
        glColor3ub(255, 255, 255);
        if (w == 0) drawstring(w + (Width / 2) + (MINI_BUTTON_WIDTH / 2) - 15, Height - h + 5, "yes");
        else if (w == 90) drawstring(w + (Width / 2) + (MINI_BUTTON_WIDTH / 2) - 10, Height - h + 5, "no");
    }
}

void interval_buttons()
{
    if (settings.interval == 3)
    {
        draw_mini_buttons(0, 365, 1);
        draw_mini_buttons(90, 365, 0);
        draw_mini_buttons(180, 365, 0);
    }
    else if (settings.interval == 5)
    {
        draw_mini_buttons(0, 365, 0);
        draw_mini_buttons(90, 365, 1);
        draw_mini_buttons(180, 365, 0);
    }
    else if (settings.interval == 7)
    {
        draw_mini_buttons(0, 365, 0);
        draw_mini_buttons(90, 365, 0);
        draw_mini_buttons(180, 365, 1);
    }
}

void count_buttons()
{
    if (settings.count_lanes == 2)
    {
        draw_mini_buttons(0, 395, 1);
        draw_mini_buttons(90, 395, 0);
    }
    else if (settings.count_lanes == 3)
    {
        draw_mini_buttons(0, 395, 0);
        draw_mini_buttons(90, 395, 1);
    }
}

void save_buttons()
{
    if (settings.autosave == true)
    {
        draw_mini_buttons(0, 425, 1);
        draw_mini_buttons(90, 425, 0);
    }
    else if (settings.autosave == false)
    {
        draw_mini_buttons(0, 425, 0);
        draw_mini_buttons(90, 425, 1);
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
        menu_button_2 = true;
        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3ub(255, 255, 255);
        //coord_lines();
        drawstring(Width / 2 - 115, Height - 200, "CAR TRAFFIC SIMULATOR");
        drawstring(Width / 2 - 37, Height - 300, "Settings:");
        drawstring(Width / 10 - 34, Height - 360, "Spawn interval of machines (seconds):");
        drawstring(Width / 4 - 6, Height - 390, "Number of car lanes:");
        drawstring(Width / 3 + 14, Height - 420, "Autosave:");
        
        interval_buttons();
        count_buttons();
        save_buttons();
        
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
    else if (button == 1)
    {
        menu_button_1 = true;
        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3ub(255, 255, 255);
        //coord_lines();
        drawstring(Width / 2 - 95, Height - 200, "AUTOMOBILE TRAFFIC");
        drawstring(Width / 2 - 200, Height - 270, "Good afternoon! A Car Traffic Simulator welcomes you.");
        drawstring(Width / 2 - 180, Height - 300, "There are several types of roads available to you:");
        drawstring(Width / 2 - 270, Height - 330, "a motorway, a road with an intersection, a road with several intersections.");
        drawstring(Width / 2 - 200, Height - 360, "You can simulate an accident in one of the traffic lanes.");
        drawstring(Width / 2 - 195, Height - 390, "To do this, you need to select one of the bands using");
        drawstring(Width / 2 - 150, Height - 420, "the arrows on the keyboard (<- and ->).");
        glColor3ub(239, 211, 52);
        drawstring(Width / 2 - 180, Height - 450, "The strip you selected will be highlighted in yellow!");
        glColor3ub(255, 255, 255);
        drawstring(Width / 2 - 240, Height - 480, "To simulate an accident situation, you need to press the space bar.");
        drawstring(Width / 2 - 80, Height - 510, "To continue, click next.");

        glColor3ub(138, 43, 226);
        glBegin(GL_QUADS);
        glVertex2i(left, bottom + (-7) * delta / 2);
        glVertex2i(left, top + (-7) * delta / 2);
        glVertex2i(right - 290, top + (-7) * delta / 2);
        glVertex2i(right - 290, bottom + (-7) * delta / 2);
        glEnd();

        glBegin(GL_QUADS);
        glVertex2i(left + 80, bottom + (-7) * delta / 2);
        glVertex2i(left + 80, top + (-7) * delta / 2);
        glVertex2i(right, top + (-7) * delta / 2);
        glVertex2i(right, bottom + (-7) * delta / 2);
        glEnd();

        glColor3ub(255, 255, 255);
        drawstring(Width / 2 - 165, Height - 580, "Back");
        drawstring(Width / 2 + 15, Height - 580, "Next");
        menu_activity = false;
        glutSwapBuffers();
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
    menu_changes_2 = false;
}

void display()
{
    glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    //glLoadIdentity();
    /* -------------------------- */
    menu();
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
            if (x <= 575 && x >= 225 && y >= 575 && y <= 625) exit(0);
            else if (x <= 575 && x >= 225 && y >= 500 && y <= 550) processing_buttons(3);
            else if (x <= 575 && x >= 225 && y >= 425 && y <= 475) processing_buttons(2);
            else if (x <= 575 && x >= 225 && y >= 350 && y <= 400) processing_buttons(1);
        }
        else if (state == GLUT_DOWN && menu_button_3 == true)
        {
            if (x <= 575 && x >= 225 && y >= 550 && y <= 600)
            {
                menu_button_3 = false;
                glutPostRedisplay();
            }
        }
        else if (state == GLUT_DOWN && menu_button_2 == true)
        {
            if (x <= 575 && x >= 225 && y >= 550 && y <= 600)
            {
                menu_button_2 = false;
                glutPostRedisplay();
            }
            else if (x <= 470 && x >= 400 && y >= 345 && y <= 365)
            {
                settings.interval = 3;
                processing_buttons(2);
            }
            else if (x <= 560 && x >= 490 && y >= 345 && y <= 365)
            {
                settings.interval = 5;
                processing_buttons(2);
            }
            else if (x <= 640 && x >= 580 && y >= 345 && y <= 365)
            {
                settings.interval = 7;
                processing_buttons(2);
            }
            else if (x <= 470 && x >= 400 && y >= 375 && y <= 395)
            {
                settings.count_lanes = 2;
                processing_buttons(2);
            }
            else if (x <= 560 && x >= 490 && y >= 375 && y <= 395)
            {
                settings.count_lanes = 3;
                processing_buttons(2);
            }
            else if (x <= 470 && x >= 400 && y >= 405 && y <= 425)
            {
                settings.autosave = true;
                processing_buttons(2);
            }
            else if (x <= 560 && x >= 490 && y >= 405 && y <= 425)
            {
                settings.autosave = false;
                processing_buttons(2);
            }
        }
        else if (state == GLUT_DOWN && menu_button_1 == true)
        {
            if (x <= 285 && x >= 225 && y >= 550 && y <= 600)
            {
                menu_button_1 = false;
                glutPostRedisplay();
            }
            else if (x <= 575 && x >= 305 && y >= 550 && y <= 600)
            {
                menu_button_1 = false;
                map_choose();
            }
        }
        else if (state == GLUT_DOWN && map_button == true)
        {
            if (x <= 575 && x >= 225 && y >= 575 && y <= 625)
            {
                map_button = false;
                glutPostRedisplay();
            }
            else if (x <= 575 && x >= 225 && y >= 275 && y <= 325)
            {
                map_button = false;
                model_active = true;
                motorway();
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
    if (key == 27 && model_active == true)
    {
        model_active = false;
        printf("escapeescapeescapeescapeescape\n");
        pause();
    }
    else if (key == 27 && model_active == false)
    {
        model_active = true;
        printf("escapeescapeescapeescapeescape\n");
        motorway();
    }
}

void struct_init()
{
    settings.interval = 5;
    settings.autosave = false;
    settings.count_lanes = 3;
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_NAME);
    struct_init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    //glutTimerFunc(0, timer, 0);

    glutPassiveMotionFunc(mouse_move);
    glutMouseFunc(mouse_pressed);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
}
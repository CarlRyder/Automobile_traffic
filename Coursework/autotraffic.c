/* Copyright (c) Yakov D. Sendov & Salavat S. Babaev, 2022 */

#define _CRT_SECURE_NO_WARNINGS

#include "pincludes.h"
#include "defines.h"

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

struct tex
{
    unsigned int texture[20];
} car_tex, map_tex;

typedef struct
{
    unsigned short num;
    short direction;
    float x[2];
    float y[2];
    short line;
    short texture_id;
    float speed;
} Tcar;

typedef struct Node
{
    Tcar car;
    struct Node* next_car;
} Cars;

struct Map
{
    int line;
    int car_counts;
    int interval;
    int count_lanes;
    bool autosave;
    int model_time;
} Map;

/* Definition of functions */
GLuint get_map_texture(char* filename);
void get_car_texture();
void map_show(int value);
void drawstring(float x, float y, char* string);
void points(int first, int second);
void control_lines(int line);
void change_line();
void check_distance(Cars* head);
void car_position(Tcar* car, int first, int second);
void car_draw(Tcar* car);
void car_coords(Tcar* car, int x, int y);
void coord_lines();
void init_car(Tcar* car);
void push_car(Cars* head);
Cars* delete_headcar(Cars* head);
Cars* delete_car(Cars* current, Cars* head);
Cars* create_car();
void motorway();
void crossroad();
void mult_crossroad();
void menu_buttons(struct menu_button* button, int h);
void text_buttons(struct menu_button* button, int flag);
void draw_mini_buttons(int w, int h, int flag);
void settings_buttons();
void processing_buttons(int button);
void menu();
void display();
void reshape(GLint w, GLint h);
void mouse_pressed(int button, int state, int x, int y);
void struct_init();
void error();
/* ----------------------- */

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_NAME);
    struct_init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse_pressed);
    glutMainLoop();
}

void struct_init()
{
    // Texture initialization
    map_tex.texture[0] = get_map_texture(MAP_ONE);
    map_tex.texture[1] = get_map_texture(MAP_TWO);
    map_tex.texture[2] = get_map_texture(MAP_THREE);
    get_car_texture();
    // Main Map struct initialization
    Map.car_counts = 0;
    Map.line = 1;
    Map.interval = 5;
    Map.autosave = false;
    Map.count_lanes = 3;
}

void error()
{
    printf("Error: Out of memory!\n");
    exit(DEFAULT_ERROR);
}

void get_car_texture()
{
    int car_count = 0, first_digit = 16, second_digit = 17;
    char path_pattern[] = "car_texture/car_00.txt";
    while (car_count < TOTAL_CARS)
    {
        car_count++;
        path_pattern[second_digit]++;
        if (car_count == 10)
        {
            path_pattern[first_digit] = '1';
            path_pattern[second_digit] = '0';
        }
        if (car_count == 20)
        {
            path_pattern[first_digit] = '2';
            path_pattern[second_digit] = '0';
        }
        int width = 24, height = 44, num_color = 3;
        unsigned char* data = (unsigned char*)malloc(width * height * num_color);
        if (data != NULL)
        {
            FILE* texture_file = fopen(path_pattern, "rb");
            int i = 0;
            unsigned char part[2] = { 0 };
            while (i < width * height * num_color)
            {
                fgets(part, 2, texture_file);
                data[i] = part[0];
                i++;
            }
            fclose(texture_file);
            glGenTextures(1, &car_tex.texture[car_count - 1]);
            glBindTexture(GL_TEXTURE_2D, car_tex.texture[car_count - 1]);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
            free(data);
        }
    }
}

GLuint get_map_texture(char* filename)
{
    GLuint texture = 0;
    int width = 800, height = 800, num_color = 3;
    unsigned char* data = (unsigned char*)malloc(width * height * num_color * sizeof(unsigned char));
    if (data != NULL)
    {
        FILE* texture_file = fopen(filename, "rb");
        int i = 0;
        unsigned char part[2] = { 0 };
        while (i < width * height * num_color)
        {
            fgets(part, 2, texture_file);
            data[i] = part[0];
            i++;
        }
        fclose(texture_file);
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
        free(data);
        return texture;
    }
    else
    {
        error();
        return DEFAULT_ERROR;
    }
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
}

void drawstring(float x, float y, char* string)
{
    glRasterPos2i(x, y);
    for (char* c = string; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
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

void pause_background()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glColor4f(0.31, 0.31, 0.31, 0.7);
    glBegin(GL_QUADS);
    points(0, 800);
    glEnd();
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

void pause()
{
    while (1)
    {
        map_show(0);
        pause_background();
        glColor3ub(255, 255, 255);
        drawstring(Width / 2 - 24, Height - 250, "PAUSE");
        glutSwapBuffers();
        if (GetAsyncKeyState(VK_ESCAPE) & 0x1) break;
    }
}

void points(int first, int second)
{
    glVertex2i(first, 0); 
    glVertex2i(first, 800);
    glVertex2i(second, 800);
    glVertex2i(second, 0);
}

void control_lines(int line)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glColor4f(0.93, 0.86, 0.5, 0.3);
    glBegin(GL_QUADS);
    if (line == 1) points(275, 316);
    else if (line == 2) points(321, 359);
    else if (line == 3) points(365, 403);
    else if (line == 4) points(409, 450);
    else if (line == 5) points(456, 497);
    else if (line == 6) points(499, 541);
    glEnd();
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

void change_line()
{
    while (1)
    {
        map_show(0);
        control_lines(Map.line);
        glColor3ub(255, 255, 255);
        drawstring(Width / 2 - 340, Height - 760, "To change the selected lane for an accident, use the arrows on the keyboard (<- ->)");
        glutSwapBuffers();
        if (GetAsyncKeyState(VK_LEFT) & 0x1 && Map.line > 1) Map.line--;
        if (GetAsyncKeyState(VK_RIGHT) & 0x1 && Map.line < 6) Map.line++;
        if (GetAsyncKeyState(VK_SPACE) & 0x1) break;
    }
}

void check_distance(Cars* head)
{
    Cars* tmp = head;
    Cars** car_dir_1 = (Cars**)malloc(Map.car_counts * 3 * sizeof(Cars*));
    Cars** car_dir_2 = (Cars**)malloc(Map.car_counts * 3 * sizeof(Cars*));
    unsigned short range_dir1[3] = { 0, Map.car_counts, Map.car_counts * 2 };
    unsigned short range_dir2[3] = { 0, Map.car_counts, Map.car_counts * 2 };
    while (tmp->next_car != NULL)
    {
        if (tmp->car.direction == 1)
        {
            if (tmp->car.line == 1)
            {
                car_dir_1[range_dir1[0]] = tmp;
                range_dir1[0]++;
            }
            else if (tmp->car.line == 2)
            {
                car_dir_1[range_dir1[1]] = tmp;
                range_dir1[1]++;
            }
            else if (tmp->car.line == 3)
            {
                car_dir_1[range_dir1[2]] = tmp;
                range_dir1[2]++;
            }
        }
        if (tmp->car.direction == 2)
        {
            if (tmp->car.line == 1)
            {
                car_dir_2[range_dir2[0]] = tmp;
                range_dir2[0]++;
            }
            else if (tmp->car.line == 2)
            {
                car_dir_2[range_dir2[1]] = tmp;
                range_dir2[1]++;
            }
            else if (tmp->car.line == 3)
            {
                car_dir_2[range_dir2[2]] = tmp;
                range_dir2[2]++;
            }
        }
        tmp = tmp->next_car;
    }
    int num_ccar = 0,
    num_dcar = 0,
    count_line = 0,
    end_arr = 0,
    start_num_arr = 0,
    count_dir = 1;
    while (count_dir < 3)
    {
        count_line = 0;
        while (count_line < 4)
        {
            if (count_line == 0)
            {
                start_num_arr = 0;
                if (count_dir == 1) end_arr = range_dir1[0];
                else end_arr = range_dir2[0];
            }
            else if (count_line == 1)
            {
                start_num_arr = Map.car_counts;
                if (count_dir == 1) end_arr = range_dir1[1];
                else end_arr = range_dir2[1];
            }
            else if (count_line == 2)
            {
                start_num_arr = Map.car_counts * 2;
                if (count_dir == 1)  end_arr = range_dir1[2];
                else end_arr = range_dir2[2];
            }

            for (num_ccar = start_num_arr; num_ccar < end_arr; num_ccar++)
            {
                for (num_dcar = start_num_arr; num_dcar < end_arr; num_dcar++)
                {
                    if (count_dir == 1 && car_dir_1[num_dcar] != car_dir_1[num_ccar])
                    {
                        if ((car_dir_1[num_dcar]->car.y[0] - car_dir_1[num_ccar]->car.y[0] > 0)
                            && (car_dir_1[num_dcar]->car.y[0] - car_dir_1[num_ccar]->car.y[0] <= MIN_DISTANCE))
                        {
                            car_dir_1[num_ccar]->car.speed = car_dir_1[num_dcar]->car.speed;
                        }
                    }
                    else if (count_dir == 2 && car_dir_2[num_dcar] != car_dir_2[num_ccar])
                    {
                        if ((car_dir_2[num_dcar]->car.y[0] - car_dir_2[num_ccar]->car.y[0] < 0)
                            && (abs(car_dir_2[num_dcar]->car.y[0] - car_dir_2[num_ccar]->car.y[0]) <= MIN_DISTANCE))
                        {
                            car_dir_2[num_ccar]->car.speed = car_dir_2[num_dcar]->car.speed;
                        }
                    }
                }
            }
            count_line++;
        }
        count_dir++;
    }
    free(car_dir_1);
    free(car_dir_2);
    tmp = NULL;
}

void car_position(Tcar* car, int first, int second)
{
    glTexCoord2f(first, second); glVertex2f(car->x[0], car->y[0] + CAR_HEIGHT); //LV
    glTexCoord2f(second, second); glVertex2f(car->x[0] + CAR_WIDTH, car->y[0] + CAR_HEIGHT); //RV
    glTexCoord2f(second, first); glVertex2f(car->x[0] + CAR_WIDTH, car->y[0]); //RN
    glTexCoord2f(first, first); glVertex2f(car->x[0], car->y[0]); //LN
}

void car_draw(Tcar* car)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, car_tex.texture[car->texture_id]);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    if (car->direction == 1) car_position(car, 0, 1);
    else if (car->direction == 2) car_position(car, 1, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void car_coords(Tcar* car, int x, int y)
{
    car->x[0] = x;
    car->x[1] = x + CAR_WIDTH;
    car->y[0] = y;
    car->y[1] = y + CAR_HEIGHT;
}

void init_car(Tcar* car)
{
    car->num = Map.car_counts + 1;
    Map.car_counts++;
    car->direction = rand() % 2 + 1;
    car->line = rand() % 3 + 1;
    car->speed = rand() % 90 + 40;
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
    car->texture_id = rand() % 19;
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
        error();
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

void print_cars(Cars* head)
{
    Cars* tmp = head;
    while (tmp != NULL)
    {
        printf("%d ", tmp->car.num);
        tmp = tmp->next_car;
    }
    printf("\n");
}

void motorway()
{
    Cars* head = NULL;
    head = create_car();
    if (model_active == true)
    {
        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        map_show(0);
        glutSwapBuffers();
        clock_t start = clock(), end = 0;
        while (true)
        {
            if (GetAsyncKeyState(VK_SPACE) & 0x1) change_line();
            if (GetAsyncKeyState(VK_ESCAPE) & 0x1) pause();
            if ((end - start) / CLOCKS_PER_SEC > 0.5)
            {
                push_car(head);
                print_cars(head);
                start = clock();
                end = 0;
            }
            check_distance(head);
            map_show(0);
            Cars* tmp = head, * ptr = NULL;
            while (tmp != NULL)
            {
                car_draw(&tmp->car);
                if (tmp->car.direction == 1)
                {
                    if (tmp->car.y[0] < 844) tmp->car.y[0] += tmp->car.speed / 3000;
                    else
                    {
                        if (tmp == head) head = delete_headcar(head);
                        else ptr->next_car = delete_car(tmp, head);
                        Map.car_counts--;
                        break;
                    }
                }
                else if (tmp->car.direction == 2)
                {
                    if (tmp->car.y[0] > -44) tmp->car.y[0] -= tmp->car.speed / 3000;
                    else
                    {
                        if (tmp == head) head = delete_headcar(head);
                        else ptr->next_car = delete_car(tmp, head);
                        Map.car_counts--;
                        break;
                    }
                }
                ptr = tmp;
                tmp = tmp->next_car;
            }
            glutSwapBuffers();
            end = clock();
        }
        glutIdleFunc(motorway);
    }
}

void crossroad()
{
    if (model_active == true)
    {
        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        map_show(1);
        glutSwapBuffers();
        glutIdleFunc(crossroad);
    }
}

void mult_crossroad()
{
    if (model_active == true)
    {
        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        map_show(2);
        glutSwapBuffers();
        glutIdleFunc(mult_crossroad);
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
    drawstring(Width / 2 - 95, Height - 200, "AUTOMOBILE TRAFFIC");
    menu_buttons(NULL, 7);
    menu_buttons(NULL, 4);
    menu_buttons(NULL, 1);
    menu_buttons(NULL, -5);

    glColor3ub(255, 255, 255);
    drawstring(Width / 2 - 40, Height - 305, "Motorway");
    drawstring(Width / 2 - 100, Height - 380, "Road with an intersection");
    drawstring(Width / 2 - 120, Height - 455, "Road with several intersections");
    drawstring(Width / 2 - 20, Height - 605, "Back");
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

void settings_buttons()
{
    // Interval buttons
    if (Map.interval == 3)
    {
        draw_mini_buttons(0, 365, 1);
        draw_mini_buttons(90, 365, 0);
        draw_mini_buttons(180, 365, 0);
    }
    else if (Map.interval == 5)
    {
        draw_mini_buttons(0, 365, 0);
        draw_mini_buttons(90, 365, 1);
        draw_mini_buttons(180, 365, 0);
    }
    else if (Map.interval == 7)
    {
        draw_mini_buttons(0, 365, 0);
        draw_mini_buttons(90, 365, 0);
        draw_mini_buttons(180, 365, 1);
    }
    // Count of lanes buttons
    if (Map.count_lanes == 2)
    {
        draw_mini_buttons(0, 395, 1);
        draw_mini_buttons(90, 395, 0);
    }
    else if (Map.count_lanes == 3)
    {
        draw_mini_buttons(0, 395, 0);
        draw_mini_buttons(90, 395, 1);
    }
    // Autosave buttons
    if (Map.autosave == true)
    {
        draw_mini_buttons(0, 425, 1);
        draw_mini_buttons(90, 425, 0);
    }
    else if (Map.autosave == false)
    {
        draw_mini_buttons(0, 425, 0);
        draw_mini_buttons(90, 425, 1);
    }
}

void processing_buttons(int button)
{
    int delta = BUTTON_HEIGHT, midX = Width / 2, midY = Height / 2;
    int left = midX - BUTTON_WIDTH / 2, right = left + BUTTON_WIDTH,
    bottom = midY - BUTTON_HEIGHT / 2, top = bottom + BUTTON_HEIGHT;
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
        drawstring(Width / 2 - 115, Height - 200, "CAR TRAFFIC SIMULATOR");
        drawstring(Width / 2 - 37, Height - 300, "Settings:");
        drawstring(Width / 10 - 34, Height - 360, "Spawn interval of machines (seconds):");
        drawstring(Width / 4 - 6, Height - 390, "Number of car lanes:");
        drawstring(Width / 3 + 14, Height - 420, "Autosave:");
        
        settings_buttons();
        
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
    srand(time(NULL));
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
    menu();
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
    glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void mouse_pressed(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        printf("%d | %d\n", x, y);
        if (menu_activity == true)
        {
            if (x <= 575 && x >= 225 && y >= 575 && y <= 625) exit(0);
            else if (x <= 575 && x >= 225 && y >= 500 && y <= 550) processing_buttons(3);
            else if (x <= 575 && x >= 225 && y >= 425 && y <= 475) processing_buttons(2);
            else if (x <= 575 && x >= 225 && y >= 350 && y <= 400) processing_buttons(1);
        }
        else if (menu_button_3 == true)
        {
            if (x <= 575 && x >= 225 && y >= 550 && y <= 600)
            {
                menu_button_3 = false;
                glutPostRedisplay();
            }
        }
        else if (menu_button_2 == true)
        {
            if (x <= 575 && x >= 225 && y >= 550 && y <= 600)
            {
                menu_button_2 = false;
                glutPostRedisplay();
            }
            else if (x <= 470 && x >= 400 && y >= 345 && y <= 365)
            {
                Map.interval = 3;
                processing_buttons(2);
            }
            else if (x <= 560 && x >= 490 && y >= 345 && y <= 365)
            {
                Map.interval = 5;
                processing_buttons(2);
            }
            else if (x <= 640 && x >= 580 && y >= 345 && y <= 365)
            {
                Map.interval = 7;
                processing_buttons(2);
            }
            else if (x <= 470 && x >= 400 && y >= 375 && y <= 395)
            {
                Map.count_lanes = 2;
                processing_buttons(2);
            }
            else if (x <= 560 && x >= 490 && y >= 375 && y <= 395)
            {
                Map.count_lanes = 3;
                processing_buttons(2);
            }
            else if (x <= 470 && x >= 400 && y >= 405 && y <= 425)
            {
                Map.autosave = true;
                processing_buttons(2);
            }
            else if (x <= 560 && x >= 490 && y >= 405 && y <= 425)
            {
                Map.autosave = false;
                processing_buttons(2);
            }
        }
        else if (menu_button_1 == true)
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
        else if (map_button == true)
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
            else if (x <= 575 && x >= 225 && y >= 350 && y <= 400)
            {
                map_button = false;
                model_active = true;
                crossroad();
            }
            else if (x <= 575 && x >= 225 && y >= 425 && y <= 475)
            {
                map_button = false;
                model_active = true;
                mult_crossroad();
            }
        }
    }
}
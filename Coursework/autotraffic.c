/* Copyright (c) Yakov D. Sendov & Salavat S. Babaev, 2022 */

#define _CRT_SECURE_NO_WARNINGS

#include "pincludes.h"
#include "defines.h"

GLint Width = 800, Height = 800;

bool menu_activity = false, menu_button_1 = false, menu_button_2 = false, menu_button_3 = false;
bool menu_changes_2 = false, map_button = false;
bool model_active = false, map_1 = false, map_2 = false, map_3 = false;
bool pause_active = false;
bool load_error = false;
bool add_car = false;
bool traffic_light1 = true, traffic_light2 = false;

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
    bool turn;
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
    int time;
    float interval;
    int count_lanes;
    bool autosave;
    int model_time;
} Map;

/* Definition of functions */
GLuint get_map_texture(char* filename);
void get_car_texture();
void map_show(int value);
void drawstring(float x, float y, char* string);
void points(int x1, int x2, int y1, int y2);
void control_lines(int line);
void change_line(int map, Cars* head);
void pause(int map, Cars* head, bool flag);
void check_distance(Cars* head);
void car_position(Tcar* car, int first, int second, bool flag);
void car_draw(Tcar* car);
void car_coords(Tcar* car, int x, int y);
void car_static_show(Cars* head);
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
void display(void);
void reshape(GLint w, GLint h);
void mouse_pressed(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);
void special_keyboard(int key, int x, int y);
void struct_init();
void error();
/* ----------------------- */

int main()
{
    int argc = 1;
    char* argv[1] = { (char*)"1" };
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_NAME);
    struct_init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse_pressed);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special_keyboard);
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
    Map.interval = 1;
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
    control_lines(Map.line);
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
    points(0, 800, 0, 800);
    glEnd();
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

void model_save(int map, Cars* head)
{
    FILE* saves = fopen("map_save.txt", "w");
    fprintf(saves, "Map settings:\n");
    fprintf(saves, "%d\n%d\n%d\n%f\n%d\n%d\n", map, Map.line, Map.car_counts, Map.interval, Map.autosave, Map.model_time);
    Cars* temp = head;
    fprintf(saves, "Cars stats:\n");
    while (temp != NULL)
    {
        fprintf(saves, "%d %d %f %f %d %d %d %f\n", temp->car.num, temp->car.direction, temp->car.x[0], temp->car.y[0], temp->car.line, temp->car.turn, temp->car.texture_id, temp->car.speed);
        temp = temp->next_car;
    }
    fclose(saves);
    pause(map, head, true);
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
    glColor3ub(255, 255, 255);
    drawstring(Width / 2 - 24, Height - 250, "PAUSE");
    drawstring(Width / 2 - 140, Height - 350, "To select, press the appropriate key:");
    drawstring(Width / 2 - 131, Height - 380, "F1 - exit to the menu without saving");
    drawstring(Width / 2 - 133, Height - 410, "F2 - save the current model to a file");
    drawstring(Width / 2 - 129, Height - 440, "F3 - add a car to the selected lane");
    if (flag == true) drawstring(Width / 2 - 190, Height - 50, "The model has been successfully saved to a file!");
    if (add_car == true)
    {
        drawstring(Width / 2 - 210, Height - 50, "The car was successfully created on the lane you selected!");
        add_car = false;
    }
    glutSwapBuffers();
}

void points(int x1, int x2, int y1, int y2)
{
    glVertex2i(x1, y1); 
    glVertex2i(x1, y2);
    glVertex2i(x2, y2);
    glVertex2i(x2, y1);
}

int count_car(Cars* head)
{
    Cars* temp = head;
    int counter = 0, car_line = 0;
    while (temp != NULL)
    {
        if (temp->car.direction == 2) car_line = temp->car.line;
        else if (temp->car.direction == 1) car_line = temp->car.line + 3;
        else if (temp->car.direction == 4) car_line = temp->car.line + 6;
        else if (temp->car.direction == 3) car_line = temp->car.line + 9;
        if (car_line == Map.line) counter++;
        temp = temp->next_car;
    }
    return counter;
}

void info(int max, Cars* head)
{
    glColor3ub(239, 211, 52);
    drawstring(5, Height - 20, "SERVICE INFORMATION");
    glColor3ub(255, 255, 255);

    drawstring(5, Height - 40, "Current number of cars:");
    char num[5] = { 0 };
    _itoa(Map.car_counts, num, 10);
    drawstring(204, Height - 40, num);

    drawstring(5, Height - 60, "in the selected lane:");
    int car_line = count_car(head);
    char number[5] = { 0 };
    _itoa(car_line, number, 10);
    drawstring(174, Height - 60, number);

    drawstring(5, Height - 80, "Bandwidth:");
    char value[5] = { 0 };
    _itoa(max, value, 10);
    drawstring(100, Height - 80, value);

    if (map_3 == true) drawstring(5, Height - 720, "Simulation time (sec):");
    else drawstring(5, Height - 100, "Simulation time (sec):");
    char model_time[10] = { 0 };
    _itoa(Map.time, model_time, 10);
    if (map_3 == true) drawstring(184, Height - 720, model_time);
    else drawstring(184, Height - 100, model_time);

    if (map_3 == true) drawstring(5, Height - 740, "Interval time (sec):");
    else drawstring(5, Height - 120, "Interval time (sec):");
    if (map_3 == true)
    {
        if (Map.interval == 0.75) drawstring(160, Height - 740, "0.75");
        else if (Map.interval == 1) drawstring(160, Height - 740, "1");
        else if (Map.interval == 1.25) drawstring(160, Height - 740, "1.25");
    }
    else
    {
        if (Map.interval == 0.75) drawstring(160, Height - 120, "0.75");
        else if (Map.interval == 1) drawstring(160, Height - 120, "1");
        else if (Map.interval == 1.25) drawstring(160, Height - 120, "1.25");
    }

    if (map_3 == true) drawstring(5, Height - 760, "Autosave:");
    else drawstring(5, Height - 140, "Autosave:");
    if (map_3 == true)
    {
        if (Map.autosave == true) drawstring(90, Height - 760, "On");
        else drawstring(90, Height - 760, "Off");
    }
    else
    {
        if (Map.autosave == true) drawstring(90, Height - 140, "On");
        else drawstring(90, Height - 140, "Off");
    }
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
    else if (line == 7) points(0, 800, 303, 267);
    else if (line == 8) points(0, 800, 349, 306);
    else if (line == 9) points(0, 800, 394, 352);
    else if (line == 10) points(0, 800, 440, 399);
    else if (line == 11) points(0, 800, 483, 444);
    else if (line == 12) points(0, 800, 532, 487);
    glEnd();
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

void change_line(int map, Cars* head)
{
    while (1)
    {
        map_show(map);
        if (head != NULL) car_static_show(head);
        control_lines(Map.line);
        glColor3ub(255, 255, 255);
        drawstring(Width / 2 - 340, Height - 760, "To change the selected lane for an accident, use the arrows on the keyboard (<- ->)");
        glutSwapBuffers();
        if (GetAsyncKeyState(VK_LEFT) & 0x1 && Map.line > 1) Map.line--;
        if (map_1 == true)
        {
            if (GetAsyncKeyState(VK_RIGHT) & 0x1 && Map.line < 6) Map.line++;
        }
        else if (map_2 == true)
        {
            if (GetAsyncKeyState(VK_RIGHT) & 0x1 && Map.line < 12) Map.line++;
        }
        else if (map_3 == true)
        {
            if (GetAsyncKeyState(VK_RIGHT) & 0x1 && Map.line < 6) Map.line++;
        }
        if (GetAsyncKeyState(VK_SPACE) & 0x1) break;
    }
}

void check_distance(Cars* head)
{
    Cars* tmp = head;

    Cars** car_dir_1 = (Cars**)malloc(Map.car_counts * 3 * sizeof(Cars*));
    Cars** car_dir_2 = (Cars**)malloc(Map.car_counts * 3 * sizeof(Cars*));
    Cars** car_dir_3 = (Cars**)malloc(Map.car_counts * 3 * sizeof(Cars*));
    Cars** car_dir_4 = (Cars**)malloc(Map.car_counts * 3 * sizeof(Cars*));
    Cars** car_dir_5 = (Cars**)malloc(Map.car_counts * 3 * sizeof(Cars*));
    Cars** car_dir_6 = (Cars**)malloc(Map.car_counts * 3 * sizeof(Cars*));

    unsigned short range_dir1[3] = { 0, Map.car_counts, Map.car_counts * 2 };
    unsigned short range_dir2[3] = { 0, Map.car_counts, Map.car_counts * 2 };
    unsigned short range_dir3[3] = { 0, Map.car_counts, Map.car_counts * 2 };
    unsigned short range_dir4[3] = { 0, Map.car_counts, Map.car_counts * 2 };
    unsigned short range_dir5[3] = { 0, Map.car_counts, Map.car_counts * 2 };
    unsigned short range_dir6[3] = { 0, Map.car_counts, Map.car_counts * 2 };

    while (tmp->next_car != NULL)
    {
        if (tmp == NULL) break;
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
        else if (tmp->car.direction == 2)
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
        else if (tmp->car.direction == 3)
        {
            if (tmp->car.line == 1)
            {
                car_dir_3[range_dir3[0]] = tmp;
                range_dir3[0]++;
            }
            else if (tmp->car.line == 2)
            {
                car_dir_3[range_dir3[1]] = tmp;
                range_dir3[1]++;
            }
            else if (tmp->car.line == 3)
            {
                car_dir_3[range_dir3[2]] = tmp;
                range_dir3[2]++;
            }
        }
        else if (tmp->car.direction == 4)
        {
            if (tmp->car.line == 1)
            {
                car_dir_4[range_dir4[0]] = tmp;
                range_dir4[0]++;
            }
            else if (tmp->car.line == 2)
            {
                car_dir_4[range_dir4[1]] = tmp;
                range_dir4[1]++;
            }
            else if (tmp->car.line == 3)
            {
                car_dir_4[range_dir4[2]] = tmp;
                range_dir4[2]++;
            }
        }
        else if (tmp->car.direction == 5)
        {
            if (tmp->car.line == 1)
            {
                car_dir_5[range_dir5[0]] = tmp;
                range_dir5[0]++;
            }
            else if (tmp->car.line == 2)
            {
                car_dir_5[range_dir5[1]] = tmp;
                range_dir5[1]++;
            }
            else if (tmp->car.line == 3)
            {
                car_dir_5[range_dir5[2]] = tmp;
                range_dir5[2]++;
            }
        }
        else if (tmp->car.direction == 6)
        {
            if (tmp->car.line == 1)
            {
                car_dir_6[range_dir6[0]] = tmp;
                range_dir6[0]++;
            }
            else if (tmp->car.line == 2)
            {
                car_dir_6[range_dir6[1]] = tmp;
                range_dir6[1]++;
            }
            else if (tmp->car.line == 3)
            {
                car_dir_6[range_dir6[2]] = tmp;
                range_dir6[2]++;
            }
        }
        tmp = tmp->next_car;
    }

    int num_ccar = 0, num_dcar = 0, count_line = 1, end_arr = 0, start_num_arr = 0, count_dir = 1;
    while (count_dir < 7)
    {
        count_line = 1;
        while (count_line < 4)
        {
            if (count_line == 1)
            {
                start_num_arr = 0;
                if (count_dir == 1) end_arr = range_dir1[0];
                else if (count_dir == 2) end_arr = range_dir2[0];
                else if (count_dir == 3) end_arr = range_dir3[0];
                else if (count_dir == 4) end_arr = range_dir4[0];
                else if (count_dir == 5) end_arr = range_dir5[0];
                else if (count_dir == 6) end_arr = range_dir6[0];
            }
            else if (count_line == 2)
            {
                start_num_arr = Map.car_counts;
                if (count_dir == 1) end_arr = range_dir1[1];
                else if (count_dir == 2) end_arr = range_dir2[1];
                else if (count_dir == 3) end_arr = range_dir3[1];
                else if (count_dir == 4) end_arr = range_dir4[1];
                else if (count_dir == 5) end_arr = range_dir5[1];
                else if (count_dir == 6) end_arr = range_dir6[1];
            }
            else if (count_line == 3)
            {
                start_num_arr = Map.car_counts * 2;
                if (count_dir == 1) end_arr = range_dir1[2];
                else if (count_dir == 2) end_arr = range_dir2[2];
                else if (count_dir == 3) end_arr = range_dir3[2];
                else if (count_dir == 4) end_arr = range_dir4[2];
                else if (count_dir == 5) end_arr = range_dir5[2];
                else if (count_dir == 6) end_arr = range_dir6[2];
            }

            for (num_ccar = start_num_arr; num_ccar < end_arr; num_ccar++)
            {
                for (num_dcar = start_num_arr; num_dcar < end_arr; num_dcar++)
                {
                    if (count_dir == 1 && car_dir_1[num_dcar] != car_dir_1[num_ccar])
                    {
                        float y_dcar = car_dir_1[num_dcar]->car.y[0];
                        float y_ccar = car_dir_1[num_ccar]->car.y[0];
                        if (y_dcar - y_ccar > 0 && y_dcar - y_ccar <= MIN_DISTANCE)
                        {
                            if (y_dcar - y_ccar < 44.0)
                            {
                                car_dir_1[num_ccar]->car.y[0] -= 21.0;
                            }
                            car_dir_1[num_ccar]->car.speed = car_dir_1[num_dcar]->car.speed;
                        }
                    }
                    else if (count_dir == 2 && car_dir_2[num_dcar] != car_dir_2[num_ccar])
                    {
                        float y_dcar = car_dir_2[num_dcar]->car.y[0];
                        float y_ccar = car_dir_2[num_ccar]->car.y[0];
                        if (y_dcar - y_ccar < 0 && fabsf(y_dcar - y_ccar) <= MIN_DISTANCE)
                        {
                            if (y_dcar - y_ccar > -44.0)
                            {
                                car_dir_2[num_ccar]->car.y[0] += 21.0;
                            }
                            car_dir_2[num_ccar]->car.speed = car_dir_2[num_dcar]->car.speed;
                        }
                    }
                    else if (count_dir == 3 && car_dir_3[num_dcar] != car_dir_3[num_ccar])
                    {
                        float y_dcar = car_dir_3[num_dcar]->car.y[0];
                        float y_ccar = car_dir_3[num_ccar]->car.y[0];
                        if (y_dcar - y_ccar < 0 && fabsf(y_dcar - y_ccar) <= MIN_DISTANCE)
                        {
                            if (y_dcar - y_ccar > -44.0)
                            {
                                car_dir_3[num_ccar]->car.x[0] += 21.0;
                            }
                            car_dir_3[num_ccar]->car.speed = car_dir_3[num_dcar]->car.speed;
                        }
                    }
                    else if (count_dir == 4 && car_dir_4[num_dcar] != car_dir_4[num_ccar])
                    {
                        float y_dcar = car_dir_4[num_dcar]->car.y[0];
                        float y_ccar = car_dir_4[num_ccar]->car.y[0];
                        if (y_dcar - y_ccar > 0 && y_dcar - y_ccar <= MIN_DISTANCE)
                        {
                            if (y_dcar - y_ccar < 44.0)
                            {
                                car_dir_4[num_ccar]->car.x[0] -= 21.0;
                            }
                            car_dir_4[num_ccar]->car.speed = car_dir_4[num_dcar]->car.speed;
                        }
                    }
                    else if (count_dir == 5 && car_dir_5[num_dcar] != car_dir_5[num_ccar])
                    {
                        float y_dcar = car_dir_5[num_dcar]->car.y[0];
                        float y_ccar = car_dir_5[num_ccar]->car.y[0];
                        if (y_dcar - y_ccar < 0 && fabsf(y_dcar - y_ccar) <= MIN_DISTANCE)
                        {
                            if (y_dcar - y_ccar > -44.0)
                            {
                                car_dir_5[num_ccar]->car.x[0] += 21.0;
                            }
                            car_dir_5[num_ccar]->car.speed = car_dir_5[num_dcar]->car.speed;
                        }
                    }
                    else if (count_dir == 6 && car_dir_6[num_dcar] != car_dir_6[num_ccar])
                    {
                        float y_dcar = car_dir_6[num_dcar]->car.y[0];
                        float y_ccar = car_dir_6[num_ccar]->car.y[0];
                        if (y_dcar - y_ccar > 0 && y_dcar - y_ccar <= MIN_DISTANCE)
                        {
                            if (y_dcar - y_ccar < 44.0)
                            {
                                car_dir_6[num_ccar]->car.x[0] -= 21.0;
                            }
                            car_dir_6[num_ccar]->car.speed = car_dir_4[num_dcar]->car.speed;
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
    free(car_dir_3);
    free(car_dir_4);
    free(car_dir_5);
    free(car_dir_6);
    tmp = NULL;
}

void car_position(Tcar* car, int first, int second, bool flag)
{
    if (flag == true)
    {
        glTexCoord2f(first, second); glVertex2f(car->x[0] + CAR_WIDTH / 2, car->y[0] + CAR_HEIGHT); // LV
        glTexCoord2f(second, second); glVertex2f(car->x[0] + CAR_WIDTH * 1.5, car->y[0] + CAR_HEIGHT); // RV
        glTexCoord2f(second, first); glVertex2f(car->x[0] + CAR_WIDTH / 2, car->y[0]); // RN
        glTexCoord2f(first, first); glVertex2f(car->x[0] - CAR_WIDTH / 2, car->y[0]); // LN
    }
    else
    {
        glTexCoord2f(first, second); glVertex2f(car->x[0], car->y[0] + CAR_HEIGHT); //LV
        glTexCoord2f(second, second); glVertex2f(car->x[0] + CAR_WIDTH, car->y[0] + CAR_HEIGHT); //RV
        glTexCoord2f(second, first); glVertex2f(car->x[0] + CAR_WIDTH, car->y[0]); //RN
        glTexCoord2f(first, first); glVertex2f(car->x[0], car->y[0]); //LN
    }
}

void car_pos(Tcar* car, int first, int second, bool flag)
{
    if (flag == true)
    {
        glTexCoord2f(first, first); glVertex2f(car->x[0] - CAR_HEIGHT, car->y[0]); // LV
        glTexCoord2f(first, second); glVertex2f(car->x[0], car->y[0]); // RV
        glTexCoord2f(second, second); glVertex2f(car->x[0], car->y[0] - CAR_WIDTH); // RN
        glTexCoord2f(second, first); glVertex2f(car->x[0] - CAR_HEIGHT, car->y[0]); // LN
    }
    else
    {
        glTexCoord2f(first, first); glVertex2f(car->x[0] - CAR_HEIGHT, car->y[0]); // LV
        glTexCoord2f(first, second); glVertex2f(car->x[0], car->y[0]); // RV
        glTexCoord2f(second, second); glVertex2f(car->x[0], car->y[0] - CAR_WIDTH); // RN
        glTexCoord2f(second, first); glVertex2f(car->x[0] - CAR_HEIGHT, car->y[0] - CAR_WIDTH); // LN
    }
}

void car_draw(Tcar* car)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, car_tex.texture[car->texture_id]);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    if (map_1 == true)
    {
        if (car->direction == 1) car_position(car, 0, 1, false);
        else if (car->direction == 2) car_position(car, 1, 0, false);
    }
    else if (map_2 == true)
    {
        if (car->direction == 1)
        {
            if (car->line == 3 && car->turn == false)
            {
                if (car->y[0] > 220 && car->y[0] < 250)
                {
                    car_position(car, 0, 1, false);
                    car->x[0] += 0.02;
                }
                else if (car->y[0] > 250)
                {
                    car->direction = 4;
                    car->x[0] = 635;
                    car->y[0] = 296;
                    car->line = 1;
                    car->turn = true;
                }
                else car_position(car, 0, 1, false);
            }
            else car_position(car, 0, 1, false);
        }
        else if (car->direction == 2)
        {
            if (car->line == 1 && car->turn == false)
            {
                if (car->y[0] > 500 && car->y[0] < 540)
                {
                    car_position(car, 1, 0, false);
                    car->x[0] -= 0.02;
                }
                else if (car->y[0] < 500)
                {
                    car->direction = 3;
                    car->x[0] = 235;
                    car->y[0] = 525;
                    car->line = 3;
                    car->turn = true;
                }
                else car_position(car, 1, 0, false);
            }
            else car_position(car, 1, 0, false);
        }
        else if (car->direction == 3)
        {
            if (car->line == 3 && car->turn == false)
            {
                if (car->x[0] > 550 && car->x[0] < 610)
                {
                    car_pos(car, 1, 0, false);
                    car->y[0] += 0.02;
                }
                else if (car->x[0] < 550)
                {
                    car->direction = 1;
                    car->x[0] = 510;
                    car->y[0] = 575;
                    car->line = 3;
                    car->turn = true;
                }
                else car_pos(car, 1, 0, false);
            }
            else car_pos(car, 1, 0, false);
        }
        else if (car->direction == 4)
        {
            if (car->line == 1 && car->turn == false)
            {
                if (car->x[0] > 260 && car->x[0] < 290)
                {
                    car_pos(car, 0, 1, false);
                    car->y[0] -= 0.02;
                }
                else if (car->x[0] > 290)
                {
                    car->direction = 2;
                    car->x[0] = 283;
                    car->y[0] = 225;
                    car->line = 1;
                    car->turn = true;
                }
                else car_pos(car, 0, 1, false);
            }
            else car_pos(car, 0, 1, false);
        }
    }
    else if (map_3 == true)
    {
        if (car->direction == 1) car_position(car, 0, 1, false);
        else if (car->direction == 2) car_position(car, 1, 0, false);
        else if (car->direction == 3) car_pos(car, 1, 0, false);
        else if (car->direction == 4) car_pos(car, 0, 1, false);
        else if (car->direction == 5) car_pos(car, 1, 0, false);
        else if (car->direction == 6) car_pos(car, 0, 1, false);
    }
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
    if (car->direction == 3 || car->direction == 4 || car->direction == 5 || car->direction == 6)
    {
        car->x[1] = x - CAR_HEIGHT;
        car->y[1] = y - CAR_WIDTH;
    }
}

void init_car(Tcar* car)
{
    car->num = Map.car_counts + 1;
    Map.car_counts++;
    car->turn = false;
    car->direction = rand() % 2 + 1;
    if (map_2 == true) car->direction = rand() % 4 + 1;
    if (map_3 == true) car->direction = rand() % 6 + 1;
    car->line = rand() % 3 + 1;
    if (add_car == true)
    {   
        if (map_1 == true)
        {
            if (Map.line < 4) car->direction = 2;
            else car->direction = 1;
            if (car->direction == 2) car->line = Map.line;
            else car->line = Map.line - 3;
        }
        else if (map_2 == true)
        {
            if (Map.line < 4) car->direction = 2;
            else if (Map.line > 3 && Map.line < 7) car->direction = 1;
            else if (Map.line > 6 && Map.line < 10) car->direction = 4;
            else if (Map.line > 9 && Map.line < 13) car->direction = 3;
            if (car->direction == 2) car->line = Map.line;
            else if (car->direction == 1) car->line = Map.line - 3;
            else if (car->direction == 4) car->line = Map.line - 6;
            else if (car->direction == 3) car->line = Map.line - 9;
        }
    }
    car->speed = rand() % 50;
    car->speed += 80;
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
    else if (car->direction == 3)
    {
        if (map_2 == true)
        {
            if (car->line == 1) car_coords(car, 844, 435);
            else if (car->line == 2) car_coords(car, 844, 475);
            else if (car->line == 3) car_coords(car, 844, 525);
        }
        else if (map_3 == true)
        {
            car->line = rand() % 2 + 1;
            if (car->line == 1) car_coords(car, 844, 215);
            else if (car->line == 2) car_coords(car, 844, 255);
        }
    }
    else if (car->direction == 4)
    {
        if (map_2 == true)
        {
            if (car->line == 1) car_coords(car, -44, 296);
            else if (car->line == 2) car_coords(car, -44, 341);
            else if (car->line == 3) car_coords(car, -44, 386);
        }
        else if (map_3 == true)
        {
            car->line = rand() % 2 + 1;
            if (car->line == 1) car_coords(car, -44, 135);
            else if (car->line == 2) car_coords(car, -44, 175);
        }
    }
    else if (car->direction == 5)
    {
        if (car->line == 1) car_coords(car, 844, 605);
        else if (car->line == 2) car_coords(car, 844, 640);
        else if (car->line == 3) car_coords(car, 844, 685);
    }
    else if (car->direction == 6)
    {
        if (car->line == 1) car_coords(car, -44, 485);
        else if (car->line == 2) car_coords(car, -44, 525);
        else if (car->line == 3) car_coords(car, -44, 565);
    }
    car->texture_id = rand() % 19;
}

Cars* remove_all(Cars* head)
{
    while (head != NULL)
    {
        Cars* ptr = head;
        head = head->next_car;
        free(ptr);
    }
    return NULL;
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

Cars* head = NULL;

void model_load()
{
    FILE* saves = fopen("map_save.txt", "r");
    if (saves == NULL)
    {
        load_error = true;
        glutPostRedisplay();
    }
    else
    {
        char str[100] = { 0 };
        int counter = 0, map = -1;
        head = create_car();
        Cars* temp = head;
        while (fgets(str, sizeof(str), saves) != NULL)
        {
            counter++;
            str[strcspn(str, "\n")] = 0;
            if (strcmp(str, "Map settings:") == 0) continue;
            if (counter == 2) map = atoi(str);
            else if (counter == 3) Map.line = atoi(str);
            else if (counter == 4) Map.car_counts = atoi(str);
            else if (counter == 5) Map.interval = (float)atof(str);
            else if (counter == 6) Map.autosave = atoi(str);
            else if (counter == 7) Map.model_time = atoi(str);
            if (strcmp(str, "Cars stats:") == 0) continue;
            if (counter > 8)
            {
                int save_i = 0, count = 0;
                temp->car.num = str[0] - '0';
                temp->car.direction = str[2] - '0';
                char value[11] = { 0 };
                for (unsigned int i = 4; i < strlen(str); i++)
                {
                    save_i = i;
                    if (str[i] == ' ') break;
                    if (count < 10) value[count] = str[i];
                    count++;
                }
                value[strlen(value)] = '\0';
                temp->car.x[0] = (float)atof(value);
                temp->car.x[1] = temp->car.x[0] + CAR_WIDTH;
                memset(value, 0, sizeof(value));
                count = 0;
                for (unsigned int i = save_i + 1; i < strlen(str); i++)
                {
                    save_i = i;
                    if (str[i] == ' ') break;
                    if (count < 10) value[count] = str[i];
                    count++;
                }
                value[strlen(value)] = '\0';
                temp->car.y[0] = (float)atof(value);
                temp->car.y[1] = temp->car.y[0] + CAR_HEIGHT;
                memset(value, 0, sizeof(value));
                temp->car.line = str[save_i + 1] - '0';
                save_i += 3;
                temp->car.turn = str[save_i] - '0';
                count = 0;
                for (unsigned int i = save_i + 2; i < strlen(str); i++)
                {
                    save_i = i;
                    if (str[i] == ' ') break;
                    if (count < 10) value[count] = str[i];
                    count++;
                }
                value[strlen(value)] = '\0';
                temp->car.texture_id = atoi(value);
                memset(value, 0, sizeof(value));
                count = 0;
                for (unsigned int i = save_i + 1; i < strlen(str); i++)
                {
                    if (count < 10) value[count] = str[i];
                    count++;
                }
                value[strlen(value)] = '\0';
                temp->car.speed = (float)atof(value);
                memset(value, 0, sizeof(value));
                temp->next_car = create_car();
                temp = temp->next_car;
            }
            memset(str, 0, sizeof(str));
        }
        fclose(saves);
        printf("load ready model finished succesfull\n");
        model_active = true;
        if (map == 0) motorway();
        else if (map == 1) crossroad();
        else if (map == 2) mult_crossroad();
    }
}

void motorway()
{
    map_1 = true, map_2 = false, map_3 = false;
    if (model_active == true)
    {
        if (head == NULL) head = create_car();
        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        map_show(0);
        glutSwapBuffers();
        clock_t start = clock(), end = 0, begin = clock();
        clock_t auto_save1 = clock(), auto_save2 = 0;
        int max_cars = 0, time = 0;
        if (Map.time != 0) time = Map.time;
        while (true)
        {
            if (GetAsyncKeyState(VK_SPACE) & 0x1) change_line(0, head);
            if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
            {
                model_active = false;
                pause_active = true;
                break;
            }
            if ((end - start) / CLOCKS_PER_SEC > Map.interval)
            {
                push_car(head);
                //print_cars(head);
                start = clock();
                end = 0;
            }
            if (Map.autosave == true && (auto_save2 - auto_save1) / CLOCKS_PER_SEC > 30)
            {
                model_save(0, head);
                auto_save1 = clock();
                auto_save2 = 0;
            }
            map_show(0);
            Map.time = (int)((clock() - begin) / CLOCKS_PER_SEC) + time;
            info(max_cars, head);
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
            check_distance(head);
            glutSwapBuffers();
            end = clock();
            auto_save2 = clock();
            if (max_cars < Map.car_counts) max_cars = Map.car_counts;
        }
    }
    if (pause_active == true) pause(0, head, false);
}

void light(int x, int y)
{
    glBegin(GL_QUADS);
    glVertex2i(x, Height - y);
    glVertex2i(x, Height - y + LIGHT_SIZE);
    glVertex2i(x + LIGHT_SIZE, Height - y + LIGHT_SIZE);
    glVertex2i(x + LIGHT_SIZE, Height - y);
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
    if (traffic_light1 == true && traffic_light2 == false)
    {
        light_draw(565, 615, 592, 568, true);
        light_draw(229, 214, 191, 169, true);
        light_draw(217, 615, 592, 568, false);
        light_draw(565, 214, 191, 169, false);
    }
    else if (traffic_light1 == false && traffic_light2 == true)
    {
        light_draw(565, 615, 592, 568, false);
        light_draw(229, 214, 191, 169, false);
        light_draw(217, 615, 592, 568, true);
        light_draw(565, 214, 191, 169, true);
    }
}

void check_light(Cars* head)
{
    Cars* temp = head;
    if (traffic_light1 == false && traffic_light2 == true)
    {
        while (temp != NULL)
        {
            if (temp->car.direction == 3 || temp->car.direction == 4)
            {
                if (temp->car.speed == 0)
                {
                    temp->car.speed = rand() % 30;
                    temp->car.speed += 60;
                }
                temp->car.speed += 0.0025;
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
                else temp->car.speed += 0.025;
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
                else temp->car.speed += 0.025;
            }
            temp = temp->next_car;
        }
    }
    else if (traffic_light1 == true && traffic_light2 == false)
    {
        while (temp != NULL)
        {
            if (temp->car.direction == 1 || temp->car.direction == 2)
            {
                if (temp->car.speed == 0)
                {
                    temp->car.speed = rand() % 30;
                    temp->car.speed += 60;
                }
                temp->car.speed += 0.0025;
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
                else temp->car.speed += 0.025;
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
                else temp->car.speed += 0.025;
            }
            temp = temp->next_car;
        }
    }
}

void crossroad()
{
    map_1 = false, map_2 = true, map_3 = false;
    if (model_active == true)
    {
        if (head == NULL) head = create_car();
        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        map_show(1);
        glutSwapBuffers();
        clock_t start = clock(), end = 0, begin = clock(), traffic1 = clock(), traffic2 = 0;
        clock_t auto_save1 = clock(), auto_save2 = 0;
        int max_cars = 0, time = 0;
        if (Map.time != 0) time = Map.time;
        while (true)
        {
            if (GetAsyncKeyState(VK_SPACE) & 0x1) change_line(1, head);
            if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
            {
                model_active = false;
                pause_active = true;
                break;
            }
            if ((end - start) / CLOCKS_PER_SEC > Map.interval)
            {
                push_car(head);
                //print_cars(head);
                start = clock();
                end = 0;
            }
            if ((traffic2 - traffic1) / CLOCKS_PER_SEC > 15)
            {
                if (traffic_light1 == true) traffic_light1 = false, traffic_light2 = true;
                else traffic_light1 = true, traffic_light2 = false;
                traffic1 = clock();
                traffic2 = 0;
            }
            if (Map.autosave == true && (auto_save2 - auto_save1) / CLOCKS_PER_SEC > 30)
            {
                model_save(1, head);
                auto_save1 = clock();
                auto_save2 = 0;
            }
            map_show(1);
            traffic_lights();
            Map.time = (int)((clock() - begin) / CLOCKS_PER_SEC) + time;
            info(max_cars, head);
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
                else if (tmp->car.direction == 3)
                {
                    if (tmp->car.x[0] > -44) tmp->car.x[0] -= tmp->car.speed / 3000;
                    else
                    {
                        if (tmp == head) head = delete_headcar(head);
                        else ptr->next_car = delete_car(tmp, head);
                        Map.car_counts--;
                        break;
                    }
                }
                else if (tmp->car.direction == 4)
                {
                    if (tmp->car.x[0] < 844) tmp->car.x[0] += tmp->car.speed / 3000;
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
            check_light(head);
            check_distance(head);
            glutSwapBuffers();
            end = clock();
            traffic2 = clock();
            auto_save2 = clock();
            if (max_cars < Map.car_counts) max_cars = Map.car_counts;
        }
    }
    if (pause_active == true) pause(1, head, false);
}

void mult_traffic_lights()
{
    if (traffic_light1 == true && traffic_light2 == false)
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
    else if (traffic_light1 == false && traffic_light2 == true)
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

void mult_check_light(Cars* head)
{
    Cars* temp = head;
    if (traffic_light1 == false && traffic_light2 == true)
    {
        while (temp != NULL)
        {
            if (temp->car.direction == 3 || temp->car.direction == 4 || temp->car.direction == 5 || temp->car.direction == 6)
            {
                if (temp->car.speed == 0)
                {
                    temp->car.speed = rand() % 30;
                    temp->car.speed += 60;
                }
                temp->car.speed += 0.0025;
            }
            else if (temp->car.direction == 1)
            {
                if (temp->car.y[0] < 85)
                {
                    if (85 - (temp->car.y[0] + CAR_HEIGHT) < 20 && 85 - (temp->car.y[0] + CAR_HEIGHT) > 0) temp->car.speed = 0;
                    else
                    {
                        if (temp->car.speed > 5) temp->car.speed -= 0.0015;
                    }
                }
                else if (temp->car.y[0] > 280 && temp->car.y[0] < 420)
                {
                    if (420 - (temp->car.y[0] + CAR_HEIGHT) < 20 && 420 - (temp->car.y[0] + CAR_HEIGHT) > 0) temp->car.speed = 0;
                    else
                    {
                        if (temp->car.speed > 5) temp->car.speed -= 0.0015;
                    }
                }
                else temp->car.speed += 0.025;
            }
            else if (temp->car.direction == 2)
            {
                if (temp->car.y[0] > 725)
                {
                    if (fabsf(725 - temp->car.y[0]) < 20 && fabsf(725 - temp->car.y[0]) > 0) temp->car.speed = 0;
                    else
                    {
                        if (temp->car.speed > 5) temp->car.speed -= 0.0015;
                    }
                }
                else if (temp->car.y[0] > 280 && temp->car.y[0] < 420)
                {
                    if (fabsf(280 - temp->car.y[0]) < 20 && fabsf(280 - temp->car.y[0]) > 0) temp->car.speed = 0;
                    else
                    {
                        if (temp->car.speed > 5) temp->car.speed -= 0.0015;
                    }
                }
                else temp->car.speed += 0.025;
            }
            temp = temp->next_car;
        }
    }
    else if (traffic_light1 == true && traffic_light2 == false)
    {
        while (temp != NULL)
        {
            if (temp->car.direction == 1 || temp->car.direction == 2)
            {
                if (temp->car.speed == 0)
                {
                    temp->car.speed = rand() % 30;
                    temp->car.speed += 60;
                }
                temp->car.speed += 0.0025;
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
                        if (temp->car.speed > 5) temp->car.speed -= 0.0015;
                    }
                }
                else temp->car.speed += 0.025;
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
                        if (temp->car.speed > 5) temp->car.speed -= 0.0015;
                    }
                }
                else temp->car.speed += 0.025;
            }
            temp = temp->next_car;
        }
    }
}

void mult_crossroad()
{
    map_1 = false, map_2 = false, map_3 = true;
    if (model_active == true)
    {
        if (head == NULL) head = create_car();
        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        map_show(2);
        glutSwapBuffers();
        clock_t start = clock(), end = 0, begin = clock(), traffic1 = clock(), traffic2 = 0;
        clock_t auto_save1 = clock(), auto_save2 = 0;
        int max_cars = 0, time = 0;
        if (Map.time != 0) time = Map.time;
        while (true)
        {
            if (GetAsyncKeyState(VK_SPACE) & 0x1) change_line(2, head);
            if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
            {
                model_active = false;
                pause_active = true;
                break;
            }
            if ((traffic2 - traffic1) / CLOCKS_PER_SEC > 15)
            {
                if (traffic_light1 == true) traffic_light1 = false, traffic_light2 = true;
                else traffic_light1 = true, traffic_light2 = false;
                traffic1 = clock();
                traffic2 = 0;
            }
            if ((end - start) / CLOCKS_PER_SEC > Map.interval)
            {
                push_car(head);
                start = clock();
                end = 0;
            }
            map_show(2);
            mult_traffic_lights();
            Map.time = (int)((clock() - begin) / CLOCKS_PER_SEC) + time;
            info(max_cars, head);
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
                else if (tmp->car.direction == 3 || tmp->car.direction == 5)
                {
                    if (tmp->car.x[0] > -44) tmp->car.x[0] -= tmp->car.speed / 3000;
                    else
                    {
                        if (tmp == head) head = delete_headcar(head);
                        else ptr->next_car = delete_car(tmp, head);
                        Map.car_counts--;
                        break;
                    }
                }
                else if (tmp->car.direction == 4 || tmp->car.direction == 6)
                {
                    if (tmp->car.x[0] < 844) tmp->car.x[0] += tmp->car.speed / 3000;
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
            mult_check_light(head);
            check_distance(head);
            glutSwapBuffers();
            end = clock();
            traffic2 = clock();
            if (max_cars < Map.car_counts) max_cars = Map.car_counts;
        }
    }
    if (pause_active == true) pause(2, head, false);
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
    menu_buttons(NULL, -2);
    menu_buttons(NULL, -5);

    glColor3ub(255, 255, 255);
    drawstring(Width / 2 - 40, Height - 305, "Motorway");
    drawstring(Width / 2 - 100, Height - 380, "Road with an intersection");
    drawstring(Width / 2 - 120, Height - 455, "Road with several intersections");
    drawstring(Width / 2 - 80, Height - 530, "Load a saved model");
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
        if (w == 0) drawstring(w + (Width / 2) + (MINI_BUTTON_WIDTH / 2) - 18, Height - h + 4, "0.75");
        else if (w == 90) drawstring(w + (Width / 2) + (MINI_BUTTON_WIDTH / 2) - 5, Height - h + 4, "1");
        else if (w == 180) drawstring(w + (Width / 2) + (MINI_BUTTON_WIDTH / 2) - 18, Height - h + 4, "1.25");
    }
    else if (h == 395)
    {
        glColor3ub(255, 255, 255);
        if (w == 0) drawstring(w + (Width / 2) + (MINI_BUTTON_WIDTH / 2) - 5, Height - h + 4, "2");
        else if (w == 90) drawstring(w + (Width / 2) + (MINI_BUTTON_WIDTH / 2) - 5, Height - h + 4, "3");
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
    if (Map.interval == 0.75)
    {
        draw_mini_buttons(0, 365, 1);
        draw_mini_buttons(90, 365, 0);
        draw_mini_buttons(180, 365, 0);
    }
    else if (Map.interval == 1)
    {
        draw_mini_buttons(0, 365, 0);
        draw_mini_buttons(90, 365, 1);
        draw_mini_buttons(180, 365, 0);
    }
    else if (Map.interval == 1.25)
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
    // If model load return error
    if (load_error == true)
    {
        glColor3ub(239, 211, 52);
        drawstring(Width / 2 - 130, 50, "Error loading the finished model!");
        load_error = false;
    }
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

void display(void)
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
                Map.interval = 0.75;
                processing_buttons(2);
            }
            else if (x <= 560 && x >= 490 && y >= 345 && y <= 365)
            {
                Map.interval = 1;
                processing_buttons(2);
            }
            else if (x <= 640 && x >= 580 && y >= 345 && y <= 365)
            {
                Map.interval = 1.25;
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
                Map.time = 0;
                Map.line = 1;
                map_button = false;
                model_active = true;
                motorway();
            }
            else if (x <= 575 && x >= 225 && y >= 350 && y <= 400)
            {
                Map.time = 0;
                Map.line = 1;
                map_button = false;
                model_active = true;
                crossroad();
            }
            else if (x <= 575 && x >= 225 && y >= 425 && y <= 475)
            {
                Map.time = 0;
                Map.line = 1;
                map_button = false;
                model_active = true;
                mult_crossroad();
            }
            else if (x <= 575 && x >= 225 && y >= 500 && y <= 550)
            {
                Map.time = 0;
                Map.line = 1;
                map_button = false;
                model_load();
            }
        }
    }
}

void keyboard(unsigned char key, int x, int y)
{
    if (pause_active == true)
    {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
        {
            pause_active = false;
            model_active = true;
            if (map_1 == true) motorway();
            else if (map_2 == true) crossroad();
            else if (map_3 == true) mult_crossroad();
        }
    }
}

void special_keyboard(int key, int x, int y)
{
    if (pause_active == true)
    {
        if (key == GLUT_KEY_F1)
        {
            head = remove_all(head);
            Map.car_counts = 0;
            pause_active = false;
            display();
        }
        if (key == GLUT_KEY_F2)
        {
            if (map_1 == true) model_save(0, head);
            else if (map_2 == true) model_save(1, head);
            else if (map_3 == true) model_save(2, head);
        }
        if (key == GLUT_KEY_F3)
        {
            add_car = true;
            push_car(head);
            if (map_1 == true) pause(0, head, false);
            else if (map_2 == true) pause(1, head, false);
            else if (map_3 == true) pause(2, NULL, false);
        }
    }
}
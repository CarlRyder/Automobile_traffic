/* Copyright (c) Yakov D. Sendov & Salavat S. Babaev, 2022 */

#define _CRT_SECURE_NO_WARNINGS

#include "pincludes.h"
#include "defines.h"

GLint Width = 800, Height = 800;

struct menu
{
    bool menu_active;
    bool model_active;
    bool pause_active;
    bool load_error;
} active;

struct button
{
    bool menu_button_1;
    bool menu_button_2;
    bool menu_button_3;
    bool map_button;
} buttons;

struct maps
{
    bool map_1;
    bool map_2;
    bool map_3;
} enable;

typedef struct car_stats
{
    float x[2];
    float y[2];
    float speed;
    bool turn;
    bool stop;
    unsigned short direction;
    unsigned short line;
    unsigned short texture_id;
} car_model;

typedef struct node
{
    car_model car;
    struct node* next_car;
} Cars;

struct stats
{
    unsigned short line;
    unsigned short car_counts;
    unsigned short time;
    float interval;
    bool autosave;
    bool add_car;
    bool traffic_light1;
    bool traffic_light2;
} model;

struct textures
{
    unsigned int texture[20];
} car_tex, map_tex;

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
void car_position(car_model* car, int first, int second, bool flag);
void car_draw(car_model* car);
void car_coords(car_model* car, int x, int y);
void car_static_show(Cars* head);
void coord_lines();
void init_car(car_model* car);
void push_car(Cars* head);
Cars* delete_headcar(Cars* head);
Cars* delete_car(Cars* current, Cars* head);
Cars* create_car();
void motorway();
void crossroad();
void mult_crossroad();
void menu_buttons(int h);
void text_buttons(int flag);
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
    active.load_error = false;
    enable.map_1 = false;
    enable.map_2 = false;
    enable.map_3 = false;
    // Main model struct initialization
    model.car_counts = 0;
    model.interval = 1;
    model.autosave = false;
    model.add_car = false;
    model.traffic_light1 = true;
    model.traffic_light2 = false;
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
            if (texture_file == NULL)
            {
                printf("There is no files with cars textures in the directory!\n");
                exit(0);
            }
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
        if (texture_file == NULL)
        {
            printf("There is no file with map textures in the directory!\n");
            exit(0);
        }
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
    control_lines(model.line);
}

void drawstring(float x, float y, char* string)
{
    glRasterPos2i(x, y);
    for (char* c = string; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

void coord_lines()
{
    glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
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
    fprintf(saves, "model settings:\n");
    fprintf(saves, "%d\n%d\n%d\n%f\n%d\n%d\n", map, model.line, model.car_counts, model.interval, model.autosave, model.time);
    Cars* temp = head;
    fprintf(saves, "ñars stats:\n");
    while (temp != NULL)
    {
        fprintf(saves, "%d %d %f %f %d %d %d %f\n", temp->car.stop, temp->car.direction, temp->car.x[0], temp->car.y[0], temp->car.line, temp->car.turn, temp->car.texture_id, temp->car.speed);
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
    glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
    drawstring(MID_COORD - 24, WINDOW_HEIGHT - 250, "PAUSE");
    drawstring(MID_COORD - 140, WINDOW_HEIGHT - 350, "To select, press the appropriate key:");
    drawstring(MID_COORD - 131, WINDOW_HEIGHT - 380, "F1 - exit to the menu without saving");
    drawstring(MID_COORD - 133, WINDOW_HEIGHT - 410, "F2 - save the current model to a file");
    drawstring(MID_COORD - 129, WINDOW_HEIGHT - 440, "F3 - add a car to the selected lane");
    if (flag == true) drawstring(Width / 2 - 190, WINDOW_HEIGHT - 50, "The model has been successfully saved to a file!");
    if (model.add_car == true)
    {
        drawstring(MID_COORD - 210, WINDOW_HEIGHT - 50, "The car was successfully created on the lane you selected!");
        model.add_car = false;
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
        if (enable.map_2 == true)
        {
            if (temp->car.direction == 4) car_line = temp->car.line + 6;
            else if (temp->car.direction == 3) car_line = temp->car.line + 9;
        }
        else if (enable.map_3 == true)
        {
            if (temp->car.direction == 4) car_line = temp->car.line + 6;
            else if (temp->car.direction == 3) car_line = temp->car.line + 8;
            else if (temp->car.direction == 6) car_line = temp->car.line + 10;
            else if (temp->car.direction == 5) car_line = temp->car.line + 13;
        }
        if (car_line == model.line) counter++;
        temp = temp->next_car;
    }
    return counter;
}

void info(int max, Cars* head)
{
    glColor3ub(239, 211, 52);
    drawstring(5, WINDOW_HEIGHT - 20, "SERVICE INFORMATION");

    glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
    drawstring(5, WINDOW_HEIGHT - 40, "Current number of cars:");
    char num[5] = { 0 };
    _itoa(model.car_counts, num, 10);
    drawstring(204, Height - 40, num);

    drawstring(5, WINDOW_HEIGHT - 60, "in the selected lane:");
    int car_line = count_car(head);
    char number[5] = { 0 };
    _itoa(car_line, number, 10);
    drawstring(174, WINDOW_HEIGHT - 60, number);

    drawstring(5, WINDOW_HEIGHT - 80, "Bandwidth:");
    char value[5] = { 0 };
    _itoa(max, value, 10);
    drawstring(100, WINDOW_HEIGHT - 80, value);

    if (enable.map_3 == true) drawstring(5, WINDOW_HEIGHT - 720, "Simulation time (sec):");
    else drawstring(5, WINDOW_HEIGHT - 100, "Simulation time (sec):");
    char model_time[10] = { 0 };
    _itoa(model.time, model_time, 10);
    if (enable.map_3 == true) drawstring(184, WINDOW_HEIGHT - 720, model_time);
    else drawstring(184, WINDOW_HEIGHT - 100, model_time);

    if (enable.map_3 == true) drawstring(5, WINDOW_HEIGHT - 740, "Interval time (sec):");
    else drawstring(5, WINDOW_HEIGHT - 120, "Interval time (sec):");
    if (enable.map_3 == true)
    {
        if (model.interval == 0.75) drawstring(160, WINDOW_HEIGHT - 740, "0.75");
        else if (model.interval == 1) drawstring(160, WINDOW_HEIGHT - 740, "1");
        else if (model.interval == 1.25) drawstring(160, WINDOW_HEIGHT - 740, "1.25");
    }
    else
    {
        if (model.interval == 0.75) drawstring(160, WINDOW_HEIGHT - 120, "0.75");
        else if (model.interval == 1) drawstring(160, WINDOW_HEIGHT - 120, "1");
        else if (model.interval == 1.25) drawstring(160, WINDOW_HEIGHT - 120, "1.25");
    }

    if (enable.map_3 == true) drawstring(5, WINDOW_HEIGHT - 760, "Autosave:");
    else drawstring(5, WINDOW_HEIGHT - 140, "Autosave:");
    if (enable.map_3 == true)
    {
        if (model.autosave == true) drawstring(90, WINDOW_HEIGHT - 760, "On");
        else drawstring(90, WINDOW_HEIGHT - 760, "Off");
    }
    else
    {
        if (model.autosave == true) drawstring(90, WINDOW_HEIGHT - 140, "On");
        else drawstring(90, WINDOW_HEIGHT - 140, "Off");
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

void change_line(int map, Cars* head)
{
    while (1)
    {
        map_show(map);
        if (head != NULL) car_static_show(head);
        control_lines(model.line);
        glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
        drawstring(MID_COORD - 200, WINDOW_HEIGHT - 40, "In order to provoke an accident, press the F5 key");
        drawstring(MID_COORD - 340, WINDOW_HEIGHT - 760, "To change the selected lane for an accident, use the arrows on the keyboard (<- ->)");
        glutSwapBuffers();
        if (GetAsyncKeyState(VK_LEFT) & 0x1 && model.line > 1) model.line--;
        if (enable.map_1 == true)
        {
            if (GetAsyncKeyState(VK_RIGHT) & 0x1 && model.line < 6) model.line++;
        }
        else if (enable.map_2 == true)
        {
            if (GetAsyncKeyState(VK_RIGHT) & 0x1 && model.line < 12) model.line++;
        }
        else if (enable.map_3 == true)
        {
            if (GetAsyncKeyState(VK_RIGHT) & 0x1 && model.line < 16) model.line++;
        }
        if (GetAsyncKeyState(VK_SPACE) & 0x1) break;
    }
}

void fill_array_cars(Cars* tmp, unsigned short* range_dir, Cars** car_dir)
{
    if (tmp->car.line == 1)
    {
        car_dir[range_dir[0]] = tmp;
        range_dir[0]++;
    }
    else if (tmp->car.line == 2)
    {
        car_dir[range_dir[1]] = tmp;
        range_dir[1]++;
    }
    else if (tmp->car.line == 3)
    {
        car_dir[range_dir[2]] = tmp;
        range_dir[2]++;
    }
}

int overtaking(int count_line, Cars** car_dir, int num_ccar, unsigned short* range_dir, int num_dir, char mode_coord)
{
    int start_right, end_right, start_left, end_left;
    bool possible_changing_r = false;
    bool possible_changing_l = false;
    bool can_changing_r = true;
    bool can_changing_l = true;
    int count = 0;
    int max_count = 0;
    if ((num_dir == 3 || num_dir == 4) && enable.map_3 == true)
    {
        if (count_line == 1)
        {
            start_right = model.car_counts;
            end_right = range_dir[1];
            possible_changing_r = true;
            count = 0;
        }
        else if (count_line == 2)
        {
            start_left = 0;
            end_left = range_dir[0];
            possible_changing_l = true;
            max_count = 2;
            count = 1;
        }
        bool possible_changing_tmp = false, can_changing_tmp = false;
        int end_tmp = 0, start_tmp = 0;
        if (count == 0 && possible_changing_r == true) //0 r
        {
            possible_changing_tmp = possible_changing_r;
            end_tmp = end_right;
            start_tmp = start_right;
            can_changing_tmp = can_changing_r;
        }
        else if (count == 1 && possible_changing_l == true) // 1 l
        {
            possible_changing_tmp = possible_changing_l;
            end_tmp = end_left;
            start_tmp = start_left;
            can_changing_tmp = can_changing_l;
        }
        if (possible_changing_tmp == true)
        {
            if (end_tmp - start_tmp == 0)
            {
                if (num_dir == 3 && count == 1) { car_dir[num_ccar]->car.y[0] = 215; car_dir[num_ccar]->car.line -= 1; }
                else if (num_dir == 3 && count == 0) { car_dir[num_ccar]->car.y[0] = 255; car_dir[num_ccar]->car.line += 1; }
                else if (num_dir == 4 && count == 1) { car_dir[num_ccar]->car.y[0] = 135; car_dir[num_ccar]->car.line -= 1; }
                else if (num_dir == 4 && count == 0) { car_dir[num_ccar]->car.y[0] = 175; car_dir[num_ccar]->car.line += 1; }
                return 1;
            }
            else
            {
                for (int num_dcar = start_tmp; num_dcar < end_tmp; num_dcar++)
                {
                    if (car_dir[num_dcar]->car.y[0] - car_dir[num_ccar]->car.y[0] > 0 &&
                        car_dir[num_dcar]->car.y[0] - car_dir[num_ccar]->car.y[0] < 100.0)
                    {
                        can_changing_tmp = false;
                    }
                    if (car_dir[num_dcar]->car.y[0] - car_dir[num_ccar]->car.y[0] < 0 &&
                        abs(car_dir[num_dcar]->car.y[0] - car_dir[num_ccar]->car.y[0]) < 100.0)
                    {
                        can_changing_tmp = false;
                    }
                }
                if (can_changing_tmp == true)
                {
                    if (num_dir == 3 && count == 1) { car_dir[num_ccar]->car.y[0] = 215; car_dir[num_ccar]->car.line -= 1; }
                    else if (num_dir == 3 && count == 0) { car_dir[num_ccar]->car.y[0] = 255; car_dir[num_ccar]->car.line += 1; }
                    else if (num_dir == 4 && count == 1) { car_dir[num_ccar]->car.y[0] = 135; car_dir[num_ccar]->car.line -= 1; }
                    else if (num_dir == 4 && count == 0) { car_dir[num_ccar]->car.y[0] = 175; car_dir[num_ccar]->car.line += 1; }
                    return 1;
                }
            }
        }
        return 0;
    }
    else
    {
        if (count_line == 1)
        {
            start_right = model.car_counts;
            end_right = range_dir[1];
            possible_changing_r = true;
            count = 0; max_count = 1;
        }
        else if (count_line == 2)
        {
            start_left = 0;
            end_left = range_dir[0];
            start_right = model.car_counts * 2;
            end_right = range_dir[2];
            possible_changing_r = true;
            possible_changing_l = true;
            max_count = 2;
            count = rand() % 1 + 0;
        }
        else if (count_line == 3)
        {
            start_left = model.car_counts;
            end_left = range_dir[1];
            possible_changing_l = true;
            count = 1; max_count = 2;
        }
        bool possible_changing_tmp = false, can_changing_tmp = false;
        int end_tmp = 0, start_tmp = 0;
        while (count < max_count)
        {
            if (count == 0 && possible_changing_r == true) //0 r
            {
                possible_changing_tmp = possible_changing_r;
                end_tmp = end_right;
                start_tmp = start_right;
                can_changing_tmp = can_changing_r;
            }
            else if (count == 1 && possible_changing_l == true) // 1 l
            {
                possible_changing_tmp = possible_changing_l;
                end_tmp = end_left;
                start_tmp = start_left;
                can_changing_tmp = can_changing_l;
            }
            if (possible_changing_tmp == true)
            {
                if (end_tmp - start_tmp == 0)
                {
                    if (count_line == 2 && count == 0) //r
                    {
                        if (num_dir == 1)  car_dir[num_ccar]->car.x[0] = 510;
                        else if (num_dir == 2) car_dir[num_ccar]->car.x[0] = 373;
                        else if (num_dir == 3 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 525;
                        else if (num_dir == 4 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 386;
                        else if (num_dir == 5) car_dir[num_ccar]->car.y[0] = 685;
                        else if (num_dir == 6) car_dir[num_ccar]->car.y[0] = 565;
                        car_dir[num_ccar]->car.line += 1;
                    }
                    else if (count_line == 2 && count == 1)//l
                    {
                        if (num_dir == 1) car_dir[num_ccar]->car.x[0] = 420;
                        else if (num_dir == 2)car_dir[num_ccar]->car.x[0] = 283;
                        else if (num_dir == 3 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 435;
                        else if (num_dir == 4 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 296;
                        else if (num_dir == 5) car_dir[num_ccar]->car.y[0] = 605;
                        else if (num_dir == 6) car_dir[num_ccar]->car.y[0] = 485;
                        car_dir[num_ccar]->car.line -= 1;
                    }
                    else if (count == 0 || count == 1) //r
                    {
                        if (num_dir == 1) car_dir[num_ccar]->car.x[0] = 465;
                        else if (num_dir == 2)car_dir[num_ccar]->car.x[0] = 328;
                        else if (num_dir == 3 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 475;
                        else if (num_dir == 4 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 341;
                        else if (num_dir == 5) car_dir[num_ccar]->car.y[0] = 640;
                        else if (num_dir == 6) car_dir[num_ccar]->car.y[0] = 525;

                        if (count == 1)  car_dir[num_ccar]->car.line -= 1;
                        else if (count == 0) car_dir[num_ccar]->car.line += 1;
                    }
                    return 1;
                }
                else
                {
                    for (int num_dcar = start_tmp; num_dcar < end_tmp; num_dcar++)
                    {
                        if (num_dir == 1 || num_dir == 2)
                        {
                            if (car_dir[num_dcar]->car.y[0] - car_dir[num_ccar]->car.y[0] > 0 &&
                                car_dir[num_dcar]->car.y[0] - car_dir[num_ccar]->car.y[0] < 100.0)
                            {
                                can_changing_tmp = false;
                            }
                            if (car_dir[num_dcar]->car.y[0] - car_dir[num_ccar]->car.y[0] < 0 &&
                                abs(car_dir[num_dcar]->car.y[0] - car_dir[num_ccar]->car.y[0]) < 100.0)
                            {
                                can_changing_tmp = false;
                            }
                        }
                        else  if ((num_dir == 3 || num_dir == 4) && enable.map_3 == true)
                        {
                            if (car_dir[num_dcar]->car.x[0] - car_dir[num_ccar]->car.x[0] > 0 &&
                                car_dir[num_dcar]->car.x[0] - car_dir[num_ccar]->car.x[0] < 100.0)
                            {
                                can_changing_tmp = false;
                            }
                            if (car_dir[num_dcar]->car.x[0] - car_dir[num_ccar]->car.x[0] < 0 &&
                                abs(car_dir[num_dcar]->car.x[0] - car_dir[num_ccar]->car.x[0]) < 100.0)
                            {
                                can_changing_tmp = false;
                            }
                        }
                        else  if (num_dir == 5 || num_dir == 6 || num_dir == 3 || num_dir == 4)
                        {
                            if (car_dir[num_dcar]->car.x[0] - car_dir[num_ccar]->car.x[0] > 0 &&
                                car_dir[num_dcar]->car.x[0] - car_dir[num_ccar]->car.x[0] < 100.0)
                            {
                                can_changing_tmp = false;
                            }
                            if (car_dir[num_dcar]->car.x[0] - car_dir[num_ccar]->car.x[0] < 0 &&
                                abs(car_dir[num_dcar]->car.x[0] - car_dir[num_ccar]->car.x[0]) < 100.0)
                            {
                                can_changing_tmp = false;
                            }
                        }
                    }
                    if (can_changing_tmp == true)
                    {
                        if (count_line == 2 && count == 0) //r
                        {
                            if (num_dir == 1)  car_dir[num_ccar]->car.x[0] = 510;
                            else if (num_dir == 2) car_dir[num_ccar]->car.x[0] = 373;
                            else if (num_dir == 3 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 525;
                            else if (num_dir == 4 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 386;
                            else if (num_dir == 5) car_dir[num_ccar]->car.y[0] = 685;
                            else if (num_dir == 6) car_dir[num_ccar]->car.y[0] = 565;
                            car_dir[num_ccar]->car.line += 1;
                        }
                        else if (count_line == 2 && count == 1)//l
                        {
                            if (num_dir == 1) car_dir[num_ccar]->car.x[0] = 420;
                            else if (num_dir == 2)car_dir[num_ccar]->car.x[0] = 283;
                            else if (num_dir == 3 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 435;
                            else if (num_dir == 4 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 296;
                            else if (num_dir == 5) car_dir[num_ccar]->car.y[0] = 605;
                            else if (num_dir == 6) car_dir[num_ccar]->car.y[0] = 485;
                            car_dir[num_ccar]->car.line -= 1;
                        }
                        else if (count == 0 || count == 1) //r
                        {
                            if (num_dir == 1) car_dir[num_ccar]->car.x[0] = 465;
                            else if (num_dir == 2) car_dir[num_ccar]->car.x[0] = 328;
                            else if (num_dir == 3 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 475;
                            else if (num_dir == 4 && enable.map_3 == false) car_dir[num_ccar]->car.y[0] = 341;
                            else if (num_dir == 5) car_dir[num_ccar]->car.y[0] = 640;
                            else if (num_dir == 6) car_dir[num_ccar]->car.y[0] = 525;

                            if (count == 1)  car_dir[num_ccar]->car.line -= 1;
                            else if (count == 0) car_dir[num_ccar]->car.line += 1;
                        }
                        return 1;
                    }
                }
            }
            count++;
        }
    }
    return 0;
}

void find_dist(char mode_coord, float coord_diff, Cars** car_dir, int num_dcar, int num_ccar, int count_line, unsigned short* range_dir1, int num_dir)
{
    float dcar = 0.0;
    float ccar = 0.0;
    if (mode_coord == 'y')
    {
        dcar = car_dir[num_dcar]->car.y[0];
        ccar = car_dir[num_ccar]->car.y[0];
    }
    else if (mode_coord == 'x')
    {
        dcar = car_dir[num_dcar]->car.x[0];
        ccar = car_dir[num_ccar]->car.x[0];
    }
    int changing = 0;
    if (coord_diff == 44.0 && dcar - ccar > 0 && dcar - ccar <= MIN_DISTANCE)
    {
        changing = overtaking(count_line, car_dir, num_ccar, range_dir1, num_dir, mode_coord);
        if (changing == 0)
        {
            car_dir[num_ccar]->car.speed = car_dir[num_dcar]->car.speed;
        }
    }
    else if (coord_diff == -44.0 && dcar - ccar < 0 && fabsf(dcar - ccar) <= MIN_DISTANCE)
    {
        changing = overtaking(count_line, car_dir, num_ccar, range_dir1, num_dir, mode_coord);
        if (changing == 0)
        {
            car_dir[num_ccar]->car.speed = car_dir[num_dcar]->car.speed;
        }
    }
}

void check_distance(Cars* head)
{
    Cars* tmp = head;

    Cars** car_dir_1 = (Cars**)malloc(model.car_counts * 3 * sizeof(Cars*));
    Cars** car_dir_2 = (Cars**)malloc(model.car_counts * 3 * sizeof(Cars*));
    Cars** car_dir_3 = (Cars**)malloc(model.car_counts * 3 * sizeof(Cars*));
    Cars** car_dir_4 = (Cars**)malloc(model.car_counts * 3 * sizeof(Cars*));
    Cars** car_dir_5 = (Cars**)malloc(model.car_counts * 3 * sizeof(Cars*));
    Cars** car_dir_6 = (Cars**)malloc(model.car_counts * 3 * sizeof(Cars*));

    unsigned short range_dir1[3] = { 0, model.car_counts, model.car_counts * 2 };
    unsigned short range_dir2[3] = { 0, model.car_counts, model.car_counts * 2 };
    unsigned short range_dir3[3] = { 0, model.car_counts, model.car_counts * 2 };
    unsigned short range_dir4[3] = { 0, model.car_counts, model.car_counts * 2 };
    unsigned short range_dir5[3] = { 0, model.car_counts, model.car_counts * 2 };
    unsigned short range_dir6[3] = { 0, model.car_counts, model.car_counts * 2 };

    while (tmp != NULL)
    {
        if (tmp->car.direction == 1) fill_array_cars(tmp, range_dir1, car_dir_1);
        else if (tmp->car.direction == 2) fill_array_cars(tmp, range_dir2, car_dir_2);
        else if (tmp->car.direction == 3) fill_array_cars(tmp, range_dir3, car_dir_3);
        else if (tmp->car.direction == 4) fill_array_cars(tmp, range_dir4, car_dir_4);
        else if (tmp->car.direction == 5) fill_array_cars(tmp, range_dir5, car_dir_5);
        else if (tmp->car.direction == 6) fill_array_cars(tmp, range_dir6, car_dir_6);
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
                start_num_arr = model.car_counts;
                if (count_dir == 1) end_arr = range_dir1[1];
                else if (count_dir == 2) end_arr = range_dir2[1];
                else if (count_dir == 3) end_arr = range_dir3[1];
                else if (count_dir == 4) end_arr = range_dir4[1];
                else if (count_dir == 5) end_arr = range_dir5[1];
                else if (count_dir == 6) end_arr = range_dir6[1];
            }
            else if (count_line == 3)
            {
                start_num_arr = model.car_counts * 2;
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
                        find_dist('y', 44.0, car_dir_1, num_dcar, num_ccar, count_line, range_dir1, count_dir);
                    }
                    else if (count_dir == 2 && car_dir_2[num_dcar] != car_dir_2[num_ccar])
                    {
                        find_dist('y', -44.0, car_dir_2, num_dcar, num_ccar, count_line, range_dir2, count_dir);
                    }
                    else if (count_dir == 3 && car_dir_3[num_dcar] != car_dir_3[num_ccar])
                    {
                        find_dist('x', -44.0, car_dir_3, num_dcar, num_ccar, count_line, range_dir3, count_dir);
                    }
                    else if (count_dir == 4 && car_dir_4[num_dcar] != car_dir_4[num_ccar])
                    {
                        find_dist('x', 44.0, car_dir_4, num_dcar, num_ccar, count_line, range_dir4, count_dir);
                    }
                    else if (count_dir == 5 && car_dir_5[num_dcar] != car_dir_5[num_ccar])
                    {
                        find_dist('x', -44.0, car_dir_5, num_dcar, num_ccar, count_line, range_dir5, count_dir);
                    }
                    else if (count_dir == 6 && car_dir_6[num_dcar] != car_dir_6[num_ccar])
                    {
                        find_dist('x', 44.0, car_dir_6, num_dcar, num_ccar, count_line, range_dir6, count_dir);
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
                if (car->y[0] > 220 && car->y[0] < 250)
                {
                    car_position(car, 0, 1, true);
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
                else car_position(car, 0, 1, true);
            }
            else car_position(car, 0, 1, true);
        }
        else if (car->direction == 2)
        {
            if (car->line == 1 && car->turn == false)
            {
                if (car->y[0] > 500 && car->y[0] < 540)
                {
                    car_position(car, 1, 0, true);
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
                else car_position(car, 1, 0, true);
            }
            else car_position(car, 1, 0, true);
        }
        else if (car->direction == 3)
        {
            if (car->line == 3 && car->turn == false)
            {
                if (car->x[0] > 550 && car->x[0] < 610)
                {
                    car_position(car, 1, 0, false);
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
                else car_position(car, 1, 0, false);
            }
            else car_position(car, 1, 0, false);
        }
        else if (car->direction == 4)
        {
            if (car->line == 1 && car->turn == false)
            {
                if (car->x[0] > 260 && car->x[0] < 290)
                {
                    car_position(car, 0, 1, false);
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
                else car_position(car, 0, 1, false);
            }
            else car_position(car, 0, 1, false);
        }
    }
    else if (enable.map_3 == true)
    {
        if (car->direction == 1) car_position(car, 0, 1, true);
        else if (car->direction == 2) car_position(car, 1, 0, true);
        else if (car->direction == 3) car_position(car, 1, 0, false);
        else if (car->direction == 4) car_position(car, 0, 1, false);
        else if (car->direction == 5) car_position(car, 1, 0, false);
        else if (car->direction == 6) car_position(car, 0, 1, false);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void car_coords(car_model* car, int x, int y)
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

void init_car(car_model* car)
{
    model.car_counts++;
    car->turn = false;
    car->stop = false;
    car->direction = rand() % 2 + 1;
    if (enable.map_2 == true) car->direction = rand() % 4 + 1;
    if (enable.map_3 == true) car->direction = rand() % 6 + 1;
    car->line = rand() % 3 + 1;
    if (model.add_car == true)
    {   
        if (enable.map_1 == true)
        {
            if (model.line < 4) car->direction = 2;
            else car->direction = 1;
            if (car->direction == 2) car->line = model.line;
            else car->line = model.line - 3;
        }
        else if (enable.map_2 == true)
        {
            if (model.line < 4) car->direction = 2;
            else if (model.line > 3 && model.line < 7) car->direction = 1;
            else if (model.line > 6 && model.line < 10) car->direction = 4;
            else if (model.line > 9 && model.line < 13) car->direction = 3;

            if (car->direction == 2) car->line = model.line;
            else if (car->direction == 1) car->line = model.line - 3;
            else if (car->direction == 4) car->line = model.line - 6;
            else if (car->direction == 3) car->line = model.line - 9;
        }
        else if (enable.map_3 == true)
        {
            if (model.line < 4) car->direction = 2;
            else if (model.line > 3 && model.line < 7) car->direction = 1;
            else if (model.line > 6 && model.line < 9) car->direction = 4;
            else if (model.line > 8 && model.line < 11) car->direction = 3;
            else if (model.line > 10 && model.line < 14) car->direction = 6;
            else if (model.line > 13 && model.line < 17) car->direction = 5;

            if (car->direction == 2) car->line = model.line;
            else if (car->direction == 1) car->line = model.line - 3;
            else if (car->direction == 4) car->line = model.line - 6;
            else if (car->direction == 3) car->line = model.line - 8;
            else if (car->direction == 4) car->line = model.line - 10;
            else if (car->direction == 3) car->line = model.line - 13;
        }
    }
    car->speed = rand() % 90;
    car->speed += 150;
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
        if (enable.map_2 == true)
        {
            if (car->line == 1) car_coords(car, 844, 435);
            else if (car->line == 2) car_coords(car, 844, 475);
            else if (car->line == 3) car_coords(car, 844, 525);
        }
        else if (enable.map_3 == true)
        {
            car->line = rand() % 2 + 1;
            if (car->line == 1) car_coords(car, 844, 215);
            else if (car->line == 2) car_coords(car, 844, 255);
        }
    }
    else if (car->direction == 4)
    {
        if (enable.map_2 == true)
        {
            if (car->line == 1) car_coords(car, -44, 296);
            else if (car->line == 2) car_coords(car, -44, 341);
            else if (car->line == 3) car_coords(car, -44, 386);
        }
        else if (enable.map_3 == true)
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

Cars* head = NULL;

void model_load()
{
    FILE* saves = fopen("map_save.txt", "r");
    if (saves == NULL)
    {
        active.load_error = true;
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
            if (strcmp(str, "model settings:") == 0) continue;
            if (counter == 2) map = atoi(str);
            else if (counter == 3) model.line = atoi(str);
            else if (counter == 4) model.car_counts = atoi(str);
            else if (counter == 5) model.interval = (float)atof(str);
            else if (counter == 6) model.autosave = atoi(str);
            else if (counter == 7) model.time = atoi(str);
            if (strcmp(str, "ñars stats:") == 0) continue;
            if (counter > 8)
            {
                int save_i = 0, count = 0;
                temp->car.stop = str[0] - '0';
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
        active.model_active = true;
        if (map == 0) motorway();
        else if (map == 1) crossroad();
        else if (map == 2) mult_crossroad();
    }
}

void stop_car(Cars* head)
{
    Cars* temp = head;
    int car_line = 0;
    while (temp != NULL)
    {
        if (temp->car.direction == 2) car_line = temp->car.line;
        else if (temp->car.direction == 1) car_line = temp->car.line + 3;
        if (enable.map_2 == true)
        {
            if (temp->car.direction == 4) car_line = temp->car.line + 6;
            else if (temp->car.direction == 3) car_line = temp->car.line + 9;
        }
        else if (enable.map_3 == true)
        {
            if (temp->car.direction == 4) car_line = temp->car.line + 6;
            else if (temp->car.direction == 3) car_line = temp->car.line + 8;
            else if (temp->car.direction == 6) car_line = temp->car.line + 10;
            else if (temp->car.direction == 5) car_line = temp->car.line + 13;
        }
        if (car_line == model.line)
        {
            temp->car.stop = true;
            temp->car.speed = 0;
            break;
        }
        temp = temp->next_car;
    }
}

void motorway()
{
    enable.map_1 = true, enable.map_2 = false, enable.map_3 = false;
    if (active.model_active == true)
    {
        if (head == NULL) head = create_car();
        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        map_show(0);
        glutSwapBuffers();
        clock_t start = clock(), end = 0, begin = clock();
        clock_t auto_save1 = clock(), auto_save2 = 0;
        int max_cars = 0, time = 0;
        if (model.time != 0) time = model.time;
        while (true)
        {
            if (GetAsyncKeyState(VK_SPACE) & 0x1) change_line(0, head);
            if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
            {
                active.model_active = false;
                active.pause_active = true;
                break;
            }
            if (GetAsyncKeyState(VK_F5) & 0x1) stop_car(head);
            if ((end - start) / CLOCKS_PER_SEC > model.interval)
            {
                push_car(head);
                start = clock();
                end = 0;
            }
            if (model.autosave == true && (auto_save2 - auto_save1) / CLOCKS_PER_SEC > 30)
            {
                model_save(0, head);
                auto_save1 = clock();
                auto_save2 = 0;
            }
            map_show(0);
            model.time = (int)((clock() - begin) / CLOCKS_PER_SEC) + time;
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
                        model.car_counts--;
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
                        model.car_counts--;
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
            if (max_cars < model.car_counts) max_cars = model.car_counts;
        }
    }
    if (active.pause_active == true) pause(0, head, false);
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

void check_light(Cars* head)
{
    Cars* temp = head;
    if (model.traffic_light1 == false && model.traffic_light2 == true)
    {
        while (temp != NULL)
        {
            if (temp->car.direction == 3 || temp->car.direction == 4)
            {
                if (temp->car.stop == false)
                {
                    if (temp->car.speed == 0)
                    {
                        temp->car.speed = rand() % 30;
                        temp->car.speed += 60;
                    }
                    temp->car.speed += 0.005;
                }
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
                else if (temp->car.stop == false) temp->car.speed += 0.025;
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
                else if (temp->car.stop == false) temp->car.speed += 0.025;
            }
            temp = temp->next_car;
        }
    }
    else if (model.traffic_light1 == true && model.traffic_light2 == false)
    {
        while (temp != NULL)
        {
            if (temp->car.direction == 1 || temp->car.direction == 2)
            {
                if (temp->car.stop == false)
                {
                    if (temp->car.speed == 0)
                    {
                        temp->car.speed = rand() % 30;
                        temp->car.speed += 60;
                    }
                    temp->car.speed += 0.005;
                }
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
                else if (temp->car.stop == false) temp->car.speed += 0.025;
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
                else if (temp->car.stop == false) temp->car.speed += 0.025;
            }
            temp = temp->next_car;
        }
    }
}

void check_cars(Cars* head)
{
    if (enable.map_2 == true)
    {
        Cars* temp = head, * ptr = head;
        while (temp != NULL)
        {
            if (temp->car.direction == 1)
            {
                while (ptr != NULL)
                {
                    if (ptr->car.direction == 4)
                    {
                        if ((ptr->car.y[0] - CAR_WIDTH) - (temp->car.y[0] + CAR_HEIGHT) < 21 && temp->car.x[0] < ptr->car.x[0] + CAR_WIDTH && temp->car.x[0] > ptr->car.x[0] - (CAR_HEIGHT + CAR_WIDTH))
                        {
                            printf("1\n");
                            temp->car.speed = 0;
                        }
                    }
                    else if (ptr->car.direction == 3)
                    {
                        if ((ptr->car.y[0] - CAR_WIDTH) - (temp->car.y[0] + CAR_HEIGHT) < 21 && temp->car.x[0] < ptr->car.x[0] + CAR_WIDTH && temp->car.x[0] > ptr->car.x[0] + (CAR_HEIGHT + CAR_WIDTH))
                        {
                            printf("2\n");
                            temp->car.speed = 0;
                        }
                    }
                    ptr = ptr->next_car;
                }
            }
            else if (temp->car.direction == 2)
            {
                while (ptr != NULL)
                {
                    if (ptr->car.direction == 4)
                    {
                        if (fabsf(ptr->car.y[0] - temp->car.y[0]) < 21 && temp->car.x[0] < ptr->car.x[0] + CAR_WIDTH && temp->car.x[0] > ptr->car.x[0] - (CAR_HEIGHT + CAR_WIDTH))
                        {
                            printf("3\n");
                            temp->car.speed = 0;
                        }
                    }
                    else if (ptr->car.direction == 3)
                    {
                        if (fabsf(ptr->car.y[0] - temp->car.y[0]) < 21 && temp->car.x[0] < ptr->car.x[0] + CAR_WIDTH && temp->car.x[0] > ptr->car.x[0] + (CAR_HEIGHT + CAR_WIDTH))
                        {
                            printf("4\n");
                            temp->car.speed = 0;
                        }
                    }
                    ptr = ptr->next_car;
                }
            }
            else if (temp->car.direction == 3)
            {
                while (ptr != NULL)
                {
                    if (ptr->car.direction == 1)
                    {
                        if ((ptr->car.x[0] + CAR_WIDTH) - (temp->car.x[0] + CAR_HEIGHT) > 21
                            && temp->car.y[0] > ptr->car.y[0] - CAR_WIDTH && temp->car.y[0] < ptr->car.y[0] + (CAR_HEIGHT + CAR_WIDTH))
                        {
                            printf("5\n");
                            temp->car.speed = 0;
                        }
                    }
                    else if (ptr->car.direction == 2)
                    {
                        if ((ptr->car.x[0] + CAR_WIDTH) - (temp->car.x[0] + CAR_HEIGHT) > 21
                            && temp->car.y[0] > ptr->car.y[0] + CAR_WIDTH && temp->car.y[0] < ptr->car.y[0] - (CAR_HEIGHT + CAR_WIDTH))
                        {
                            printf("6\n");
                            temp->car.speed = 0;
                        }
                    }
                    ptr = ptr->next_car;
                }
            }
            else if (temp->car.direction == 4)
            {
                while (ptr != NULL)
                {
                    if (ptr->car.direction == 1)
                    {
                        if (fabsf(ptr->car.x[0] - temp->car.x[0]) < 21 
                            && temp->car.y[0] > ptr->car.y[0] - CAR_WIDTH && temp->car.y[0] < ptr->car.y[0] + (CAR_HEIGHT + CAR_WIDTH))
                        {
                            printf("7\n");
                            temp->car.speed = 0;
                        }
                    }
                    else if (ptr->car.direction == 2)
                    {
                        if (fabsf(ptr->car.x[0] - temp->car.x[0]) < 21 
                            && temp->car.y[0] < ptr->car.y[0] - CAR_WIDTH && temp->car.y[0] > ptr->car.y[0] - (CAR_HEIGHT + CAR_WIDTH))
                        {
                            printf("8\n");
                            temp->car.speed = 0;
                        }
                    }
                    ptr = ptr->next_car;
                }
            }
            ptr = head;
            temp = temp->next_car;
        }
    }
}

void crossroad()
{
    enable.map_1 = false, enable.map_2 = true, enable.map_3 = false;
    if (active.model_active == true)
    {
        if (head == NULL) head = create_car();
        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        map_show(1);
        glutSwapBuffers();
        clock_t start = clock(), end = 0, begin = clock(), traffic1 = clock(), traffic2 = 0;
        clock_t auto_save1 = clock(), auto_save2 = 0;
        int max_cars = 0, time = 0;
        if (model.time != 0) time = model.time;
        while (true)
        {
            if (GetAsyncKeyState(VK_SPACE) & 0x1) change_line(1, head);
            if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
            {
                active.model_active = false;
                active.pause_active = true;
                break;
            }
            if (GetAsyncKeyState(VK_F5) & 0x1)
            {
                stop_car(head);
            }
            if ((end - start) / CLOCKS_PER_SEC > model.interval)
            {
                push_car(head);
                start = clock();
                end = 0;
            }
            if ((traffic2 - traffic1) / CLOCKS_PER_SEC > 15)
            {
                if (model.traffic_light1 == true) model.traffic_light1 = false, model.traffic_light2 = true;
                else model.traffic_light1 = true, model.traffic_light2 = false;
                traffic1 = clock();
                traffic2 = 0;
            }
            if (model.autosave == true && (auto_save2 - auto_save1) / CLOCKS_PER_SEC > 30)
            {
                model_save(1, head);
                auto_save1 = clock();
                auto_save2 = 0;
            }
            map_show(1);
            traffic_lights();
            model.time = (int)((clock() - begin) / CLOCKS_PER_SEC) + time;
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
                        model.car_counts--;
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
                        model.car_counts--;
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
                        model.car_counts--;
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
                        model.car_counts--;
                        break;
                    }
                }
                ptr = tmp;
                tmp = tmp->next_car;
            }
            check_light(head);
            check_cars(head);
            check_distance(head);
            glutSwapBuffers();
            end = clock();
            traffic2 = clock();
            auto_save2 = clock();
            if (max_cars < model.car_counts) max_cars = model.car_counts;
        }
    }
    if (active.pause_active == true) pause(1, head, false);
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

void mult_check_light(Cars* head)
{
    Cars* temp = head;
    if (model.traffic_light1 == false && model.traffic_light2 == true)
    {
        while (temp != NULL)
        {
            if (temp->car.direction == 3 || temp->car.direction == 4 || temp->car.direction == 5 || temp->car.direction == 6)
            {
                if (temp->car.stop == false)
                {
                    if (temp->car.speed == 0)
                    {
                        temp->car.speed = rand() % 30;
                        temp->car.speed += 60;
                    }
                    temp->car.speed += 0.005;
                }
            }
            else if (temp->car.direction == 1)
            {
                if (temp->car.y[0] < 85)
                {
                    if (85 - (temp->car.y[0] + CAR_HEIGHT) < 20 && 85 - (temp->car.y[0] + CAR_HEIGHT) > 0) temp->car.speed = 0;
                    else
                    {
                        if (temp->car.speed > 1) temp->car.speed -= 0.005;
                    }
                }
                else if (temp->car.y[0] > 280 && temp->car.y[0] < 420)
                {
                    if (420 - (temp->car.y[0] + CAR_HEIGHT) < 20 && 420 - (temp->car.y[0] + CAR_HEIGHT) > 0) temp->car.speed = 0;
                    else
                    {
                        if (temp->car.speed > 1) temp->car.speed -= 0.005;
                    }
                }
                else if (temp->car.stop == false) temp->car.speed += 0.04;
            }
            else if (temp->car.direction == 2)
            {
                if (temp->car.y[0] > 725)
                {
                    if (fabsf(725 - temp->car.y[0]) < 20 && fabsf(725 - temp->car.y[0]) > 0) temp->car.speed = 0;
                    else
                    {
                        if (temp->car.speed > 1) temp->car.speed -= 0.005;
                    }
                }
                else if (temp->car.y[0] > 280 && temp->car.y[0] < 420)
                {
                    if (fabsf(280 - temp->car.y[0]) < 20 && fabsf(280 - temp->car.y[0]) > 0) temp->car.speed = 0;
                    else
                    {
                        if (temp->car.speed > 1) temp->car.speed -= 0.005;
                    }
                }
                else if (temp->car.stop == false) temp->car.speed += 0.04;
            }
            temp = temp->next_car;
        }
    }
    else if (model.traffic_light1 == true && model.traffic_light2 == false)
    {
        while (temp != NULL)
        {
            if (temp->car.direction == 1 || temp->car.direction == 2)
            {
                if (temp->car.stop == false)
                {
                    if (temp->car.speed == 0)
                    {
                        temp->car.speed = rand() % 30;
                        temp->car.speed += 60;
                    }
                    temp->car.speed += 0.005;
                }
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
                        if (temp->car.speed > 1) temp->car.speed -= 0.005;
                    }
                }
                else if (temp->car.stop == false) temp->car.speed += 0.04;
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
                        if (temp->car.speed > 5) temp->car.speed -= 0.005;
                    }
                }
                else if (temp->car.stop == false) temp->car.speed += 0.04;
            }
            temp = temp->next_car;
        }
    }
}

void mult_crossroad()
{
    enable.map_1 = false, enable.map_2 = false, enable.map_3 = true;
    if (active.model_active == true)
    {
        if (head == NULL) head = create_car();
        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        map_show(2);
        glutSwapBuffers();
        clock_t start = clock(), end = 0, begin = clock(), traffic1 = clock(), traffic2 = 0;
        clock_t auto_save1 = clock(), auto_save2 = 0;
        int max_cars = 0, time = 0, save_dir = 0, save_line = 0;
        float save_x = 0, save_y = 0;
        if (model.time != 0) time = model.time;
        while (true)
        {
            if (GetAsyncKeyState(VK_SPACE) & 0x1) change_line(2, head);
            if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
            {
                active.model_active = false;
                active.pause_active = true;
                break;
            }
            if (GetAsyncKeyState(VK_F5) & 0x1)
            {
                stop_car(head);
            }
            if ((traffic2 - traffic1) / CLOCKS_PER_SEC > 15)
            {
                if (model.traffic_light1 == true) model.traffic_light1 = false, model.traffic_light2 = true;
                else model.traffic_light1 = true, model.traffic_light2 = false;
                traffic1 = clock();
                traffic2 = 0;
            }
            if ((end - start) / CLOCKS_PER_SEC > model.interval)
            {
                push_car(head);
                start = clock();
                end = 0;
            }
            map_show(2);
            mult_traffic_lights();
            model.time = (int)((clock() - begin) / CLOCKS_PER_SEC) + time;
            info(max_cars, head);
            Cars* tmp = head, * ptr = NULL;
            while (tmp != NULL)
            {
                car_draw(&tmp->car);
                if (tmp->car.direction == 1)
                {
                    save_dir = tmp->car.direction;
                    save_line = tmp->car.line;
                    save_y = tmp->car.y[0];
                    if (tmp->car.y[0] < 844) tmp->car.y[0] += tmp->car.speed / 3000;
                    else
                    {
                        if (tmp == head) head = delete_headcar(head);
                        else ptr->next_car = delete_car(tmp, head);
                        model.car_counts--;
                        break;
                    }
                }
                else if (tmp->car.direction == 2)
                {
                    save_dir = tmp->car.direction;
                    save_line = tmp->car.line;
                    save_y = tmp->car.y[0];
                    if (tmp->car.y[0] > -44) tmp->car.y[0] -= tmp->car.speed / 3000;
                    else
                    {
                        if (tmp == head) head = delete_headcar(head);
                        else ptr->next_car = delete_car(tmp, head);
                        model.car_counts--;
                        break;
                    }
                }
                else if (tmp->car.direction == 3 || tmp->car.direction == 5)
                {
                    save_dir = tmp->car.direction;
                    save_line = tmp->car.line;
                    save_x = tmp->car.x[0];
                    if (tmp->car.x[0] > -44) tmp->car.x[0] -= tmp->car.speed / 3000;
                    else
                    {
                        if (tmp == head) head = delete_headcar(head);
                        else ptr->next_car = delete_car(tmp, head);
                        model.car_counts--;
                        break;
                    }
                }
                else if (tmp->car.direction == 4 || tmp->car.direction == 6)
                {
                    save_dir = tmp->car.direction;
                    save_line = tmp->car.line;
                    save_x = tmp->car.x[0];
                    if (tmp->car.x[0] < 844) tmp->car.x[0] += tmp->car.speed / 3000;
                    else
                    {
                        if (tmp == head) head = delete_headcar(head);
                        else ptr->next_car = delete_car(tmp, head);
                        model.car_counts--;
                        break;
                    }
                }
                ptr = tmp;
                tmp = tmp->next_car;
            }
            mult_check_light(head);
            check_distance(head);
            check_cars(head);
            glutSwapBuffers();
            end = clock();
            traffic2 = clock();
            if (max_cars < model.car_counts) max_cars = model.car_counts;
        }
    }
    if (active.pause_active == true) pause(2, head, false);
}

void menu_buttons(int h)
{
    glColor3ub(COLOR_BUTTON_RED, COLOR_BUTTON_GREEN, COLOR_BUTTON_BLUE);
    glBegin(GL_QUADS);
    glVertex2i(MENU_BUTTON_LEFT, MENU_BUTTON_BOTTOM + (h - 3) * BUTTON_HEIGHT / 2);
    glVertex2i(MENU_BUTTON_LEFT, MENU_BUTTON_TOP + (h - 3) * BUTTON_HEIGHT / 2);
    glVertex2i(MENU_BUTTON_RIGHT, MENU_BUTTON_TOP + (h - 3) * BUTTON_HEIGHT / 2);
    glVertex2i(MENU_BUTTON_RIGHT, MENU_BUTTON_BOTTOM + (h - 3) * BUTTON_HEIGHT / 2);
    glEnd();
}

void map_choose()
{
    buttons.map_button = true;
    glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = -5; i < 8; i += 3) menu_buttons(i);
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

void text_buttons(int flag)
{
    glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
    if (flag == 0) drawstring(MID_COORD - 115, WINDOW_HEIGHT - 200, "CAR TRAFFIC SIMULATOR");
    else if (flag == 1) drawstring(MID_COORD - 70, WINDOW_HEIGHT - 380, "Automobile traffic");
    else if (flag == 2) drawstring(MID_COORD - 40, WINDOW_HEIGHT - 455, "Settings");
    else if (flag == 3) drawstring(MID_COORD - 40, WINDOW_HEIGHT - 530, "About us");
    else if (flag == 4) drawstring(MID_COORD - 20, WINDOW_HEIGHT - 605, "Exit");
}

void draw_mini_buttons(int w, int h, int flag)
{
    if (flag == 0) glColor3ub(COLOR_BUTTON_RED, COLOR_BUTTON_GREEN, COLOR_BUTTON_BLUE);
    else if (flag == 1) glColor3ub(0, 153, 0);
    glBegin(GL_QUADS);
    glVertex2i(Width / 2 + w, Height - h);
    glVertex2i(Width / 2 + w, Height - h + MINI_BUTTON_HEIGHT);
    glVertex2i(Width / 2 + MINI_BUTTON_WIDTH + w, Height - h + MINI_BUTTON_HEIGHT);
    glVertex2i(Width / 2 + MINI_BUTTON_WIDTH + w, Height - h);
    glEnd();
    if (h == 365)
    {
        glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
        if (w == 0) drawstring(w + MID_COORD + (MINI_BUTTON_WIDTH / 2) - 18, WINDOW_HEIGHT - h + 4, "0.75");
        else if (w == 90) drawstring(w + MID_COORD + (MINI_BUTTON_WIDTH / 2) - 5, WINDOW_HEIGHT - h + 4, "1");
        else if (w == 180) drawstring(w + MID_COORD + (MINI_BUTTON_WIDTH / 2) - 18, WINDOW_HEIGHT - h + 4, "1.25");
    }
    else if (h == 395)
    {
        glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
        if (w == 0) drawstring(w + MID_COORD + (MINI_BUTTON_WIDTH / 2) - 15, WINDOW_HEIGHT - h + 5, "yes");
        else if (w == 90) drawstring(w + MID_COORD + (MINI_BUTTON_WIDTH / 2) - 10, WINDOW_HEIGHT - h + 5, "no");
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

void draw_welcome_text()
{
    glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
    drawstring(Width / 2 - 95, Height - 200, "AUTOMOBILE TRAFFIC");
    drawstring(Width / 2 - 200, Height - 270, "Good afternoon! A Car Traffic Simulator welcomes you.");
    drawstring(Width / 2 - 180, Height - 300, "There are several types of roads available to you:");
    drawstring(Width / 2 - 270, Height - 330, "a motorway, a road with an intersection, a road with several intersections.");
    drawstring(Width / 2 - 200, Height - 360, "You can simulate an accident in one of the traffic lanes.");
    drawstring(Width / 2 - 195, Height - 390, "To do this, you need to select one of the bands using");
    drawstring(Width / 2 - 150, Height - 420, "the arrows on the keyboard (<- and ->).");
    glColor3ub(239, 211, 52);
    drawstring(Width / 2 - 180, Height - 450, "The strip you selected will be highlighted in yellow!");
    glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
    drawstring(Width / 2 - 240, Height - 480, "To simulate an accident situation, you need to press the space bar.");
    drawstring(Width / 2 - 80, Height - 510, "To continue, click next.");
}

void draw_next_buttons(int a, int b)
{
    glColor3ub(COLOR_BUTTON_RED, COLOR_BUTTON_GREEN, COLOR_BUTTON_BLUE);
    glBegin(GL_QUADS);
    glVertex2i(MENU_BUTTON_LEFT + b, MENU_BUTTON_BOTTOM + (-7) * BUTTON_HEIGHT / 2);
    glVertex2i(MENU_BUTTON_LEFT + b, MENU_BUTTON_TOP + (-7) * BUTTON_HEIGHT / 2);
    glVertex2i(MENU_BUTTON_RIGHT + a, MENU_BUTTON_TOP + (-7) * BUTTON_HEIGHT / 2);
    glVertex2i(MENU_BUTTON_RIGHT + a, MENU_BUTTON_BOTTOM + (-7) * BUTTON_HEIGHT / 2);
    glEnd();
}

void processing_buttons(int button)
{
    if (button == 3)
    {
        buttons.menu_button_3 = true;
        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
        drawstring(Width / 2 - 115, Height - 200, "CAR TRAFFIC SIMULATOR");
        drawstring(Width / 2 - 47, Height - 300, "Developers:");
        drawstring(Width / 2 - 140, Height - 330, "Sendov Yakov Denisovich | @cryder");
        drawstring(Width / 2 - 160, Height - 360, "Babaev Salavat Shakirovich | @lilbabidjon");
        drawstring(Width / 2 - 63, Height - 440, "4831001/10003");
        drawstring(Width / 4, Height - 470, "Institute of Cybersecurity and Information Protection");
        drawstring(Width / 6, Height - 500, "Copyright (c) 2022 Saint-Petersburg Polytechnic University, Russia");
        menu_buttons(-4);
        glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
        drawstring(Width / 2 - 20, Height - 580, "Back");
        active.menu_active = false;
        glutSwapBuffers();
    }
    else if (button == 2)
    {
        buttons.menu_button_2 = true;
        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
        drawstring(Width / 2 - 115, Height - 200, "CAR TRAFFIC SIMULATOR");
        drawstring(Width / 2 - 37, Height - 300, "Settings:");
        drawstring(Width / 10 - 34, Height - 360, "Spawn interval of machines (seconds):");
        drawstring(Width / 3 + 14, Height - 390, "Autosave:");
        settings_buttons();
        menu_buttons(-4);
        glColor3ub(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
        drawstring(Width / 2 - 20, Height - 580, "Back");
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
        drawstring(Width / 2 - 165, Height - 580, "Back");
        drawstring(Width / 2 + 15, Height - 580, "Next");
        active.menu_active = false;
        glutSwapBuffers();
    }
}

void menu()
{
    // If model load return error
    if (active.load_error == true)
    {
        glColor3ub(239, 211, 52);
        drawstring(Width / 2 - 130, 50, "Error loading the finished model!");
        active.load_error = false;
    }
    srand(time(NULL));
    // Drawing menu buttons
    for (int i = -5; i < 5; i += 3) menu_buttons(i);
    // Rendering text in the menu
    for (int i = 0; i < 5; i++) text_buttons(i);
    active.menu_active = true;
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
        if (active.menu_active == true)
        {
            if (x >= MENU_BUTTON_LEFT && x <= MENU_BUTTON_RIGHT && y >= 575 && y <= 625) exit(0);
            else if (x >= MENU_BUTTON_LEFT && x <= MENU_BUTTON_RIGHT && y >= 500 && y <= 550) processing_buttons(3);
            else if (x >= MENU_BUTTON_LEFT && x <= MENU_BUTTON_RIGHT && y >= 425 && y <= 475) processing_buttons(2);
            else if (x >= MENU_BUTTON_LEFT && x <= MENU_BUTTON_RIGHT && y >= 350 && y <= 400) processing_buttons(1);
        }
        else if (buttons.menu_button_3 == true)
        {
            if (x >= MENU_BUTTON_LEFT && x <= MENU_BUTTON_RIGHT && y >= 550 && y <= 600)
            {
                buttons.menu_button_3 = false;
                glutPostRedisplay();
            }
        }
        else if (buttons.menu_button_2 == true)
        {
            if (x >= MENU_BUTTON_LEFT && x <= MENU_BUTTON_RIGHT && y >= 550 && y <= 600)
            {
                buttons.menu_button_2 = false;
                glutPostRedisplay();
            }
            else if (x <= 470 && x >= 400 && y >= 345 && y <= 365)
            {
                model.interval = 0.75;
                processing_buttons(2);
            }
            else if (x <= 560 && x >= 490 && y >= 345 && y <= 365)
            {
                model.interval = 1;
                processing_buttons(2);
            }
            else if (x <= 640 && x >= 580 && y >= 345 && y <= 365)
            {
                model.interval = 1.25;
                processing_buttons(2);
            }
            else if (x <= 470 && x >= 400 && y >= 375 && y <= 395)
            {
                model.autosave = true;
                processing_buttons(2);
            }
            else if (x <= 560 && x >= 490 && y >= 375 && y <= 395)
            {
                model.autosave = false;
                processing_buttons(2);
            }
        }
        else if (buttons.menu_button_1 == true)
        {
            if (x >= MENU_BUTTON_LEFT && x <= MENU_BUTTON_LEFT + 60 && y >= 550 && y <= 600)
            {
                buttons.menu_button_1 = false;
                glutPostRedisplay();
            }
            else if (x >= MENU_BUTTON_LEFT + 80 && x <= MENU_BUTTON_RIGHT && y >= 550 && y <= 600)
            {
                buttons.menu_button_1 = false;
                map_choose();
            }
        }
        else if (buttons.map_button == true)
        {
            if (x >= MENU_BUTTON_LEFT && x <= MENU_BUTTON_RIGHT && y >= 575 && y <= 625)
            {
                buttons.map_button = false;
                glutPostRedisplay();
            }
            else if (x >= MENU_BUTTON_LEFT && x <= MENU_BUTTON_RIGHT && y >= 275 && y <= 325)
            {
                model.time = 0;
                model.line = 1;
                buttons.map_button = false;
                active.model_active = true;
                motorway();
            }
            else if (x >= MENU_BUTTON_LEFT && x <= MENU_BUTTON_RIGHT && y >= 350 && y <= 400)
            {
                model.time = 0;
                model.line = 1;
                buttons.map_button = false;
                active.model_active = true;
                crossroad();
            }
            else if (x >= MENU_BUTTON_LEFT && x <= MENU_BUTTON_RIGHT && y >= 425 && y <= 475)
            {
                model.time = 0;
                model.line = 1;
                buttons.map_button = false;
                active.model_active = true;
                mult_crossroad();
            }
            else if (x >= MENU_BUTTON_LEFT && x <= MENU_BUTTON_RIGHT && y >= 500 && y <= 550)
            {
                model.time = 0;
                model.line = 1;
                buttons.map_button = false;
                model_load();
            }
        }
    }
}

void keyboard(unsigned char key, int x, int y)
{
    if (active.pause_active == true)
    {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
        {
            active.pause_active = false;
            active.model_active = true;
            if (enable.map_1 == true) motorway();
            else if (enable.map_2 == true) crossroad();
            else if (enable.map_3 == true) mult_crossroad();
        }
    }
}

void special_keyboard(int key, int x, int y)
{
    if (active.pause_active == true)
    {
        if (key == GLUT_KEY_F1)
        {
            head = remove_all(head);
            model.car_counts = 0;
            active.pause_active = false;
            display();
        }
        if (key == GLUT_KEY_F2)
        {
            if (enable.map_1 == true) model_save(0, head);
            else if (enable.map_2 == true) model_save(1, head);
            else if (enable.map_3 == true) model_save(2, head);
        }
        if (key == GLUT_KEY_F3)
        {
            model.add_car = true;
            push_car(head);
            if (enable.map_1 == true) pause(0, head, false);
            else if (enable.map_2 == true) pause(1, head, false);
            else if (enable.map_3 == true) pause(2, head, false);
        }
    }
}
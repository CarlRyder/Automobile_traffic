/* Copyright (c) Yakov D. Sendov & Salavat S. Babaev, 2022 */

#define _CRT_SECURE_NO_WARNINGS

#include "pincludes.h"
#include "defines.h"
#include "textures.h"
#include "check.h"

GLint Width = 800, Height = 800;

void model_save(int map);
void model_load();
void motorway();
void crossroad();
void mult_crossroad();
void display(void);
void reshape(GLint w, GLint h);
void mouse_pressed(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);
void special_keyboard(int key, int x, int y);
void struct_init();

int main()
{
    int argc = 1;
    char* argv[1] = { (char*)"1" };
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_NAME);
    struct_init();
    //FreeConsole();
    sndPlaySound(L"sound/Main.wav", SND_ASYNC);
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
    model.stop_time = 0;
}

Cars* head_car = NULL;

void model_save(int map)
{
    FILE* saves = fopen("map_save.txt", "w");
    fprintf(saves, "model settings:\n");
    fprintf(saves, "%d\n%d\n%d\n%f\n%d\n%d\n", map, model.line, model.car_counts, model.interval, model.autosave, model.time);
    Cars* temp = head_car;
    fprintf(saves, "ñars stats:\n");
    while (temp != NULL)
    {
        fprintf(saves, "%d %d %f %f %d %d %d %f\n", temp->car.stop, temp->car.direction, temp->car.x[0], temp->car.y[0], temp->car.line, temp->car.turn, temp->car.texture_id, temp->car.speed);
        temp = temp->next_car;
    }
    fclose(saves);
    pause(map, head_car, true);
}

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
        head_car = create_car();
        Cars* temp = head_car;
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

void motorway()
{
    enable.map_1 = true, enable.map_2 = false, enable.map_3 = false;
    sndPlaySound(L"sound/Sound.wav", SND_ASYNC);
    if (active.model_active == true)
    {
        if (head_car == NULL) head_car = create_car();
        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        map_show(0);
        glutSwapBuffers();
        clock_t start = clock(), end = 0, begin = clock();
        clock_t auto_save1 = clock(), auto_save2 = 0, stop_timer1 = 0, stop_timer2 = 0;
        int max_cars = 0, time = 0;
        bool flag_stop = false;
        if (model.time != 0) time = model.time;
        while (true)
        {
            if (GetAsyncKeyState(VK_SPACE) & 0x1) change_line(0, head_car);
            if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
            {
                active.model_active = false;
                active.pause_active = true;
                break;
            }
            if (GetAsyncKeyState(VK_F5) & 0x1)
            {
                stop_car(head_car);
                flag_stop = true;
                stop_timer1 = clock();
                stop_timer2 = 0;
            }
            if ((end - start) / CLOCKS_PER_SEC > model.interval)
            {
                push_car(head_car);
                start = clock();
                end = 0;
            }  
            if (model.autosave == true && (auto_save2 - auto_save1) / CLOCKS_PER_SEC > AUTOSAVE_INTERVAL)
            {
                model_save(0);
                auto_save1 = clock();
                auto_save2 = 0;
            }
            if ((stop_timer2 - stop_timer1) / CLOCKS_PER_SEC > STOP_CAR_INTERVAL)
            {
                start_car(head_car);
                flag_stop = false;
                stop_timer1 = 0;
                stop_timer2 = 0;
            }
            
            map_show(0);
            model.time = (int)((clock() - begin) / CLOCKS_PER_SEC) + time;
            info(max_cars, head_car);
            Cars* tmp = head_car, * ptr = NULL;
            while (tmp != NULL)
            {
                car_draw(&tmp->car);
                if (tmp->car.direction == 1)
                {
                    if (tmp->car.y[0] < 844) tmp->car.y[0] += tmp->car.speed / 3000;
                    else
                    {
                        if (tmp == head_car) head_car = delete_headcar(head_car);
                        else ptr->next_car = delete_car(tmp, head_car);
                        model.car_counts--;
                        break;
                    }
                }
                else if (tmp->car.direction == 2)
                {
                    if (tmp->car.y[0] > -44) tmp->car.y[0] -= tmp->car.speed / 3000;
                    else
                    {
                        if (tmp == head_car) head_car = delete_headcar(head_car);
                        else ptr->next_car = delete_car(tmp, head_car);
                        model.car_counts--;
                        break;
                    }
                }
                ptr = tmp;
                tmp = tmp->next_car;
            }
            check_distance(head_car);
            glutSwapBuffers();
            end = clock();
            auto_save2 = clock();
            if (flag_stop == true) stop_timer2 = clock();
            if (max_cars < model.car_counts) max_cars = model.car_counts;
        }
    }
    sndPlaySound(L"sound/null.wav", SND_ASYNC);
    if (active.pause_active == true) pause(0, head_car, false);
}

void crossroad()
{
    enable.map_1 = false, enable.map_2 = true, enable.map_3 = false;
    sndPlaySound(L"sound/Sound.wav", SND_ASYNC);
    if (active.model_active == true)
    {
        if (head_car == NULL) head_car = create_car();
        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        map_show(1);
        glutSwapBuffers();
        clock_t start = clock(), end = 0, begin = clock(), traffic1 = clock(), traffic2 = 0;
        clock_t auto_save1 = clock(), auto_save2 = 0, stop_timer1 = 0, stop_timer2 = 0;
        int max_cars = 0, time = 0;
        bool flag_stop = false;
        if (model.time != 0) time = model.time;
        while (true)
        {
            if (GetAsyncKeyState(VK_SPACE) & 0x1) change_line(1, head_car);
            if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
            {
                active.model_active = false;
                active.pause_active = true;
                break;
            }
            if (GetAsyncKeyState(VK_F5) & 0x1)
            {
                stop_car(head_car);
                flag_stop = true;
                stop_timer1 = clock();
                stop_timer2 = 0;
            }
            if ((end - start) / CLOCKS_PER_SEC > model.interval)
            {
                push_car(head_car);
                start = clock();
                end = 0;
            }
            if ((traffic2 - traffic1) / CLOCKS_PER_SEC > TRAFFIC_LIGHT_INTERVAL)
            {
                if (model.traffic_light1 == true) model.traffic_light1 = false, model.traffic_light2 = true;
                else model.traffic_light1 = true, model.traffic_light2 = false;
                traffic1 = clock();
                traffic2 = 0;
            }
            if (model.autosave == true && (auto_save2 - auto_save1) / CLOCKS_PER_SEC > AUTOSAVE_INTERVAL)
            {
                model_save(1);
                auto_save1 = clock();
                auto_save2 = 0;
            }
            if ((stop_timer2 - stop_timer1) / CLOCKS_PER_SEC > STOP_CAR_INTERVAL)
            {
                start_car(head_car);
                flag_stop = false;
                stop_timer1 = 0;
                stop_timer2 = 0;
            }
            map_show(1);
            traffic_lights();
            model.time = (int)((clock() - begin) / CLOCKS_PER_SEC) + time;
            info(max_cars, head_car);
            Cars* tmp = head_car, * ptr = NULL;
            while (tmp != NULL)
            {
                car_draw(&tmp->car);
                if (tmp->car.direction == 1)
                {
                    if (tmp->car.y[0] < 844) tmp->car.y[0] += tmp->car.speed / 3000;
                    else
                    {
                        if (tmp == head_car) head_car = delete_headcar(head_car);
                        else ptr->next_car = delete_car(tmp, head_car);
                        model.car_counts--;
                        break;
                    }
                }
                else if (tmp->car.direction == 2)
                {
                    if (tmp->car.y[0] > -44) tmp->car.y[0] -= tmp->car.speed / 3000;
                    else
                    {
                        if (tmp == head_car) head_car = delete_headcar(head_car);
                        else ptr->next_car = delete_car(tmp, head_car);
                        model.car_counts--;
                        break;
                    }
                }
                else if (tmp->car.direction == 3)
                {
                    if (tmp->car.x[0] > -44) tmp->car.x[0] -= tmp->car.speed / 3000;
                    else
                    {
                        if (tmp == head_car) head_car = delete_headcar(head_car);
                        else ptr->next_car = delete_car(tmp, head_car);
                        model.car_counts--;
                        break;
                    }
                }
                else if (tmp->car.direction == 4)
                {
                    if (tmp->car.x[0] < 844) tmp->car.x[0] += tmp->car.speed / 3000;
                    else
                    {
                        if (tmp == head_car) head_car = delete_headcar(head_car);
                        else ptr->next_car = delete_car(tmp, head_car);
                        model.car_counts--;
                        break;
                    }
                }
                ptr = tmp;
                tmp = tmp->next_car;
            }
            check_light(head_car);
            check_cars(head_car);
            check_distance(head_car);
            glutSwapBuffers();
            end = clock();
            traffic2 = clock();
            auto_save2 = clock();
            if (flag_stop == true) stop_timer2 = clock();
            if (max_cars < model.car_counts) max_cars = model.car_counts;
        }
    }
    sndPlaySound(L"sound/null.wav", SND_ASYNC);
    if (active.pause_active == true) pause(1, head_car, false);
}

void mult_crossroad()
{
    enable.map_1 = false, enable.map_2 = false, enable.map_3 = true;
    sndPlaySound(L"sound/Sound.wav", SND_ASYNC);
    if (active.model_active == true)
    {
        if (head_car == NULL) head_car = create_car();
        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        map_show(2);
        glutSwapBuffers();
        clock_t start = clock(), end = 0, begin = clock(), traffic1 = clock(), traffic2 = 0;
        clock_t auto_save1 = clock(), auto_save2 = 0, stop_timer1 = 0, stop_timer2 = 0;;
        int max_cars = 0, time = 0, save_dir = 0, save_line = 0, stop_time = 0;
        float save_x = 0, save_y = 0;
        bool flag_stop = false;
        if (model.time != 0) time = model.time;
        while (true)
        {
            if (GetAsyncKeyState(VK_SPACE) & 0x1) change_line(2, head_car);
            if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
            {
                active.model_active = false;
                active.pause_active = true;
                break;
            }
            if (GetAsyncKeyState(VK_F5) & 0x1)
            {
                stop_car(head_car);
                flag_stop = true;
                stop_timer1 = clock();
                stop_timer2 = 0;
            }
            if ((traffic2 - traffic1) / CLOCKS_PER_SEC > TRAFFIC_LIGHT_INTERVAL)
            {
                if (model.traffic_light1 == true) model.traffic_light1 = false, model.traffic_light2 = true;
                else model.traffic_light1 = true, model.traffic_light2 = false;
                traffic1 = clock();
                traffic2 = 0;
            }
            if ((end - start) / CLOCKS_PER_SEC > model.interval)
            {
                push_car(head_car);
                start = clock();
                end = 0;
            }
            if (model.autosave == true && (auto_save2 - auto_save1) / CLOCKS_PER_SEC > AUTOSAVE_INTERVAL)
            {
                model_save(2);
                auto_save1 = clock();
                auto_save2 = 0;
            }
            if ((stop_timer2 - stop_timer1) / CLOCKS_PER_SEC > STOP_CAR_INTERVAL)
            {
                start_car(head_car);
                flag_stop = false;
                stop_timer1 = 0;
                stop_timer2 = 0;
            }
            map_show(2);
            mult_traffic_lights();
            model.time = (int)((clock() - begin) / CLOCKS_PER_SEC) + time;
            info(max_cars, head_car);
            Cars* tmp = head_car, * ptr = NULL;
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
                        if (tmp == head_car) head_car = delete_headcar(head_car);
                        else ptr->next_car = delete_car(tmp, head_car);
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
                        if (tmp == head_car) head_car = delete_headcar(head_car);
                        else ptr->next_car = delete_car(tmp, head_car);
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
                        if (tmp == head_car) head_car = delete_headcar(head_car);
                        else ptr->next_car = delete_car(tmp, head_car);
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
                        if (tmp == head_car) head_car = delete_headcar(head_car);
                        else ptr->next_car = delete_car(tmp, head_car);
                        model.car_counts--;
                        break;
                    }
                }
                ptr = tmp;
                tmp = tmp->next_car;
            }
            mult_check_light(head_car);
            check_distance(head_car);
            check_cars(head_car);
            glutSwapBuffers();
            end = clock();
            traffic2 = clock();
            auto_save2 = clock();
            if (flag_stop == true) stop_timer2 = clock();
            if (max_cars < model.car_counts) max_cars = model.car_counts;
        }
    }
    sndPlaySound(L"sound/null.wav", SND_ASYNC);
    if (active.pause_active == true) pause(2, head_car, false);
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
                sndPlaySound(L"sound/null.wav", SND_ASYNC);
                model.time = 0;
                model.line = 1;
                buttons.map_button = false;
                active.model_active = true;
                motorway();
            }
            else if (x >= MENU_BUTTON_LEFT && x <= MENU_BUTTON_RIGHT && y >= 350 && y <= 400)
            {
                sndPlaySound(L"sound/null.wav", SND_ASYNC);
                model.time = 0;
                model.line = 1;
                buttons.map_button = false;
                active.model_active = true;
                crossroad();
            }
            else if (x >= MENU_BUTTON_LEFT && x <= MENU_BUTTON_RIGHT && y >= 425 && y <= 475)
            {
                sndPlaySound(L"sound/null.wav", SND_ASYNC);
                model.time = 0;
                model.line = 1;
                buttons.map_button = false;
                active.model_active = true;
                mult_crossroad();
            }
            else if (x >= MENU_BUTTON_LEFT && x <= MENU_BUTTON_RIGHT && y >= 500 && y <= 550)
            {
                sndPlaySound(L"sound/null.wav", SND_ASYNC);
                model.time = 0;
                model.line = 1;
                buttons.map_button = false;
                model_load(head_car);
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
            head_car = remove_all(head_car);
            model.car_counts = 0;
            active.pause_active = false;
            sndPlaySound(L"sound/Main.wav", SND_ASYNC);
            display();
        }
        if (key == GLUT_KEY_F2)
        {
            if (enable.map_1 == true) model_save(0);
            else if (enable.map_2 == true) model_save(1);
            else if (enable.map_3 == true) model_save(2);
        }
        if (key == GLUT_KEY_F3)
        {
            model.add_car = true;
            push_car(head_car);
            if (enable.map_1 == true) pause(0, head_car, false);
            else if (enable.map_2 == true) pause(1, head_car, false);
            else if (enable.map_3 == true) pause(2, head_car, false);
        }
    }
}
//#define _CRT_SECURE_NO_WARNINGS
//
//#include "maps.h"
//#include "defines.h"
//
//void model_save(int map)
//{
//    FILE* saves = fopen("map_save.txt", "w");
//    fprintf(saves, "model settings:\n");
//    fprintf(saves, "%d\n%d\n%d\n%f\n%d\n%d\n", map, model.line, model.car_counts, model.interval, model.autosave, model.time);
//    Cars* temp = head_car;
//    fprintf(saves, "ñars stats:\n");
//    while (temp != NULL)
//    {
//        fprintf(saves, "%d %d %f %f %d %d %d %f\n", temp->car.stop, temp->car.direction, temp->car.x[0], temp->car.y[0], temp->car.line, temp->car.turn, temp->car.texture_id, temp->car.speed);
//        temp = temp->next_car;
//    }
//    fclose(saves);
//    pause(map, head_car, true);
//}
//
//void model_load()
//{
//    FILE* saves = fopen("map_save.txt", "r");
//    if (saves == NULL)
//    {
//        active.load_error = true;
//        glutPostRedisplay();
//    }
//    else
//    {
//        char str[100] = { 0 };
//        int counter = 0, map = -1;
//        head_car = create_car();
//        Cars* temp = head_car;
//        while (fgets(str, sizeof(str), saves) != NULL)
//        {
//            counter++;
//            str[strcspn(str, "\n")] = 0;
//            if (strcmp(str, "model settings:") == 0) continue;
//            if (counter == 2) map = atoi(str);
//            else if (counter == 3) model.line = atoi(str);
//            else if (counter == 4) model.car_counts = atoi(str);
//            else if (counter == 5) model.interval = (float)atof(str);
//            else if (counter == 6) model.autosave = atoi(str);
//            else if (counter == 7) model.time = atoi(str);
//            if (strcmp(str, "ñars stats:") == 0) continue;
//            if (counter > 8)
//            {
//                int save_i = 0, count = 0;
//                temp->car.stop = str[0] - '0';
//                temp->car.direction = str[2] - '0';
//                char value[11] = { 0 };
//                for (unsigned int i = 4; i < strlen(str); i++)
//                {
//                    save_i = i;
//                    if (str[i] == ' ') break;
//                    if (count < 10) value[count] = str[i];
//                    count++;
//                }
//                value[strlen(value)] = '\0';
//                temp->car.x[0] = (float)atof(value);
//                temp->car.x[1] = temp->car.x[0] + CAR_WIDTH;
//                memset(value, 0, sizeof(value));
//                count = 0;
//                for (unsigned int i = save_i + 1; i < strlen(str); i++)
//                {
//                    save_i = i;
//                    if (str[i] == ' ') break;
//                    if (count < 10) value[count] = str[i];
//                    count++;
//                }
//                value[strlen(value)] = '\0';
//                temp->car.y[0] = (float)atof(value);
//                temp->car.y[1] = temp->car.y[0] + CAR_HEIGHT;
//                memset(value, 0, sizeof(value));
//                temp->car.line = str[save_i + 1] - '0';
//                save_i += 3;
//                temp->car.turn = str[save_i] - '0';
//                count = 0;
//                for (unsigned int i = save_i + 2; i < strlen(str); i++)
//                {
//                    save_i = i;
//                    if (str[i] == ' ') break;
//                    if (count < 10) value[count] = str[i];
//                    count++;
//                }
//                value[strlen(value)] = '\0';
//                temp->car.texture_id = atoi(value);
//                memset(value, 0, sizeof(value));
//                count = 0;
//                for (unsigned int i = save_i + 1; i < strlen(str); i++)
//                {
//                    if (count < 10) value[count] = str[i];
//                    count++;
//                }
//                value[strlen(value)] = '\0';
//                temp->car.speed = (float)atof(value);
//                memset(value, 0, sizeof(value));
//                temp->next_car = create_car();
//                temp = temp->next_car;
//            }
//            memset(str, 0, sizeof(str));
//        }
//        fclose(saves);
//        active.model_active = true;
//        if (map == 0) motorway();
//        else if (map == 1) crossroad();
//        else if (map == 2) mult_crossroad();
//    }
//}
//
//void motorway()
//{
//    enable.map_1 = true, enable.map_2 = false, enable.map_3 = false;
//    if (active.model_active == true)
//    {
//        if (head_car == NULL) head_car = create_car();
//        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
//        glClear(GL_COLOR_BUFFER_BIT);
//        map_show(0);
//        glutSwapBuffers();
//        clock_t start = clock(), end = 0, begin = clock();
//        clock_t auto_save1 = clock(), auto_save2 = 0;
//        int max_cars = 0, time = 0;
//        if (model.time != 0) time = model.time;
//        while (true)
//        {
//            if (GetAsyncKeyState(VK_SPACE) & 0x1) change_line(0, head_car);
//            if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
//            {
//                active.model_active = false;
//                active.pause_active = true;
//                break;
//            }
//            if (GetAsyncKeyState(VK_F5) & 0x1) stop_car(head_car);
//            if ((end - start) / CLOCKS_PER_SEC > model.interval)
//            {
//                push_car(head_car);
//                start = clock();
//                end = 0;
//            }
//            if (model.autosave == true && (auto_save2 - auto_save1) / CLOCKS_PER_SEC > 30)
//            {
//                model_save(0);
//                auto_save1 = clock();
//                auto_save2 = 0;
//            }
//            map_show(0);
//            model.time = (int)((clock() - begin) / CLOCKS_PER_SEC) + time;
//            info(max_cars, head_car);
//            Cars* tmp = head_car, * ptr = NULL;
//            while (tmp != NULL)
//            {
//                car_draw(&tmp->car);
//                if (tmp->car.direction == 1)
//                {
//                    if (tmp->car.y[0] < 844) tmp->car.y[0] += tmp->car.speed / 3000;
//                    else
//                    {
//                        if (tmp == head_car) head_car = delete_headcar(head_car);
//                        else ptr->next_car = delete_car(tmp, head_car);
//                        model.car_counts--;
//                        break;
//                    }
//                }
//                else if (tmp->car.direction == 2)
//                {
//                    if (tmp->car.y[0] > -44) tmp->car.y[0] -= tmp->car.speed / 3000;
//                    else
//                    {
//                        if (tmp == head_car) head_car = delete_headcar(head_car);
//                        else ptr->next_car = delete_car(tmp, head_car);
//                        model.car_counts--;
//                        break;
//                    }
//                }
//                ptr = tmp;
//                tmp = tmp->next_car;
//            }
//            check_distance(head_car);
//            glutSwapBuffers();
//            end = clock();
//            auto_save2 = clock();
//            if (max_cars < model.car_counts) max_cars = model.car_counts;
//        }
//    }
//    if (active.pause_active == true) pause(0, head_car, false);
//}
//
//void crossroad()
//{
//    enable.map_1 = false, enable.map_2 = true, enable.map_3 = false;
//    if (active.model_active == true)
//    {
//        if (head_car == NULL) head_car = create_car();
//        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
//        glClear(GL_COLOR_BUFFER_BIT);
//        map_show(1);
//        glutSwapBuffers();
//        clock_t start = clock(), end = 0, begin = clock(), traffic1 = clock(), traffic2 = 0;
//        clock_t auto_save1 = clock(), auto_save2 = 0;
//        int max_cars = 0, time = 0;
//        if (model.time != 0) time = model.time;
//        while (true)
//        {
//            if (GetAsyncKeyState(VK_SPACE) & 0x1) change_line(1, head_car);
//            if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
//            {
//                active.model_active = false;
//                active.pause_active = true;
//                break;
//            }
//            if (GetAsyncKeyState(VK_F5) & 0x1)
//            {
//                stop_car(head_car);
//            }
//            if ((end - start) / CLOCKS_PER_SEC > model.interval)
//            {
//                push_car(head_car);
//                start = clock();
//                end = 0;
//            }
//            if ((traffic2 - traffic1) / CLOCKS_PER_SEC > 15)
//            {
//                if (model.traffic_light1 == true) model.traffic_light1 = false, model.traffic_light2 = true;
//                else model.traffic_light1 = true, model.traffic_light2 = false;
//                traffic1 = clock();
//                traffic2 = 0;
//            }
//            if (model.autosave == true && (auto_save2 - auto_save1) / CLOCKS_PER_SEC > 30)
//            {
//                model_save(1);
//                auto_save1 = clock();
//                auto_save2 = 0;
//            }
//            map_show(1);
//            traffic_lights();
//            model.time = (int)((clock() - begin) / CLOCKS_PER_SEC) + time;
//            info(max_cars, head_car);
//            Cars* tmp = head_car, * ptr = NULL;
//            while (tmp != NULL)
//            {
//                car_draw(&tmp->car);
//                if (tmp->car.direction == 1)
//                {
//                    if (tmp->car.y[0] < 844) tmp->car.y[0] += tmp->car.speed / 3000;
//                    else
//                    {
//                        if (tmp == head_car) head_car = delete_headcar(head_car);
//                        else ptr->next_car = delete_car(tmp, head_car);
//                        model.car_counts--;
//                        break;
//                    }
//                }
//                else if (tmp->car.direction == 2)
//                {
//                    if (tmp->car.y[0] > -44) tmp->car.y[0] -= tmp->car.speed / 3000;
//                    else
//                    {
//                        if (tmp == head_car) head_car = delete_headcar(head_car);
//                        else ptr->next_car = delete_car(tmp, head_car);
//                        model.car_counts--;
//                        break;
//                    }
//                }
//                else if (tmp->car.direction == 3)
//                {
//                    if (tmp->car.x[0] > -44) tmp->car.x[0] -= tmp->car.speed / 3000;
//                    else
//                    {
//                        if (tmp == head_car) head_car = delete_headcar(head_car);
//                        else ptr->next_car = delete_car(tmp, head_car);
//                        model.car_counts--;
//                        break;
//                    }
//                }
//                else if (tmp->car.direction == 4)
//                {
//                    if (tmp->car.x[0] < 844) tmp->car.x[0] += tmp->car.speed / 3000;
//                    else
//                    {
//                        if (tmp == head_car) head_car = delete_headcar(head_car);
//                        else ptr->next_car = delete_car(tmp, head_car);
//                        model.car_counts--;
//                        break;
//                    }
//                }
//                ptr = tmp;
//                tmp = tmp->next_car;
//            }
//            check_light(head_car);
//            check_cars(head_car);
//            check_distance(head_car);
//            glutSwapBuffers();
//            end = clock();
//            traffic2 = clock();
//            auto_save2 = clock();
//            if (max_cars < model.car_counts) max_cars = model.car_counts;
//        }
//    }
//    if (active.pause_active == true) pause(1, head_car, false);
//}
//
//void mult_crossroad()
//{
//    enable.map_1 = false, enable.map_2 = false, enable.map_3 = true;
//    if (active.model_active == true)
//    {
//        if (head_car == NULL) head_car = create_car();
//        glClearColor(COLOR_MENU_RED, COLOR_MENU_GREEN, COLOR_MENU_BLUE, 0);
//        glClear(GL_COLOR_BUFFER_BIT);
//        map_show(2);
//        glutSwapBuffers();
//        clock_t start = clock(), end = 0, begin = clock(), traffic1 = clock(), traffic2 = 0;
//        clock_t auto_save1 = clock(), auto_save2 = 0;
//        int max_cars = 0, time = 0, save_dir = 0, save_line = 0;
//        float save_x = 0, save_y = 0;
//        if (model.time != 0) time = model.time;
//        while (true)
//        {
//            if (GetAsyncKeyState(VK_SPACE) & 0x1) change_line(2, head_car);
//            if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
//            {
//                active.model_active = false;
//                active.pause_active = true;
//                break;
//            }
//            if (GetAsyncKeyState(VK_F5) & 0x1)
//            {
//                stop_car(head_car);
//            }
//            if ((traffic2 - traffic1) / CLOCKS_PER_SEC > 15)
//            {
//                if (model.traffic_light1 == true) model.traffic_light1 = false, model.traffic_light2 = true;
//                else model.traffic_light1 = true, model.traffic_light2 = false;
//                traffic1 = clock();
//                traffic2 = 0;
//            }
//            if ((end - start) / CLOCKS_PER_SEC > model.interval)
//            {
//                push_car(head_car);
//                start = clock();
//                end = 0;
//            }
//            map_show(2);
//            mult_traffic_lights();
//            model.time = (int)((clock() - begin) / CLOCKS_PER_SEC) + time;
//            info(max_cars, head_car);
//            Cars* tmp = head_car, * ptr = NULL;
//            while (tmp != NULL)
//            {
//                car_draw(&tmp->car);
//                if (tmp->car.direction == 1)
//                {
//                    save_dir = tmp->car.direction;
//                    save_line = tmp->car.line;
//                    save_y = tmp->car.y[0];
//                    if (tmp->car.y[0] < 844) tmp->car.y[0] += tmp->car.speed / 3000;
//                    else
//                    {
//                        if (tmp == head_car) head_car = delete_headcar(head_car);
//                        else ptr->next_car = delete_car(tmp, head_car);
//                        model.car_counts--;
//                        break;
//                    }
//                }
//                else if (tmp->car.direction == 2)
//                {
//                    save_dir = tmp->car.direction;
//                    save_line = tmp->car.line;
//                    save_y = tmp->car.y[0];
//                    if (tmp->car.y[0] > -44) tmp->car.y[0] -= tmp->car.speed / 3000;
//                    else
//                    {
//                        if (tmp == head_car) head_car = delete_headcar(head_car);
//                        else ptr->next_car = delete_car(tmp, head_car);
//                        model.car_counts--;
//                        break;
//                    }
//                }
//                else if (tmp->car.direction == 3 || tmp->car.direction == 5)
//                {
//                    save_dir = tmp->car.direction;
//                    save_line = tmp->car.line;
//                    save_x = tmp->car.x[0];
//                    if (tmp->car.x[0] > -44) tmp->car.x[0] -= tmp->car.speed / 3000;
//                    else
//                    {
//                        if (tmp == head_car) head_car = delete_headcar(head_car);
//                        else ptr->next_car = delete_car(tmp, head_car);
//                        model.car_counts--;
//                        break;
//                    }
//                }
//                else if (tmp->car.direction == 4 || tmp->car.direction == 6)
//                {
//                    save_dir = tmp->car.direction;
//                    save_line = tmp->car.line;
//                    save_x = tmp->car.x[0];
//                    if (tmp->car.x[0] < 844) tmp->car.x[0] += tmp->car.speed / 3000;
//                    else
//                    {
//                        if (tmp == head_car) head_car = delete_headcar(head_car);
//                        else ptr->next_car = delete_car(tmp, head_car);
//                        model.car_counts--;
//                        break;
//                    }
//                }
//                ptr = tmp;
//                tmp = tmp->next_car;
//            }
//            mult_check_light(head_car);
//            check_distance(head_car);
//            check_cars(head_car);
//            glutSwapBuffers();
//            end = clock();
//            traffic2 = clock();
//            if (max_cars < model.car_counts) max_cars = model.car_counts;
//        }
//    }
//    if (active.pause_active == true) pause(2, head_car, false);
//}
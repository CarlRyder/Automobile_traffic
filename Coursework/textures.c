#define _CRT_SECURE_NO_WARNINGS

#include "textures.h"
#include "defines.h"

#define INDEX_FIRST 16
#define INDEX_SECOND 17
#define CAR_DATA_SIZE 3168
#define MAP_DATA_SIZE 1920000

void get_car_texture()
{
    int car_count = 0;
    char path[] = "car_texture/car_00.txt";
    while (car_count < TOTAL_CARS)
    {
        car_count++;
        path[INDEX_SECOND]++;
        if (car_count == 10)
        {
            path[INDEX_FIRST] = '1';
            path[INDEX_SECOND] = '0';
        }
        if (car_count == 20)
        {
            path[INDEX_FIRST] = '2';
            path[INDEX_SECOND] = '0';
        }
        unsigned char* data = (unsigned char*)malloc(CAR_DATA_SIZE);
        if (data != NULL)
        {
            FILE* texture_file = fopen(path, "rb");
            if (texture_file == NULL)
            {
                printf("There is no files with cars textures in the directory!\n");
                exit(0);
            }
            unsigned char part[2] = { 0 };
            for (unsigned char* p = &data[0]; p < &data[CAR_DATA_SIZE]; p++)
            {
                fgets(part, 2, texture_file);
                *p = *part;
            }
            fclose(texture_file);
            glGenTextures(1, &car_tex.texture[car_count - 1]);
            glBindTexture(GL_TEXTURE_2D, car_tex.texture[car_count - 1]);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 24, 44, GL_RGB, GL_UNSIGNED_BYTE, data);
            free(data);
        }
        else
        {
            printf("Error: Out of memory!\n");
            exit(DEFAULT_ERROR);
        }
    }
}

GLuint get_map_texture(char* filename)
{
    GLuint texture = 0;
    unsigned char* data = (unsigned char*)malloc(MAP_DATA_SIZE);
    if (data != NULL)
    {
        FILE* texture_file = fopen(filename, "rb");
        if (texture_file == NULL)
        {
            printf("There is no file with map textures in the directory!\n");
            exit(0);
        }
        unsigned char part[2] = { 0 };
        for (unsigned char* p = &data[0]; p < &data[MAP_DATA_SIZE]; p++)
        {
            fgets(part, 2, texture_file);
            *p = *part;
        }
        fclose(texture_file);
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, data);
        free(data);
        return texture;
    }
    else
    {
        printf("Error: Out of memory!\n");
        exit(DEFAULT_ERROR);
    }
}
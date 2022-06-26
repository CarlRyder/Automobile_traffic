#define _CRT_SECURE_NO_WARNINGS

#include "textures.h"
#include "defines.h"

#define INDEX_FIRST 16
#define INDEX_SECOND 17

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
        int width = 24, height = 44, num_color = 3;
        unsigned char* data = (unsigned char*)malloc(width * height * num_color);
        if (data != NULL)
        {
            FILE* texture_file = fopen(path, "rb");
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
    int num_color = 3;
    unsigned char* data = (unsigned char*)malloc(WINDOW_WIDTH * WINDOW_HEIGHT * num_color * sizeof(unsigned char));
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
        while (i < WINDOW_WIDTH * WINDOW_HEIGHT * num_color)
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
#pragma once
#include "pincludes.h"

struct textures
{
    unsigned int texture[20];
} car_tex, map_tex;

void get_car_texture();
GLuint get_map_texture(char* filename);
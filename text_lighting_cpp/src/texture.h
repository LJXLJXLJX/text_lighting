#pragma once
#include "stb_image.h"
#include <glad/glad.h>

unsigned int textureFromFile(const char* img_file,int& height,int& width);
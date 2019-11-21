#pragma once
#include "stb_image.h"
#include <glad/glad.h>
#include <iostream>


unsigned char* rawGray2RGB(unsigned char* data, unsigned int size);

unsigned int textureFromFile(const char* img_file);

#include "texture.h"

unsigned char* rawGray2RGB(unsigned char * data, unsigned int size)
{
	unsigned char* ret = (unsigned char*)malloc(size * 3 * sizeof(unsigned char));
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < 3; ++j) {
			ret[3 * i + j] = data[i];
		}
	}
	return ret;
}

unsigned int textureFromFile(const char * img_file)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载并生成纹理
	int width;
	int height;
	int nrChannels;
	unsigned char *data = stbi_load(img_file, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (nrChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (nrChannels == 1) {
			unsigned char* rgb_data = rawGray2RGB(data, width*height*nrChannels);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb_data);
			delete rgb_data;
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}

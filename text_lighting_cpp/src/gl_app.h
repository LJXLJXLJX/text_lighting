#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <functional>
#include <list>

#include "text_mesh.h"

class GlApp {
public:
	GlApp();
	~GlApp();
	
	int createWindow(int width, int height,const GLchar* title="gl app");
	
	int setViewport(int width, int height);

	//For debugging
	int renderLoop(std::list<std::function<void()>>&func_list);

	void setShader(Shader* shader);

	void setTextMesh(TextMesh* tm);

	void generateFrameBuffer();

private:
	int initGlfw();
	int initGlad();


	Shader* m_shader;
	TextMesh* m_tm;
	unsigned int m_framebuffer;

	//For debugging
	GLFWwindow* m_window;
};
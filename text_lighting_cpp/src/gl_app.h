#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <functional>
#include <list>
#include <string>

#include "text_mesh.h"
#include "quad_mesh.h"

class GlApp {

public:
	unsigned int m_framebuffer;
	unsigned int m_texture_colorbuffer;

	std::shared_ptr<Shader> m_text_shader;
	std::shared_ptr<Shader> m_quad_shader;
	std::shared_ptr<TextMesh> m_tm;
	std::shared_ptr<QuadMesh> m_qm;

public:
	GlApp();
	~GlApp();

	int createWindow(int width, int height, const GLchar* title = "gl app");

	int setViewport(int width, int height);

	void drawFbo();

	void fboToFile(std::string path);

	void drawQuad();

	void renderOnce(std::string img_path);

	//For debugging
	int DebugRenderLoop();

	void generateFrameBuffer();

private:
	int initGlfw();

	int initGlad();

	//For debugging
	GLFWwindow* m_window;
};
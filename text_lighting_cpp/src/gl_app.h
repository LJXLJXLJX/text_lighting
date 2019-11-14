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

	//For debugging
	int DebugRenderLoop(std::string img_path);

	void generateFrameBuffer();

	void run(std::string src_dir, std::string dest_dir, int num_for_each);

private:
	int initGlfw();

	int initGlad();

	void generateTexture(std::string img_path);

	void drawFbo();

	void fboToFile(std::string path);

	void drawQuad();

	// 将当前的纹理绘制一次到fbo
	void renderOnce();

	void deleteTexture();

	//For debugging
	GLFWwindow* m_window;

	unsigned int m_texture_id;
};
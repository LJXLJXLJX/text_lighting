#include "gl_app.h"

#include <iostream>

using namespace std;

GlApp::GlApp()
{
	initGlfw();
}

GlApp::~GlApp()
{
	glfwTerminate();
}

int GlApp::initGlfw()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	return 0;
}

int GlApp::createWindow(int width, int height, const GLchar * title)
{
	m_window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (m_window == nullptr) {
		cout << "Failed to create window!" << endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(m_window);
	initGlad();
	glViewport(0, 0, width, height);
	return 0;
}

int GlApp::initGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	return 0;
}

int GlApp::setViewport(int width, int height)
{
	glViewport(0, 0, width, height);
	return 0;
}

int GlApp::renderLoop(std::list<std::function<void()>>&func_list)
{
	while (!glfwWindowShouldClose(m_window)) {

		glClear(GL_COLOR_BUFFER_BIT);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (!func_list.empty()) {
			for (auto func : func_list)
				func();
		}

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
	return 0;
}

void GlApp::setShader(Shader * shader)
{
	m_shader = shader;
}

void GlApp::setTextMesh(TextMesh * tm)
{
	m_tm = tm;
}

void GlApp::generateFrameBuffer()
{
	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	// 生成纹理
	unsigned int tex_color_buffer;
	glGenTextures(1, &tex_color_buffer);
	glBindTexture(GL_TEXTURE_2D, tex_color_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 280, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	//附加到当前绑定的帧缓冲对象
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, tex_color_buffer, 0);

	//创建渲染缓冲对象
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 280, 32);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

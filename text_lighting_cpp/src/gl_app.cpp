#include "gl_app.h"

#include <iostream>

using namespace std;



GlApp::GlApp()
{
	m_tm = make_shared<TextMesh>();
	m_qm = make_shared<QuadMesh>();
	m_text_shader = make_shared<Shader>();
	m_quad_shader = make_shared<Shader>();
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

int GlApp::renderLoop()
{
	while (!glfwWindowShouldClose(m_window)) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_tm->draw(m_text_shader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glEnable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_qm->draw(m_quad_shader);
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
	return 0;
}


void GlApp::generateFrameBuffer()
{
	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	// 生成纹理
	glGenTextures(1, &m_texture_colorbuffer);
	glBindTexture(GL_TEXTURE_2D, m_texture_colorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 280, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	//附加到当前绑定的帧缓冲对象
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, m_texture_colorbuffer, 0);

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



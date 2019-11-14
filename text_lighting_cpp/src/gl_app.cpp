#include "gl_app.h"
#include "stb_image_write.h"
#include "texture.h"
#include "light.h"

#include <iostream>
#include <random>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;


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
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
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

void GlApp::generateTexture(std::string img_path)
{
	int height, width;
	m_texture_id = textureFromFile(img_path.c_str(), height, width);
	setViewport(width, height);
}

int GlApp::setViewport(int width, int height)
{
	glViewport(0, 0, width, height);
	return 0;
}

void GlApp::drawFbo()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_tm->draw(m_text_shader);
}

void GlApp::fboToFile(std::string path)
{
	GLubyte* pixels = (GLubyte*)malloc(280 * 32 * 3 * sizeof(GLubyte));
	glReadPixels(0, 0, 280, 32, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	stbi_flip_vertically_on_write(true);
	stbi_write_jpg(path.c_str(), 280, 32, 3, pixels, 100);
}

void GlApp::drawQuad()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_qm->draw(m_quad_shader);
}

// 渲染一次
void GlApp::renderOnce()
{
	m_tm->setTexture(m_text_shader, m_texture_id);

	m_text_shader->setInt("material.diffuse", m_texture_id);
	m_text_shader->setInt("material.specular", m_texture_id);
	m_text_shader->setFloat("material.shininess", 32.0f);

	PointLight pl;

	random_device rd{};
	uniform_real_distribution<float> dist(-2.0f, 2.0f);
	float x = dist(rd);
	float y = dist(rd);
	dist = uniform_real_distribution<float>(0.5f, 2.0f);
	float z = dist(rd);
	pl.position = { x,y,z };
	dist = uniform_real_distribution<float>(0.1f, 0.4f);
	float ambient_v = dist(rd);
	pl.ambient = { ambient_v,ambient_v, ambient_v };
	dist = uniform_real_distribution<float>(0.4f, 0.7f);
	float diffuse_v = dist(rd);
	pl.diffuse = { diffuse_v, diffuse_v,diffuse_v };
	dist = uniform_real_distribution<float>(0.5f, 1.0f);
	float specular_v = dist(rd);
	pl.specular = { specular_v,specular_v, specular_v };
	pl._near = 0.5f;
	pl._far = 10.0f;

	m_text_shader->setVec3("point_light.position", pl.position);
	m_text_shader->setVec3("point_light.ambient", pl.ambient);
	m_text_shader->setVec3("point_light.diffuse", pl.diffuse);
	m_text_shader->setVec3("point_light.specular", pl.specular);
	m_text_shader->setFloat("point_light.near", pl._near);
	m_text_shader->setFloat("point_light.far", pl._far);

	m_text_shader->setVec3("obj_color", { 1.0f,0.0f,0.0f });

	m_text_shader->setBool("is_light", false);
	m_text_shader->setVec3("view_pos", { 0.0f,0.0f,1.0f });

	m_qm->setTexture(m_quad_shader, m_texture_colorbuffer);
	m_quad_shader->setInt("screen_texture", m_texture_colorbuffer);

	drawFbo();
		
}

void GlApp::deleteTexture()
{
	glDeleteTextures(1, &m_texture_id);
}

int GlApp::DebugRenderLoop(std::string img_path)
{
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	generateTexture("../img/1.jpg");
	while (!glfwWindowShouldClose(m_window)) {
		renderOnce();
		drawQuad();
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
	deleteTexture();
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

void GlApp::run(std::string src_dir, std::string dest_dir, int num_for_each)
{
	fs::path path_dest_dir(dest_dir);
	if (!fs::exists(path_dest_dir))
		fs::create_directory(path_dest_dir);
	cout << path_dest_dir.string() << endl;
	for (auto& de : fs::directory_iterator(src_dir)) {
		string file_path_str = de.path().string();
		generateTexture(file_path_str);
		string filename = de.path().filename().string();
		for (int i = 0; i < num_for_each; ++i) {
			renderOnce();
			string dest_file_name = "light_" + to_string(i) + "_" + filename;
			string dest_path_str = (path_dest_dir/dest_file_name).string();
			fboToFile(dest_path_str);
		}
		deleteTexture();
	}
}



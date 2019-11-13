#include "gl_app.h"
#include "texture.h"
#include "light.h"
#include <memory>

using namespace std;


int main()
{
	shared_ptr<GlApp> app = make_shared<GlApp>();
	app->createWindow(280, 32, "test");
	app->generateFrameBuffer();

	app->m_text_shader->initFromFile("shaders/vs.glsl", "shaders/fs_light.glsl");
	app->m_quad_shader->initFromFile("shaders/quad_vs.glsl", "shaders/quad_fs.glsl");
	
	app->m_tm->init();
	app->m_qm->init();

	int height, width;
	unsigned int texture_id = textureFromFile("./img/1.jpg", height, width);
	app->setViewport(width, height);
	app->m_tm->setTexture(app->m_text_shader, texture_id);

	app->m_text_shader->setInt("material.diffuse", texture_id);
	app->m_text_shader->setInt("material.specular", texture_id);
	app->m_text_shader->setFloat("material.shininess", 32.0f);

	PointLight pl;
	pl.position = { 0.0f,0.0f,1.0f };
	pl.ambient = { 0.2f, 0.2f, 0.2f };
	pl.diffuse = { 0.8f, 0.5f, 0.5f };
	pl.specular = { 1.0f, 0.1f, 0.1f };
	pl._near = 0.5f;
	pl._far = 10.0f;

	app->m_text_shader->setVec3("point_light.position", pl.position);
	app->m_text_shader->setVec3("point_light.ambient", pl.ambient);
	app->m_text_shader->setVec3("point_light.diffuse", pl.diffuse);
	app->m_text_shader->setVec3("point_light.specular", pl.specular);
	app->m_text_shader->setFloat("point_light.near", pl._near);
	app->m_text_shader->setFloat("point_light.far", pl._far);

	app->m_text_shader->setVec3("obj_color", { 1.0f,0.0f,0.0f });
	app->m_text_shader->setBool("is_light", false);
	app->m_text_shader->setVec3("view_pos", { 0.0f,0.0f,1.0f });

	app->m_qm->setTexture(app->m_quad_shader, app->m_texture_colorbuffer);
	app->m_quad_shader->setInt("screen_texture", app->m_texture_colorbuffer);

	app->renderLoop();
	return 0;
}
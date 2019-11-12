#include "gl_app.h"
#include "texture.h"
#include "light.h"

using namespace std;

int main()
{
	GlApp *app = new GlApp;
	app->createWindow(280, 32, "test");
	app->generateFrameBuffer();

	TextMesh* tm = new TextMesh;
	tm->init();
	Shader* shader = new Shader("shaders/vs.glsl", "shaders/fs_light.glsl");

	int height, width;
	unsigned int texture_id = textureFromFile("./img/1.jpg", height, width);
	app->setViewport(width, height);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	tm->setTexture(shader, texture_id);

	PointLight pl;
	pl.position = { 0.0f,0.0f,1.0f };
	pl.ambient = { 0.2f, 0.2f, 0.2f };
	pl.diffuse = { 0.8f, 0.5f, 0.5f };
	pl.specular = { 1.0f, 0.1f, 0.1f };
	pl._near = 0.5f;
	pl._far = 10.0f;

	shader->setVec3("point_light.position", pl.position);
	shader->setVec3("point_light.ambient", pl.ambient);
	shader->setVec3("point_light.diffuse", pl.diffuse);
	shader->setFloat("point_light.near", pl._near);
	shader->setFloat("point_light.far", pl._far);


	shader->setVec3("obj_color", { 1.0f,0.0f,0.0f });
	shader->setBool("is_light", false);
	shader->setVec3("view_pos", { 0.0f,0.0f,1.0f });

	

	list<function<void()>> f_list;
	f_list.emplace_back(function<void()>([&]() {tm->draw(shader); }));
	app->renderLoop(f_list);
	return 0;
}
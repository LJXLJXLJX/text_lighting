#include "gl_app.h"
#include "texture.h"
#include "light.h"
#include <memory>
#include <random>

using namespace std;


int main()
{
	shared_ptr<GlApp> app = make_shared<GlApp>();
	app->createWindow(280, 32, "test");
	app->generateFrameBuffer();

	app->m_text_shader->initFromFile("../shaders/vs.glsl", "../shaders/fs_light.glsl");
	app->m_quad_shader->initFromFile("../shaders/quad_vs.glsl", "../shaders/quad_fs.glsl");

	app->m_tm->init();
	app->m_qm->init();

	//app->renderOnce("../img/1.jpg");


	//app->DebugRenderLoop("../img/1.jpg");
	app->run("../img", "../output", 1);
	return 0;
}
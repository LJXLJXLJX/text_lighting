#include "gl_app.h"
#include "texture.h"
#include "light.h"
#include <memory>
#include <random>

using namespace std;


int main(int argc,char* argv)
{
	const char* input_dir;
	const char* output_dir;

	if (argc == 1) {
		input_dir = "../img/gray";
		output_dir = "../output";
	}
	else if (argc == 3) {
		input_dir = (const char*)argv[1];
		output_dir = (const char*)argv[2];
	}
	else {
		cout << "输入参数错误" << endl;
		return -1;
	}

	cout << "正在初始化..." << endl;
	shared_ptr<GlApp> app = make_shared<GlApp>();
	app->createWindow(280, 280, "test");
	app->setViewport(280, 280);
	app->generateFrameBuffer();

	app->m_text_shader->initFromFile("../shaders/vs.glsl", "../shaders/fs_light.glsl");
	app->m_quad_shader->initFromFile("../shaders/quad_vs.glsl", "../shaders/quad_fs.glsl");

	app->m_tm->init();
	app->m_qm->init();

	//app->renderOnce("../img/1.jpg");

	//app->DebugRenderLoop("../0.jpg");
	app->run(input_dir, output_dir, 1);

	return 0;
}
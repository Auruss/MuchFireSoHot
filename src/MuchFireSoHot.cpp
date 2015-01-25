#include <cstdio>
#include <cstring>
#include <string>
#include <cstdlib>
#include <stdlib.h>

#include <emscripten/emscripten.h>

#include <Common/GameTime.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Level/Model/Base.h>

#include <Control/Mapper.h>
#include <Control/Global.h>

#include <OpenGL/Global.h>

#include <Common/LiveLog/Reflection.h>
#include <Common/LiveLog/Builder.h>

GameTimeObj fps_debugger;


extern "C" void toggle_fullscreen();
extern "C" void set_resolution(int x, int y);

// events required for frontend
extern "C" void game_build_status(int status);  // 0: ready 1: errors
extern "C" void game_session_status(int status); // 0: ready 1: errors
extern "C" void game_session_percentage_changed(int percentage);


Level::Model::Base* current_level = NULL;

Control::Mapper* mapper = NULL;
Control::Global* global_controls = NULL;


struct FpsLog {
    int fps;
};
static Common::LiveLog::ReflObject FpsLogRefl;


// prepare test system
void prepare_test() {
	// create test level
	current_level = new Level::Model::Base();
	current_level->ChunksX = 5;
	current_level->ChunksY = 5;
	current_level->ChunkScale = 10;
	current_level->LevelName = "Developer Test Level";
	current_level->Renderer = new Level::Renderer::Base(current_level);

	// test layer
	auto test_layer = new Level::Model::Layer();
	test_layer->X = 10;
	test_layer->Y = 10;
	test_layer->Scale = 10;
	test_layer->Renderer = new Level::Renderer::Layer(test_layer);
	test_layer->updateChanges();

	// add layers
	current_level->Layers.push_back(test_layer);

	// editor
	global_controls->LevelEditor->setCurrentLevel(current_level);
}


GLFWwindow* window;

//called each frame
void mainloop()
{
	// poll changes
	glfwPollEvents();

	// update
	Common::GameTime::on_frame();
	/*if (Common::GameTime::tickEvery(10000, fps_debugger, false)) { // print every 10 seconds the fps
		printf("FPS Debugger Tick: %d\n", Common::GameTime::FPS);
	}*/
    if(Common::GameTime::tickEvery(500, fps_debugger, false)) {
        FpsLog log;
        log.fps = Common::GameTime::FPS;

        Common::LiveLog::Builder builder(LOG_STATS_FPS);
        builder.setMessage("5 secs elapsed, time for logging");
        builder.addRefObj("fps", &FpsLogRefl, (void*)&log);
        builder.push();
    }

	current_level->Renderer->update();
	OpenGL::Global::update();

	//render
	glClear(GL_COLOR_BUFFER_BIT);


	current_level->Renderer->render();
	OpenGL::Global::render();

	glfwSwapBuffers(window);
}

void on_glfw_key_input(GLFWwindow* window, int key, int scan_code, int action, int mods) {
	int mapped_action = mapper->mapGLFWinput(key, action, mods);

	global_controls->onAction(mapped_action);
}

void init_gl(int width, int height) {
	if (glfwInit() != GL_TRUE) {
		fprintf(stderr, "GLFW failed\n");
		exit(1);
	}

	window = glfwCreateWindow(width, height, "JS", NULL, NULL);
	if (window == NULL) {
		printf("glfwCreateWindow() failed\n");
		exit(1);
	}
	//glfwSwapInterval(1);
	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if(err != GLEW_OK) {
		fprintf(stderr, "GLEW failed: %s\n", glewGetErrorString(err));
		exit(1);
	}

	printf("OpenGL Stats:\n");

	// Printing OpenGL information
	printf("\tVersion:  %s\n", glGetString(GL_VERSION));
	printf("\tGLSL:     %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("\tVendor:   %s\n", glGetString(GL_VENDOR));
	printf("\tRenderer: %s\n", glGetString(GL_RENDERER));

	// Print extensions
	GLint n = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &n);
	printf("\tExt.:     %d\n", n);


#ifdef WIN32
	for (GLint i = 0; i < n; i++)
	{
		printf("\tExt %d: %s\n", i, glGetStringi(GL_EXTENSIONS, i));
	}
#else
	printf("Extensions: %s\n", glGetString(GL_EXTENSIONS));
#endif


	glClearColor(0.4f, 0.1f, 0.3f, 1.0f);

	Common::GameTime::initialize();
	OpenGL::Global::init();

	// GLFW input
	glfwSetKeyCallback(window, on_glfw_key_input);
}

int main(int argc, char* argv [])
{
	bool test_flag = false;
	bool fullscreen_flag = false;
	int resolution_x = 800;
	int resolution_y = 600;

	for(int i = 0; i < argc; i++) {
		if(std::strcmp(argv[i], "--developer-test") == 0) {
			test_flag = true;
		}
		if (std::strcmp(argv[i], "--fullscreen") == 0) {
			fullscreen_flag = true;
		}
		else if(std::strcmp(argv[i], "-rx") == 0) {
			resolution_x = std::stoi(argv[++i]);
		}
		else if (std::strcmp(argv[i], "-ry") == 0) {
			resolution_y = std::stoi(argv[++i]);
		}
	}

	init_gl(resolution_x, resolution_y);
	mapper = new Control::Mapper();
	global_controls = new Control::Global(test_flag);

    FpsLogRefl.init<FpsLog>();
    FpsLogRefl.addMember<int>("fps", offsetof(FpsLog, fps));
    if(FpsLogRefl.sizeOf() != sizeof(FpsLog)) {
        fprintf(stderr, "Reflection error!\n");
        return 1;
    }

	if (test_flag) {
		printf("Started developer test\n");
		prepare_test();
	}

	emscripten_set_main_loop(mainloop, 0, true);
	return 0;
}

#include <cstdio>
#include <string>
#include <cstdlib>

#ifdef WIN32
#include <thread>
#include <chrono>
#endif

#include <Common/GameTime.h>

#ifndef WIN32 
#include <emscripten/emscripten.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Level/Model/Base.h>


GameTimeObj fps_debugger;


extern "C" void toggle_fullscreen() {

}

extern "C" void set_resolution(int x, int y) {

}

Level::Model::Base* current_level = NULL;


// prepare test system
void prepare_test() {
	// create test level
	current_level = new Level::Model::Base();
	current_level->ChunksX = 5;
	current_level->ChunksY = 5;
	current_level->ChunkScale = 10;
	current_level->LevelName = "Developer Test Level";
	current_level->Background = new Level::Model::Background();
	current_level->Background->BgColor = glm::vec3(1.0f, 0.5f, 0.25f);
	
}


GLFWwindow* window;

//called each frame
void mainloop()
{
	// update game time
	Common::GameTime::on_frame();
	if (Common::GameTime::tickEvery(10000, fps_debugger, false)) { // print every 10 seconds the fps
		printf("FPS Debugger Tick: %d\n", Common::GameTime::FPS);
	}

	//render
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (current_level != NULL) {
	}

	glfwSwapBuffers(window);
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
	glfwSwapInterval(1);
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


	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
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

	if(test_flag) {
		printf("Started developer test\n");
		prepare_test();
	}

	init_gl(resolution_x, resolution_y);

#ifdef WIN32
	while (true) {
		mainloop();
		//std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
#else
	emscripten_set_main_loop(mainloop, 60, true);
#endif

	return 0;
}

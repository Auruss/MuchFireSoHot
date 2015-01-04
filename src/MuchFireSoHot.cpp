#include <cstdio>
#include <string>
#include <cstdlib>

#ifdef WIN32
#include <thread>
#include <chrono>
#endif

#include "GameTime.h"

#ifndef WIN32 
#include <emscripten/emscripten.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>


GameTimeObj fps_debugger;


extern "C" void toggle_fullscreen() {

}

extern "C" void set_resolution(int x, int y) {

}

// prepare test system
void prepare_test() {

}

//called each frame
void mainloop()
{
	// update game time
	GameTime::on_frame();
	if(GameTime::tickEvery(10000, fps_debugger, false)) { // print every 10 seconds the fps
		printf("FPS Debugger Tick: %d\n", GameTime::FPS);
	}
}

void init_gl(int width, int height) {
	if (glfwInit() != GL_TRUE) {
		fprintf(stderr, "GLFW failed\n");
		exit(1);
	}

	GLFWwindow* window = glfwCreateWindow(width, height, "JS", NULL, NULL);
	if (window == NULL) {
		printf("glfwCreateWindow() failed\n");
		exit(1);
	}
	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if(err != GLEW_OK) {
		fprintf(stderr, "GLEW failed: %s\n", glewGetErrorString(err));
		exit(1);
	}
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
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
#else
	emscripten_set_main_loop(mainloop, 60, true);
#endif

	return 0;
}

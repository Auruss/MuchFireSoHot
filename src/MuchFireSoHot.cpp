#include <cstdio>
#include <string>

#ifdef WIN32
#include <thread>
#include <chrono>
#endif

#include "GameTime.h"

#ifndef WIN32 
#include <emscripten/emscripten.h>
#endif

GameTimeObj fps_debugger;


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


int main(int argc, char* argv [])
{
	bool test_flag = false;
	for(int i = 0; i < argc; i++) {
		if(std::strcmp(argv[i], "--developer-test") == 0) {
			test_flag = true;
		}
	}

	if(test_flag) {
		printf("Started developer test\n");
		prepare_test();
	}

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

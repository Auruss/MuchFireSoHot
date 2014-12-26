#include <cstdio>

#include "GameTime.h"

#include <emscripten/emscripten.h>


GameTimeObj fps_debugger;


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
	emscripten_set_main_loop(mainloop, 60, true);
	return 0;
}

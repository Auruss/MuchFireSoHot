#include <Common/GameTime.h>

namespace Common {

	uint32_t GameTime::FPS = 0;
	uint32_t GameTime::CurrentFrame = 0;

	GameTimeObj GameTime::fps_resetter;

	// -------------------------------------------------------------------------------------------------------

	void GameTime::initialize() {
		GameTime::CurrentFrame = 0;
		GameTime::FPS = 0;
	}

	void GameTime::on_frame() {
		GameTime::CurrentFrame++;

		if(GameTime::tickEvery(1000, GameTime::fps_resetter, false)) {
			GameTime::FPS = GameTime::CurrentFrame;
			GameTime::CurrentFrame = 0;
		}
	}

	// -------------------------------------------------------------------------------------------------------

	float GameTime::fade(uint32_t ms, GameTimeObj& obj) {
		using namespace std::chrono;

		if (obj.ms_done == 0) {
			obj.start = high_resolution_clock::now();
		}

		// might result into an non smooth movement on low fps (but still better than being unsync)
		if (obj.ms_done >= ms)  {
			float res = 1.0f - obj.done;
			obj.done = 1.0f;
			return res;
		}

		// calc in frames
		float current_frame_duration = 1000.0f / GameTime::FPS;
		float aprox_frames = (ms - obj.ms_done) / current_frame_duration;

		float perc = 1 / aprox_frames;
		return perc;
	}

	bool GameTime::tickEvery(uint32_t ms, GameTimeObj& obj, bool now) {
		using namespace std::chrono;

		if (obj.ms_done == 0) {
			obj.ms_done = 1;
			obj.last_tick = high_resolution_clock::now();


			// yes.. thats on purpose
			if (now) {
				return true;
			}
			else {
				return false;
			}
		}

		auto duration = high_resolution_clock::now() - obj.last_tick;
		if (duration_cast<milliseconds>(duration).count() >= ms) {
			obj.last_tick = high_resolution_clock::now();
			return true;
		}

		return false;
	}
}
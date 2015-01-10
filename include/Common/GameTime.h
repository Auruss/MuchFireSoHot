#pragma once

#include <stdint.h>
#include <chrono>

//typedef uint32_t GameTimeObj;
struct GameTimeObj {
	float done;

	uint32_t ms_done;

	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point last_tick;

	GameTimeObj() { reset(); }
	void reset() { done = 0.0f; ms_done = 0; }
};

namespace Common {

	class GameTime {

	public:
		static uint32_t FPS;
		static uint32_t CurrentFrame;
		static std::chrono::high_resolution_clock::time_point SecondStart;


		static void initialize();
		static void on_frame();

		/**
			Fades from 0.0 to 1.0

			Called every frame, after "ms" milliseconds all the sum of all results given equals 1.0
			After that it will keep responding with 0.0

			Example: 10FPS -> fade(1000, ....) -> it will result with 0.1 (10% percent) 10 times.

			@param ms time in milliseconds
			@param obj Create a new one for each action
			*/
		static float fade(uint32_t ms, GameTimeObj& obj);


		/**
			Tells whether an action that should tick every "ms" milliseconds should be called now.

			@param ms time in milliseconds
			@param obj Create a new one for each action
			*/
		static bool tickEvery(uint32_t ms, GameTimeObj& obj, bool now);

		/**
			UNIMPLEMENTED YET -------------------------------------------

			"fade" and "tickEvery" combined.
			every "every_ms" milliseconds fade gets called for "ms" milliseconds.

			@param ms how long fade will last
			@param every_ms every "every_ms" ms, fade will be called
			@param obj Create a new one for each action

			@see GameTime::fade
			@see GameTime::tickEvery
			*/
		static float fadeEvery(uint32_t ms, uint32_t every_ms, GameTimeObj& obj);

	private:
		static GameTimeObj fps_resetter;

	};

}
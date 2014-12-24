#include <core/oxygine.h>
#include <Stage.h>
#include <DebugActor.h>

//called each frame
int mainloop()
{
	//update our stage
	//update all actors. Actor::update would be called also for all children
	oxygine::getStage()->update();

	if (oxygine::core::beginRendering())
	{
		oxygine::Color clearColor(32, 32, 32, 255);
		oxygine::Rect viewport(oxygine::Point(0, 0), oxygine::core::getDisplaySize());
		//render all actors. Actor::render would be called also for all children
		oxygine::getStage()->render(clearColor, viewport);

		oxygine::core::swapDisplayBuffers();
	}

	//update internal components
	//all input events would be passed to Stage::instance.handleEvent
	//if done is true then User requests quit from app.
	bool done = oxygine::core::update();

	return done ? 1 : 0;
}

//it is application entry point
void run()
{
	oxygine::ObjectBase::__startTracingLeaks();

	//initialize Oxygine's internal stuff
	oxygine::core::init_desc desc;
	desc.vsync = true;
	desc.w = 960;
	desc.h = 640;
	oxygine::core::init(&desc);


	//create Stage. Stage is a root node
	oxygine::Stage::instance = new oxygine::Stage(true);
	oxygine::Point size = oxygine::core::getDisplaySize();
	oxygine::getStage()->setSize(size);

	//DebugActor is a helper actor node. It shows FPS, memory usage and other useful stuff
#if _DEBUG
	oxygine::DebugActor::show();
#endif

	//here is main game loop
	while (true)
	{
		int done = mainloop();
		if (done)
			break;
	}
	//user wants to leave application...

	//lets dump all created objects into log
	//all created and not freed resources would be displayed
	oxygine::ObjectBase::dumpCreatedObjects();

	//lets cleanup everything right now and call ObjectBase::dumpObjects() again
	//we need to free all allocated resources and delete all created actors
	//all actors/sprites are smart pointer objects and actually you don't need it remove them by hands
	//but now we want delete it by hands

	/**releases all internal components and Stage*/
	oxygine::core::release();

	//dump list should be empty now
	//we deleted everything and could be sure that there aren't any memory leaks
	oxygine::ObjectBase::dumpCreatedObjects();

	oxygine::ObjectBase::__stopTracingLeaks();
	//end
}

//extern "C"
//{
	int main(int argc, char* argv [])
	{
		run();
		return 0;
	}
//};
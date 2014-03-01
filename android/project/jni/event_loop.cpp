#include "event_loop.h"
#include "log.h"

EventLoop::EventLoop(android_app *app)
	: m_app(app)
{

}

void EventLoop::run()
{
	int32_t result;
	int32_t events;

	android_poll_source *source;

	app_dummy();

	Log::info("Starting event loop");

	while(true)
	{
		while ((result = ALooper_pollAll(-1, NULL, &events, (void**)(&source))) >= 0)
		{
			if (source != NULL)
			{
				Log::info("Processing an event");
				source->process(m_app, source);
			}

			if (m_app->destroyRequested)
			{
				Log::info("Existing event loop");
				return;
			}
		}
	}
}

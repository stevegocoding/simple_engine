#ifndef __ANDROID_EVENT_LOOP_H__
#define __ANDROID_EVENT_LOOP_H__

#include <android_native_app_glue.h>
#include "types.h"

using namespace std;

class EventLoop
{
public:
	EventLoop(android_app *app);

	void run();

private:
	android_app *m_app;
};




#endif

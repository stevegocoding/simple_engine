#ifndef __ANDROID_LOG_H__
#define __ANDROID_LOG_H__


class Log
{
public:
	static void info(const char *msg, ...);
	/*
	static void error(const char *msg, ...);
	static void warn(const char *msg, ...);
	static void debug(const char *msg, ...);
	*/
};

#ifndef NDEBUG
	#define log_debug(...) Log::debug(__VA_ARGS__)
#else
	#define log_debug(...)
#endif


#endif

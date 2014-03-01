#include "log.h"

#include <stdarg.h>
#include <android/log.h>


void Log::info(const char *msg, ...)
{
	va_list lvar_args;
	va_start(lvar_args, msg);

	__android_log_vprint(ANDROID_LOG_INFO, "test_msg", msg, lvar_args);
	__android_log_print(ANDROID_LOG_INFO, "test_msg", "\n");

	va_end(lvar_args);
}

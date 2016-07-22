#include <hal/commandline.h>
#include <hal/hal.h>
#include <logger.h>
#include <numconv.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <vga_text.h>
static int curLogLevel = 1;
const char *levels[]{
    "[DEBUG]", "[DEBUG]", "[INFO ]", "[WARN ]", "[ERROR]",
};
void logger_init() {
	int level = std::strtonum(get_arg("LogLevel"), 1);
	setLogLevel((LogLevel)(level + 1));
	Log(LOG_INFO, "[LOGGER]", "Log level is %s", levels[curLogLevel]);
}
void setLogLevel(LogLevel level) { curLogLevel = (int)level; }

void Log(LogLevel level, const char *service, const char *fmt, ...) {
	va_list list;
	va_start(list, fmt);
	Logv(level, service, fmt, list);
	va_end(list);
}
void Logv(LogLevel level, const char *service, const char *fmt, va_list list) {
	// syncRAII<remutex> sync(vga_mutex);
	if(level < curLogLevel) return;
	va_list l2;
	va_copy(l2, list);
	int len = vsnprintf(nullptr, 0, fmt, l2);
	va_end(l2);
	char *buf = (char *)__alloca(len + 1);
	vsnprintf(buf, len + 1, fmt, list);
	printf("%s %s %s\n", levels[level], service, buf);
	if(level == LOG_ERROR) { hal::halt(); }
}

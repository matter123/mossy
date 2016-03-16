#include "vcbprintf.h"
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
static inline bool check_print(struct print_status_t *ps,const char *buf, size_t len) {
	size_t ret=ps->write(ps->pass,buf,len);
	ps->count+=ret;
	return ret==len;
}
int vcbprintf(void *pass, cb_callback cb, const char *fmt, va_list arg) {
	print_status ps;
	memset(&ps,0,sizeof(ps));
	ps.pass=pass;
	ps.write=cb;
	va_copy(ps.arg,arg);
	while(*fmt) {
		const char *mark=fmt;
		while(*fmt && *fmt != '%') fmt++;
		if(mark != fmt) {
			check_print(&ps,mark,fmt-mark);
		}
		if(*fmt == '%') {
			int read=read_formatted(fmt,&ps);
			if(read < 0) {
				va_end(ps.arg);
				return -1;
			}
			fmt+=read;
		}
	}
	va_end(ps.arg);
	return ps.count;
}
int cbprintf(void *pass, cb_callback cb, const char *fmt, ...) {
	va_list arg;
	va_start(arg,fmt);
	int ret=vcbprintf(pass,cb,fmt,arg);
	va_end(arg);
	return ret;
}

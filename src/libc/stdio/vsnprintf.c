#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>

struct state {
    size_t bufrem;
    char *bufp;
};

static size_t strout(void *p, const char *buf, size_t sz) {
    struct state *s = p;
    size_t copy = s->bufrem >= sz ? sz : s->bufrem;
    memcpy( s->bufp, buf, copy );
    s->bufrem -= copy;
    s->bufp   += copy;
    return sz;
}

int vsnprintf(char * s,
              size_t n,
              const char * format,
              va_list arg) {
	struct state st;
	st.bufrem = n;
	st.bufp   = s;
	int r = vcbprintf(&st, strout, format, arg);
	if(st.bufrem) {
		*st.bufp = 0;
	}
    return r;
}
int snprintf(char *s,size_t max, const char *format, ...) {
	va_list arg;
	va_start(arg,format);
	int ret=vsnprintf(s,max,format,arg);
	va_end(arg);
	return ret;
}
int vsprintf(char *s,const char *format,va_list arg) {
	return vsnprintf(s,SIZE_MAX,format,arg);
}
int sprintf(char *s,const char *format, ...) {
	va_list arg;
	va_start(arg,format);
	int ret=vsprintf(s,format,arg);
	va_end(arg);
	return ret;
}

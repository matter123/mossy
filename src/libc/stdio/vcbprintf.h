#pragma once
#include <stdarg.h>
#include <stdio.h>
typedef struct print_status_t{
	int base;
	int flags;
	int n;
	int count;
	int current;
	int width;
	int prec;
	void *pass;
	cb_callback write;
	va_list arg;
} print_status;

#ifdef __cplusplus
extern "C"
#endif
int read_formatted(const char *fmt,struct print_status_t *ps);

#pragma once
// useful headers
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
// start of shared C/C++
#define IN_KERNEL defined(KERNEL)
#define IN_LIBK defined(LIBK)
#define IN_LIBC defined(LIBC)
#define NORETURN __attribute__((noreturn))
#define PACKED __attribute__((packed))
#define BEFORE_INIT __attribute__((constructor))
#define PRINTF_LIKE(arg_fmt, arg_chk) __attribute__((__format__(__printf__, arg_fmt, arg_chk)))
#define RUN_ONCE __attribute((section(".init.text"), cold))

typedef uint8_t *pointer_t;
typedef const uint8_t *const_pointer_t;
typedef unsigned int uint;

NORETURN void panic_fn(const char *message, const char *func, const char *file, int line);
NORETURN void panic(const char *message);
NORETURN void assertf(const char *message);
#define PANIC(m) panic_fn(m, __func__, __FILE__, __LINE__)
#define assert(a, b, m)   \
	do {                  \
		if((a) != (b))    \
			assertf((m)); \
	} while(0)

#ifdef __cplusplus
}
// C++ only
inline void *operator new(size_t, void *p) {
	return p;
}
inline void *operator new[](size_t, void *p) {
	return p;
}
inline void operator delete(void *, void *) {}
inline void operator delete[](void *, void *) {}
#endif

// kernel only c++
#if IN_KERNEL && defined(__cplusplus)
#include <logger.h>
constexpr int static_string_length(const char *str) {
	return *str ? 1 + static_string_length(str + 1) : 0;
}
#endif

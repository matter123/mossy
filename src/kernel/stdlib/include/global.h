#pragma once
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
#define IN_KERNEL defined(KERNEL)
#define IN_LIBK   defined(LIBK)
#define IN_LIBC   defined(LIBC)
#define NORETURN __attribute__((noreturn))
#define PACKED __attribute__((packed))
typedef uint8_t* pointer;
typedef const uint8_t* const_pointer;
NORETURN void panic_fn(const char *message,const char *func,const char *file, int line);
NORETURN void panic(const char *message);
NORETURN void assertf(const char *message);
#define PANIC(m) panic_fn(m,__func__,__FILE__,__LINE__)
#define assert(a,b,m) do {if((a) != (b))assertf((m));}while(0);
#ifdef __cplusplus
}
inline void *operator new(size_t, void *p)     { return p; }
inline void *operator new[](size_t, void *p)   { return p; }
inline void  operator delete  (void *, void *) { };
inline void  operator delete[](void *, void *) { };
#endif

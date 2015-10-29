#pragma once
#ifdef __cplusplus__
extern "C" {
#endif
#define IN_KERNEL defined(KERNEL)
#define IN_LIBK   defined(LIBK)
#define IN_LIBC   defined(LIBC)
#define NORETURN __attribute__((noreturn))
#define PACKED __attribute__((packed))
#include <stdint.h>
typedef uint8_t* pointer;
NORETURN void panic_fn(char *message,char *func,char *file, int line);
NORETURN void panic(char *message);
#define PANIC(m) panic_fn(m,__func__,__FILE__,__LINE__)
#ifdef __cplusplus__
}
#endif

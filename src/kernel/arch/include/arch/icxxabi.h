#ifndef ARCH_ICXXABI_H
#define ARCH_ICXXABI_H
#define ATEXIT_MAX_FUNCS 128

#ifdef __cplusplus
extern "C" {
#endif

struct atexit_func_entry_t {
	/*
	* Each member is at least 8 bytes large. Such that each entry is 24bytes.
	* 128 * 24 = 3KB exact.
	**/
	void (*destructor_func)(void *);
	void *obj_ptr;
	void *dso_handle;
};

int __cxa_atexit(void (*f)(void *), void *objptr, void *dso);
void __cxa_finalize(void *f);

#ifdef __cplusplus
};
#endif

#endif

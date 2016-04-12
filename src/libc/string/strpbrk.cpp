#include <string.h>

char *strpbrk(char *str1, const char *str2) {
	const char *s2=str2;
	while(*str1) {
		s2=str2;
		while(*s2) {
			if(*s2++==*str1) {
				return (char *)str1;
			}
		}
		str1++;
	}
	return nullptr;
}

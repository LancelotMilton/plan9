#include <string.h>

char*
strrchr(const char *s, int c)
{
	const char *r;

	if(c == 0)
		return strchr(s, 0);
	r = 0;
	while(s = strchr(s, c))
		r = s++;
	return (char*)r;
}

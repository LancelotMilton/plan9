#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

/*
 * BUG: never works
 */

int
setuid(uid_t uid)
{
	USED(uid);
	errno = EPERM;
	return -1;
}

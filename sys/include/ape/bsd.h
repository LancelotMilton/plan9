#ifndef _BSD_EXTENSION
    This header file is an extension to ANSI/POSIX
#endif

#ifndef __BSD_H_
#define __BSD_H_
#pragma src "/sys/src/ape/lib/bsd"
#pragma lib "/$M/lib/ape/libbsd.a"

#ifndef __TYPES_H
#include <sys/types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned long size_t;
#endif

/* ifndefs because X11 stuff (ugh) */
#ifndef bcopy
extern void	bcopy(void*, void*, size_t);
#endif
#ifndef bcmp
extern int	bcmp(void*, void*, size_t);
#endif
#ifndef bzero
extern void	bzero(void*, size_t);
#endif
extern int	ffs(unsigned int);
extern void	bhappy(void*);
extern int	rresvport(int*);
extern int	rcmd(char**, int, char*, char*, char*, int*);
extern int	strcasecmp(char*, char*);
extern int 	putenv(char*);
extern int	setenv(const char*, const char*, int);
extern int	unsetenv(const char*);
extern int	strncasecmp(char*, char*,int);
extern unsigned int	arc4random(void);
extern void	arc4random_buf(void*, size_t);
extern int	getentropy(void*, size_t);

extern const char	*getprogname(void);
extern void	setprogname(const char*);

extern int	getopt(int, char**, char*);
extern int	opterr;
extern int	optind;
extern int	optopt;
extern char	*optarg;
extern char	*mktemp(char *);
extern int	mkstemp(char *);
extern char	*sys_errlist[];
extern int	sys_nerr;

#ifdef __cplusplus
}
#endif

#endif

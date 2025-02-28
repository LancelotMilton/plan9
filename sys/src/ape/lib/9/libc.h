#ifndef _LIBC_H_
#define _LIBC_H_ 1

#define _LOCK_EXTENSION
#define _QLOCK_EXTENSION
#define _BSD_EXTENSION
#include <sys/types.h>
#include <lock.h>
#include <qlock.h>
#include <lib9.h>
#include <stdlib.h>
#include <string.h>
#include <bsd.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <utf.h>
#include <fmt.h>
#include <signal.h>
#include <time.h>

#define	nelem(x)	(sizeof(x)/sizeof((x)[0]))

extern	int	tokenize(char*, char**, int);

typedef
struct Qid
{
	uvlong	path;
	ulong	vers;
	uchar	type;
} Qid;

typedef
struct Dir {
	/* system-modified data */
	ushort	type;	/* server type */
	uint	dev;	/* server subtype */
	/* file data */
	Qid	qid;	/* unique id from server */
	ulong	mode;	/* permissions */
	ulong	atime;	/* last read time */
	ulong	mtime;	/* last write time */
	vlong	length;	/* file length: see <u.h> */
	char	*name;	/* last element of path */
	char	*uid;	/* owner name */
	char	*gid;	/* group name */
	char	*muid;	/* last modifier name */
} Dir;

uint	_convM2D(uchar*, uint, Dir*, char*);
uint	_convD2M(Dir*, uchar*, uint);
Dir	*_dirstat(char*);
int	_dirwstat(char*, Dir*);
Dir	*_dirfstat(int);
int	_dirfwstat(int, Dir*);
long	_dirread(int, Dir**);
long _dirreadall(int, Dir**);
void _nulldir(Dir*);
uint _sizeD2M(Dir*);

typedef
struct Waitmsg
{
	int pid;	/* of loved one */
	unsigned long time[3];	/* of loved one & descendants */
	char	*msg;
} Waitmsg;


extern	int	_AWAIT(char*, int);
extern	int	_ALARM(unsigned long);
extern	int	_BIND(const char*, const char*, int);
extern	int	_CHDIR(const char*);
extern	int	_CLOSE(int);
extern	int	_CREATE(char*, int, unsigned long);
extern	int	_DUP(int, int);
extern	int	_ERRSTR(char*, unsigned int);
extern	int	_EXEC(char*, char*[]);
extern	void	_EXITS(char *);
extern	int	_FD2PATH(int, char*, int);
extern	int	_FAUTH(int, char*);
extern	int	_FSESSION(int, char*, int);
extern	int	_FSTAT(int, unsigned char*, int);
extern	int	_FWSTAT(int, unsigned char*, int);
extern	int	_MOUNT(int, int, const char*, int, const char*);
extern	int	_NOTED(int);
extern	int	_NOTIFY(int(*)(void*, char*));
extern	int	_OPEN(const char*, int);
extern	int	_PIPE(int*);
extern	long	_PREAD(int, void*, long, long long);
extern	long	_PWRITE(int, void*, long, long long);
extern	long	_READ(int, void*, long);
extern	int	_REMOVE(const char*);
extern	void*	_RENDEZVOUS(void*, void*);
extern	int	_RFORK(int);
extern	void*	_SEGATTACH(int, char*, void*, unsigned long);
extern	void*	_SEGBRK(void*, void*);
extern	int	_SEGDETACH(void*);
extern	int	_SEGFLUSH(void*, unsigned long);
extern	int	_SEGFREE(void*, unsigned long);
extern	long long	_SEEK(int, long long, int);
extern	int	_SLEEP(long);
extern	int	_STAT(const char*, unsigned char*, int);
extern	Waitmsg*	_WAIT(void);
extern	long	_WRITE(int, const void*, long);
extern	int	_WSTAT(const char*, unsigned char*, int);
extern	long	_READN(int, void*, long);
extern	int	_IOUNIT(int);

extern 	void *_MALLOCZ(int, int);		/* not a syscall */

#define dirstat _dirstat
#define dirfstat _dirfstat

#define OREAD 0
#define OWRITE 1
#define ORDWR 2
#define	OEXEC	3	/* execute, == read but check execute permission */
#define	OTRUNC	16	/* or'ed in (except for exec), truncate file first */
#define	OCEXEC	32	/* or'ed in, close on exec */
#define	ORCLOSE	64	/* or'ed in, remove on close */
#define	OEXCL	0x1000	/* or'ed in, exclusive use (create only) */

#define AREAD 4
#define AWRITE 2
#define AEXEC 1
#define AEXIST 0

#ifdef thisisallgoingtoendintears
#define open _OPEN
#define close _CLOSE
#define read _READ
#define write _WRITE
#define create _CREATE
#define pread _PREAD
#define seek _SEEK
#endif

/* we don't have fauth(), so let this slide */
#define seek(fd, off, dir)	lseek(fd, off, dir)
#define	fauth _FAUTH
/* neither iounit */
#define iounit	_IOUNIT
#define wait	_WAIT

#define create(file, omode, perm)	open(file, (omode) |O_CREAT | O_TRUNC, perm)
#define readn _READN
#define mallocz _MALLOCZ

#define _exits(s) _exit(s && *(char*)s ? 1 : 0)
#define exits(s) exit(s && *(char*)s ? 1 : 0)
/* assume being called as in event.c */
#define postnote(x, pid, msg) kill(pid, SIGTERM)
#define atnotify(x, y) signal(SIGTERM, NULL)

#define ERRMAX 128

extern	void	setmalloctag(void*, ulong);
extern	ulong	getcallerpc(void*);

/* Used in libsec.h and not picked up in earlier type definitions */
typedef unsigned int u32int;
typedef unsigned long long u64int;

int  dec16(uchar *, int, char *, int);
int  enc16(char *, int, uchar *, int);
int  dec32(uchar *, int, char *, int);
int  enc32(char *, int, uchar *, int);
int  dec64(uchar *, int, char *, int);
int  enc64(char *, int, uchar *, int);
int decrypt(void*, void*, int);
int encrypt(void*, void*, int);

extern	vlong	nsec(void);

extern void sysfatal(char*, ...);

extern	ulong	truerand(void);			/* uses /dev/random */

#endif	/* _LIBC_H_ */

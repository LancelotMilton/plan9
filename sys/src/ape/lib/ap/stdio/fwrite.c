/*
 * pANS stdio -- fwrite
 */
#include "iolib.h"
#include <string.h>

#define BIGN (BUFSIZ/2)

size_t fwrite(const void *p, size_t recl, size_t nrec, FILE *f){
	char *s;
	int n, d;

	s=(char *)p;
	n=recl*nrec;
	while(n>0 && f->state!=CLOSED){
		d=f->rp-f->wp;
		if(d>0){
			if(d>n)
				d=n;
			memcpy(f->wp, s, d);
			f->wp+=d;
		}else{
			if(f->buf==f->unbuf || (n>=BIGN && f->state==WR && !(f->flags&(STRING|LINEBUF)))){
				d=f->wp-f->buf;
				if(d>0){
					if(f->flags&APPEND)
						lseek(f->fd, 0, SEEK_END);
					if(write(f->fd, f->buf, d)!=d){
						if(f->state!=CLOSED)
							f->state=ERR;
						goto ret;
					}
					f->wp=f->rp=f->buf;
				}
				if(f->flags&APPEND)
					lseek(f->fd, 0, SEEK_END);
				d=write(f->fd, s, n);
				if(d<=0){
					if(f->state!=CLOSED)
						f->state=ERR;
					goto ret;
				}
			}else{
				if(_IO_putc(*s, f)==EOF)
					goto ret;
				d=1;
			}
		}
		s+=d;
		n-=d;
	}
    ret:
	if(recl)
		return (s-(char*)p)/recl;
	else
		return s-(char*)p;
}

#include <u.h>
#include <libc.h>
#include "cpp.h"

Includelist	includelist[NINCLUDE];

char	*objname;

static int
curpos(int next)
{
	if (next)
		return cursource->pos;
	return NINCLUDE;
}

void
doinclude(Tokenrow *trp, int next)
{
	char fname[256], iname[256], *p;
	Includelist *ip;
	Source *s;
	int angled, len, fd, i, pos;

	trp->tp += 1;
	if (trp->tp>=trp->lp)
		goto syntax;
	if (trp->tp->type!=STRING && trp->tp->type!=LT) {
		len = trp->tp - trp->bp;
		expandrow(trp, "<include>", Notinmacro);
		trp->tp = trp->bp+len;
	}
	if (trp->tp->type==STRING) {
		len = trp->tp->len-2;
		if (len > sizeof(fname) - 1)
			len = sizeof(fname) - 1;
		strncpy(fname, (char*)trp->tp->t+1, len);
		angled = 0;
	} else if (trp->tp->type==LT) {
		len = 0;
		trp->tp++;
		while (trp->tp->type!=GT) {
			if (trp->tp>trp->lp || len+trp->tp->len+2 >= sizeof(fname))
				goto syntax;
			strncpy(fname+len, (char*)trp->tp->t, trp->tp->len);
			len += trp->tp->len;
			trp->tp++;
		}
		angled = 1;
	} else
		goto syntax;
	trp->tp += 2;
	if (trp->tp < trp->lp || len==0)
		goto syntax;
	fname[len] = '\0';
	pos = NINCLUDE;
	if (fname[0]=='/') {
		fd = open(fname, 0);
		strcpy(iname, fname);
	} else for (fd=-1,i=curpos(next)-1; i>=0; i--) {
		ip = &includelist[i];
		if (ip->file==NULL || ip->deleted || (angled && ip->always==0))
			continue;
		if (strlen(fname)+strlen(ip->file)+2 > sizeof(iname))
			continue;
		strcpy(iname, ip->file);
		strcat(iname, "/");
		strcat(iname, fname);
		if ((fd = open(iname, 0)) >= 0){
			pos = i;
			break;
		}
	}
	if(fd < 0 && !next) {
		strcpy(iname, cursource->filename);
		p = strrchr(iname, '/');
		if(p != NULL) {
			*p = '\0';
			strcat(iname, "/");
			strcat(iname, fname);
			fd = open(iname, 0);
			pos = NINCLUDE-1;
		}
	}
	if ( Mflag>1 || !angled&&Mflag==1 ) {
		write(1,objname,strlen(objname));
		write(1,iname,strlen(iname));
		write(1,"\n",1);
	}
	if (fd >= 0) {
		if (++incdepth > 20)
			error(FATAL, "#include too deeply nested");
		s = setsource((char*)newstring((uchar*)iname, strlen(iname), 0), fd, NULL);
		s->pos = pos;
		genline();
	} else {
		trp->tp = trp->bp+2;
		error(ERROR, "Could not find include file %r", trp);
	}
	return;
syntax:
	error(ERROR, "Syntax error in #include");
	return;
}

/*
 * Generate a line directive for cursource
 */
void
genline(void)
{
	static Token ta = { UNCLASS, NULL, 0, 0 };
	static Tokenrow tr = { &ta, &ta, &ta+1, 1 };
	uchar *p;

	if(nolineinfo)
		return;

	ta.t = p = (uchar*)outp;
	strcpy((char*)p, "#line ");
	p += sizeof("#line ")-1;
	p = (uchar*)outnum((char*)p, cursource->line);
	*p++ = ' '; *p++ = '"';
	if (cursource->filename[0]!='/' && wd[0]) {
		strcpy((char*)p, wd);
		p += strlen(wd);
		*p++ = '/';
	}
	strcpy((char*)p, cursource->filename);
	p += strlen((char*)p);
	*p++ = '"'; *p++ = '\n';
	ta.len = (char*)p-outp;
	outp = (char*)p;
	tr.tp = tr.bp;
	puttokens(&tr);
}

void
setobjname(char *f)
{
	int n = strlen(f);
	objname = (char*)domalloc(n+5);
	strcpy(objname,f);
	if(objname[n-2]=='.'){
		strcpy(objname+n-1,"$O: ");
	}else{
		strcpy(objname+n,"$O: ");
	}
}

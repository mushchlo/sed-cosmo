#include "u.h"
#include "libc.h"

char*
vseprint(char *buf, char *e, char *fmt, va_list args)
{
	if(e <= buf)
		return nil;
    Fmt f = {
	    .runes = 0,
	    .start = buf,
	    .to = buf,
	    .stop = e - 1,
	    .flush = nil,
	    .farg = nil,
	    .nfmt = 0,
	    .args = args
    };

	dofmt(&f, fmt);
	*(char*)f.to = '\0';
	return f.to;
}

#include	"../u.h"
#include	"../libc.h"
#include	"../bio.h"

long
Bgetrune(Biobufhdr *bp)
{
	int c, i;
	Rune rune;
	char str[UTFmax];

	c = Bgetc(bp);
	if(c < Runeself) {		/* one char */
		bp->runesize = 1;
		return c;
	}
	str[0] = c;

	for(i=1;;) {
		c = Bgetc(bp);
		if(c < 0)
			return c;
		str[i++] = c;

		if(fullrune(str, i)) {
			bp->runesize = chartorune(&rune, str);
			while(i > bp->runesize) {
				Bungetc(bp);
				i--;
			}
			return rune;
		}
	}
}

int
Bungetrune(Biobufhdr *bp)
{

	if(bp->state == Bracteof)
		bp->state = Bractive;
	if(bp->state != Bractive)
		return Beof;
	bp->icount -= bp->runesize;
	bp->runesize = 0;
	return 1;
}

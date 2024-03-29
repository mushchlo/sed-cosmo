#include	"../u.h"
#include	"../libc.h"
#include	"../bio.h"

vlong
Bseek(Biobufhdr *bp, vlong offset, int base)
{
	vlong n, d;

	switch(bp->state) {
	default:
		fprint(2, "Bseek: unknown state %d\n", bp->state);
		return Beof;

	case Bracteof:
		bp->state = Bractive;
		bp->icount = 0;
		bp->gbuf = bp->ebuf;

	case Bractive:
		n = offset;
		if(base == 1) {
			n += Boffset(bp);
			base = 0;
		}

		/*
		 * try to seek within buffer
		 */
		if(base == 0) {
			/*
			 * if d is too large for an int, icount may wrap,
			 * so we need to ensure that icount hasn't wrapped
			 * and points within the buffer's valid data.
			 */
			d = n - Boffset(bp);
			bp->icount += d;
			if(d <= bp->bsize && bp->icount <= 0 &&
			    bp->ebuf - bp->gbuf >= -bp->icount)
				return n;
		}

		/*
		 * reset the buffer
		 */
		n = seek(bp->fid, n, base);
		bp->icount = 0;
		bp->gbuf = bp->ebuf;
		break;

	case Bwactive:
		Bflush(bp);
		n = seek(bp->fid, offset, base);
		break;
	}
	bp->offset = n;
	return n;
}

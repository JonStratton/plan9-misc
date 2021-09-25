#include <u.h>
#include <libc.h>
#include <draw.h>
#include "klondike.h"

void
drawcard(Card *c)
{
	draw(bb,  cardrect(c), c->img[c->up], nil,  ZP);
}

void
drawstack(Cardstack *cs)
{
	int i;
	Rectangle cr;
	
	if(cs->n == 0){
		cr = Rect(0, 0, Cwid, Cht);
		border(bb, rectaddpt(cr, cs->p), 1, display->black, ZP);
	}
	else{
		if(cs->dpad != 0 && cs->upad != 0)
			for(i = 0; i < cs->n; i++)
				drawcard(cs->c[i]);
		else
			drawcard(top(cs));
	}
}

void
drawtable(Image *img)
{
	int i;
	
	tablemath();
	draw(bb, bb->r, display->white, nil, ZP);
	drawstack(stock);
	drawstack(waste);
	for(i = 0; i < nelem(foundation); i++)
		drawstack(foundation[i]);
	for(i = 0; i < nelem(tableau); i++)
		drawstack(tableau[i]);
	draw(img, bb->r, bb, nil, bb->r.min);
}

void
translate(Card *ca[], Point dt)
{
	Rectangle cr;
	Card *c, **cp;

	cr = cardrect(ca[0]);
	for(cp = ca; *cp != nil; cp++)
		combinerect(&cr, cardrect(*cp));
	draw(bb, cr, bb2, nil, cr.min);
	for(cp = ca; (c = *cp); cp++) {
		c->p = addpt(c->p, dt);
		drawcard(c);
		combinerect(&cr, cardrect(c));
	}
	draw(screen, cr, bb, nil, cr.min);
}

Image*
openimage(char *path)
{
	int fd;
	Image *img;

	if((fd = open(path, OREAD)) < 0)
		sysfatal("open %s: %r", path);
	img = readimage(display, fd, 0);
	if(img == nil)
		sysfatal("readimage %s: %r", path);
	close(fd);
	return img;
}

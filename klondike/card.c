#include <u.h>
#include <libc.h>
#include <draw.h>
#include "klondike.h"

char *suitpathfmt[] = {
	"%s/c%d.bit",
	"%s/d%d.bit",
	"%s/s%d.bit",
	"%s/h%d.bit"
};

Image*
opencard(int suit, int rank)
{
	char *s;
	Image *img;

	s = smprint(suitpathfmt[suit], CARDPATH, rank);
	if(s == nil)
		sysfatal("smprint: %r");
	img = openimage(s);
	free(s);
	return img;
}

Card*
cardinit(int suit, int rank, int up)
{
	Card *c;

	c = malloc(sizeof(Card));
	c->suit = suit;
	c->rank = rank;
	c->up = up;
	c->img[0] = cardback;
	c->img[1] = opencard(suit, rank+1);
	c->p = ZP;
	return c;
}

int
turncard(Point xy)
{
	Card *c;
	Cardstack *src, *dst;
	
	src = chosenstack(xy);
	if(src == nil)
		return 0;
	if(!canturn(src))
		return 0;
	if(src == stock && src->n == 0){
		while((c = pop(waste)) != nil){
			c->up = 0;
			push(stock, c);
		}
	}
	else{
		c = pop(src);
		if(!c->up)
			c->up++;
		dst = src == stock ? waste : src;
		push(dst, c);
	}
	drawtable(screen);
	return 1;
}

void
putinfoundation(Point xy)
{
	int i;
	Card *c;
	Cardstack *src, *dst;

	src = chosenstack(xy);
	if(src == nil || src == stock
	|| stackinarray(src, foundation, nelem(foundation))
	|| movpilesize(xy, src) == 0
	|| !top(src)->up)
		return;
	c = pop(src);
	dst = src;
	for(i = 0; i < nelem(foundation); i++){
		dst = foundation[i];
		if(validmove(c, dst))
			break;
		dst = src;
	}
	push(dst, c);
	drawtable(screen);
}

int
canturn(Cardstack *cs)
{
	Card *c;

	if(cs == stock)
		return 1;
	if(stackinarray(cs, tableau, nelem(tableau))){
		if(cs->n == 0)
			return 0;
		c = top(cs);
		if(!c->up)
			return 1;
	}
	return 0;
}

int
candrop(Card *c, Cardstack *cs)
{
	int a1, a2;
	Rectangle cr, sr;

	cr = cardrect(c);
	sr = stackrect(cs);
	if(rectclip(&sr, cr)){
		a1 = Dx(cr) * Dy(cr);
		a2 = Dx(sr) * Dy(sr);
		if((double) a2/a1 > 0.25) /*arbitrary*/
			return 1;
	}
	return 0;
}

int
samesuit(Card *c1, Card *c2)
{
	return c1->suit == c2->suit;
}

int
samecolor(Card *c1, Card *c2)
{
	return c1->suit%2 == c2->suit%2;
}

int
insequence(Card *c1, Card *c2, int b)
{
	return c1->rank == c2->rank+b;
}

int
validmove(Card *c, Cardstack *dst)
{
	Card *t;

	t = top(dst);
	if(stackinarray(dst, foundation, nelem(foundation))){
		if(dst->n == 0)
			return c->rank == Ace;
		return samesuit(c, t)
		&& insequence(c, t, Bup)  
		&& t->up;
	}
	if(stackinarray(dst, tableau, nelem(tableau))){
		if(dst->n == 0)
			return c->rank == King;
		return !samecolor(c, t) 
		&& insequence(c, t, Tdn) 
		&& t->up;
	}
	return 0;
}

#include <u.h>
#include <libc.h>
#include <draw.h>
#include "klondike.h"

Cardstack*
stackinit(int n, int upad, int dpad, int sz)
{
	Cardstack *cs;

	cs = malloc(sizeof(Cardstack));
	cs->sz = sz;
	cs->n = n;
	cs->upad = upad;
	cs->dpad = dpad;
	cs->p = ZP;
	cs->c = malloc(sz*sizeof(Card*));
	return cs;
}

Card*
pop(Cardstack *cs)
{
	if(cs->n == 0)
		return nil;
	return cs->c[--cs->n];
}

Card*
top(Cardstack *cs)
{
	if(cs->n == 0)
		return nil;
	return cs->c[cs->n-1];
}

void
push(Cardstack *cs, Card *c)
{
	if(cs->n < cs->sz)
		cs->c[cs->n++] = c;
}

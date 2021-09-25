#include <u.h>
#include <libc.h>
#include <draw.h>
#include "klondike.h"

void
stackmath(Cardstack *cs)
{
	int i, y;
	Card *c;

	y = cs->p.y;
	for(i = 0; i < cs->n; i++){
		c = cs->c[i];
		c->p = Pt(cs->p.x, y);
		if(c->up)
			y += cs->upad;
		else
			y += cs->dpad;
	}
}

void
tablemath()
{
	int i, x, y;

	stock->p.x = Xpad;
	stock->p.y = Ypad;
	stock->p = addpt(screen->r.min, stock->p);
	stackmath(stock);
	waste->p.x = Cwid+Xpad*2;
	waste->p.y = Ypad;
	waste->p = addpt(screen->r.min, waste->p);
	stackmath(waste);
	x = Xpad;
	y = Cht+Ypad*2;
	for(i = 0; i < nelem(tableau); i++){
		tableau[i]->p.x = x;
		tableau[i]->p.y = y;
		tableau[i]->p = addpt(screen->r.min, tableau[i]->p);
		stackmath(tableau[i]);
		x += Cwid+Xpad;
	}
	x = tableau[3]->p.x - screen->r.min.x;
	y = Ypad;
	for(i = 0; i < nelem(foundation); i++){
		foundation[i]->p.x = x;
		foundation[i]->p.y = y;
		foundation[i]->p = addpt(screen->r.min, foundation[i]->p);
		stackmath(foundation[i]);
		x += Cwid+Xpad;
	}
}

int
stackinarray(Cardstack *cs, Cardstack *csa[], int n)
{
	int i;

	for(i = 0; i < n; i++)
		if(cs == csa[i])
			return 1;
	return 0;
}

int
ptinstack(Point xy, Cardstack *cs)
{
	return ptinrect(xy, stackrect(cs));
}

Rectangle
cardrect(Card *c)
{
	return rectaddpt(Rect(0, 0, Cwid, Cht), c->p);
}

Rectangle
stackrect(Cardstack *cs)
{
	int i;
	Card *c;
	Rectangle sr;

	sr = Rpt(ZP, ZP);
	for(i = 0; i < cs->n-1; i++){
		c = cs->c[i];
		if(c->up)
			sr.max.y += cs->upad;
		else
			sr.max.y += cs->dpad;
	}
	sr.max.x += Cwid;
	sr.max.y += Cht;
	return rectaddpt(sr, cs->p);
}

Cardstack*
chosenstack(Point xy)
{
	int i;

	if(ptinstack(xy, stock))
		return stock;
	if(ptinstack(xy, waste))
		return waste;
	for(i = 0; i < nelem(foundation); i++)
		if(ptinstack(xy, foundation[i]))
			return foundation[i];
	for(i = 0; i < nelem(tableau); i++)
		if(ptinstack(xy, tableau[i]))
			return tableau[i];
	return nil;
}

Cardstack*
droptarget(Card *c)
{
	int i;

	for(i = 0; i < nelem(foundation); i++)
		if(candrop(c, foundation[i]))
			return foundation[i];
	for(i = 0; i < nelem(tableau); i++)
		if(candrop(c, tableau[i]))
			return tableau[i];
	return nil;
}

int
movpilesize(Point xy, Cardstack *cs)
{
	Card *c;
	Rectangle cr;
	int i;
	
	if(cs->n == 0)
		return 0;
	cr = cardrect(top(cs));
	i = cs->n - 1;
	while(!ptinrect(xy, cr)){
		c = cs->c[--i];
		if(c->up)
			cr.min.y -= cs->upad;
		else
			return 0;
	}
	return cs->n - i;
}

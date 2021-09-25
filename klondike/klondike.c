/*Felipe Bichued <bichued(at)gmail(dot)com>*/
#include <u.h>
#include <libc.h>
#include <draw.h>
#include <event.h>
#include <cursor.h>
#include "klondike.h"

char *buttons[] =
{
	"New",
	"Quit",
	0
};

Menu menu = 
{
	buttons
};

Cardstack *stock;
Cardstack *waste;
Cardstack *foundation[4];
Cardstack *tableau[7];
Card *movpile[14];/*13 cards + 1 nil*/
Card *allcards[52];

int dealtflag;
Image *bb, *bb2;
Image *cardback;
Mouse mouse;

void
resize(void)
{
	int fd;

	fd = open("/dev/wctl", OWRITE);
	if(fd > 0){
		fprint(fd, "resize -dx %d -dy %d", Swid, Sht);
		close(fd);
	}
}

void
eresized(int n)
{
	if(n && getwindow(display, Refnone) < 0)
		exits("eresized");
	freeimage(bb);
	bb = allocimage(display, screen->r, screen->chan, 0, 0);
	freeimage(bb2);
	bb2 = allocimage(display, screen->r, screen->chan, 0, 0);
	resize();
	drawtable(screen);
}

void
gameinit(void)
{
	int i;
	cardback = openimage(smprint("%s/back.bit", CARDPATH));
	stock = stackinit(0, 0, 0, 52);
	waste = stackinit(0, 0, 0, 52);
	for(i = 0; i < nelem(foundation); i++)
		foundation[i] = stackinit(0, 0, 0, 13);
	for(i = 0; i < nelem(tableau); i++)
		tableau[i] = stackinit(0, Upad, Dpad, i+13);
	for(i = 0; i < 52; i++)
		allcards[i] = cardinit(i%nelem(foundation), i%13, 0);
	for(i = 0; i < nelem(movpile); i++)
		movpile[i] = nil;
	bb = allocimage(display, screen->r, screen->chan, 0, 0);
	bb2 = allocimage(display, screen->r, screen->chan, 0, 0);
	resize();
}

void
shuffle(void)
{
	int i, r;
	Card *c;

	srand(time(0));
	for(i = 52; i > 0; i--){
		r = nrand(i);
		c = allcards[i-1];
		allcards[i-1] = allcards[r];
		allcards[r] = c;
	}
}

void
deal(void)
{
	int i, j;
	Card *c;
	
	shuffle();
	for(i = 0; i < 52; i++)
		push(stock, allcards[i]);
	for(i = 0; i < nelem(tableau); i++)
		for(j = 0; j <= i; j++){
			c = pop(stock);
			if(i == j)
				c->up++;
			push(tableau[i], c);
		}
	dealtflag = 1;
}

void 
cleangame(void){
	int i;

	if(!dealtflag)
		return;
	while(pop(stock) != nil);
	while(pop(waste) != nil);
	for(i = 0; i < nelem(foundation); i++)
		while(pop(foundation[i]) != nil);
	for(i = 0; i < nelem(tableau); i++)
		while(pop(tableau[i]) != nil);
	for(i = 0; i < 52; i++)
		allcards[i]->up = 0;
	for(i = 0; i < nelem(movpile); i++)
		movpile[i] = nil;
	dealtflag = 0;
}

void
waitbutrelease(int but)
{
	do mouse = emouse();
	while(mouse.buttons == but);
}

void
drag(void)
{
	int i, n;
	Point oxy, dxy;
	Cardstack *src, *dst;

	oxy = mouse.xy;
	src = chosenstack(oxy);
	if(src == nil)
		return;
	n = movpilesize(oxy, src);
	if(n == 0)
		return;
	while(n--)
		movpile[n] = pop(src);
	drawtable(bb2);
	//translate(movpile, ZP);
	do{
		dxy = subpt(mouse.xy, oxy);
		oxy = mouse.xy;
		if(dxy.x != 0 || dxy.y != 0)
			translate(movpile, dxy);
		mouse = emouse();
	}while(mouse.buttons == 1);
	dst = droptarget(movpile[0]);
	if(dst == nil || !validmove(movpile[0], dst))
		dst = src;
	for(i = 0; movpile[i] != nil; i++)
		push(dst, movpile[i]);
	drawtable(screen);
	for(i = 0; movpile[i] != nil; i++)
		movpile[i] = nil;
}

int
finished(void)
{
	int i;

	for(i = 0; i < nelem(foundation); i++)
		if(foundation[i]->n != 13)
			return 0;
	return 1;
}

void
main(void)
{
	if(initdraw(nil, nil, "klondike") < 0)
		sysfatal("initdraw: %r");
	einit(Emouse);
	gameinit();
Start:
	cleangame();
	deal();
	drawtable(screen);
	for(;;){			
		mouse = emouse();
		switch(mouse.buttons){
		case 1:
			if(turncard(mouse.xy))
				waitbutrelease(1);
			else
				drag();
			break;
		case 2:
			putinfoundation(mouse.xy);
			waitbutrelease(2);
			break;
		case 4:
			switch(emenuhit(3, &mouse, &menu)){
			case 0:
				goto Start;
			case 1:
				exits(0);
			}
		}
		if(finished())
			goto Start;
	}
}

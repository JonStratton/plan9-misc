enum{
	Xpad = 10,
	Ypad = 20,
	Sht = 508,
	Swid = 594
};

enum{
	Dpad = 6,
	Upad = 18,
	Cwid = 72,
	Cht = 99
};

enum{
	Tdn = -1, 
	Bup = 1
};

enum{
	Ace = 0,
	King = 12
};

typedef struct Card Card;
struct Card{
	int suit;
	int rank;
	int up;
	Point p;
	Image *img[2];
};

typedef struct Cardstack Cardstack;
struct Cardstack
{
	int sz;
	int n;
	int upad;
	int dpad;
	Point p;
	Card **c;
};

extern Cardstack *stock;
extern Cardstack *waste;
extern Cardstack *foundation[4];
extern Cardstack *tableau[7];
extern Image *bb, *bb2;
extern Image *cardback;
extern Mouse mouse;
/* extern char *cardpath = "./cards"; */
extern char cardpath[];

/*card.c*/
Card* cardinit(int, int, int);
int turncard(Point);
int canturn(Cardstack*);
int candrop(Card*, Cardstack*);
int validmove(Card*, Cardstack*);
void putinfoundation(Point);

/*draw.c*/
void drawtable(Image*);
void translate(Card*[], Point);
Image* openimage(char*);

/*stack.c*/
Cardstack* stackinit(int, int, int, int);
void push(Cardstack*, Card*);
Card* pop(Cardstack*);
Card* top(Cardstack*);

/*math.c*/
void tablemath(void);
int stackinarray(Cardstack*, Cardstack*[], int);
Rectangle cardrect(Card*);
Rectangle stackrect(Cardstack*);
Cardstack* chosenstack(Point);
Cardstack* droptarget(Card*);
int movpilesize(Point, Cardstack*);

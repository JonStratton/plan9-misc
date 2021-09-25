
</$objtype/mkfile

#BIN=/$objtype/bin/games
#CARDPATH=/sys/lib/klondike/cards
BIN=/usr/sdf/bin
CARDPATH=/usr/sdf/lib/klondike/cards
#BIN=./
#CARDPATH=cards

CFLAGS= -DCARDPATH="$CARDPATH"
TARG=klondike
OFILES=\
	card.$O\
	draw.$O\
	klondike.$O\
	math.$O\
	stack.$O\

UPDATE=\
	mkfile\
	${OFILES:%.$O=%.c}\

build: $O.out
	echo build

install:
	mkdir -p $CARDPATH
	cp cards/* $CARDPATH
	cp $O.out $BIN/$TARG

</sys/src/cmd/mkone
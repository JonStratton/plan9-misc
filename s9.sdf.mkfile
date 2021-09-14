# mkfile for Plan 9
# By Nils M Holm and contributors, 2008-2018
# Tweaked for 9p.sdf.org by jgstratt
#    To use, "mk -f s9.sdf.mkfile inst"

</$objtype/mkfile

TARG=		s9
OFILES=		s9.$O s9core.$O
CLEANFILES=	s9.image test.image
CFLAGS=		-FVw -Dplan9
EXTRASCM=

s9dir=		/usr/sdf/lib/s9fes
s9bin=		/usr/sdf/bin
s9rc=		/usr/sdf/rc

all:V:	s9 s9.image

tests:V:	coretest test realtest srtest libtest

s9:	$O.out
	cp $prereq $target
	chmod +x $target

s9.image:	s9 s9.scm config.scm
	S9_IMAGE_DIR=. S9FES_LIBRARY_PATH=lib:contrib \
		./s9 -i - -l config.scm $EXTRASCM -d $target

coretest:V:	s9core.c s9core.h s9import.h
	$CC -Dplan9 -DTEST -o s9test.$O s9core.c
	$LD -o s9test s9test.$O
	./s9test
	rm -f s9test.$O s9test

libtest:V: s9 test.image
	ape/psh util/$target.sh

%test:V: s9 test.image util/%test.scm
	./s9 -i ./test -f util/$target.scm

test.image: s9 s9.scm
	./s9 -i - -d $target

inst: s9 s9.image
	mkdir -p $s9dir
	mkdir -p $s9dir/^($objtype lib ext contrib help)
	cp s9 $s9bin/s9fes
	cp s9.image $s9dir/$objtype/s9.image
	cp s9.scm $s9dir/s9fes.scm
	cp lib/* $s9dir/lib
	cp contrib/* $s9dir/contrib
	{ x = `{pwd}; tar c help | { cd $s9dir; tar x }; cd $x }
	cat util/s9.rc | sed -e 's|^s9dir=.*|s9dir='$s9dir'|' | sed -e 's|^/\$objtype/bin|'$s9bin'|' > $s9rc/s9
	chmod 775 $s9rc/s9

deinst:
	rm -rf $s9dir
	rm -f $s9rc/s9 $s9bin/s9fes

</sys/src/cmd/mkone

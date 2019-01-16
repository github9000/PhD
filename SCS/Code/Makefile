#
# Makefile for Simple Classifier System C code
#
#####################################
# Define implicit compilation rules #
#####################################

# nominally tested on Sun systems only.
CC=gcc
LDLIBS= -lm

SRCCODE=advance.c aoc.c detector.c effector.c environ.c\
	ga.c initial.c io.c perform.c reinfor.c report.c\
	scs.c timekeep.c utility.c

OBJECTS=advance.o aoc.o detector.o effector.o environ.o\
	ga.o initial.o io.o perform.o reinfor.o report.o\
	scs.o timekeep.o utility.o

scs:    $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDLIBS)
	chmod 755 $@

$(OBJECTS): scs.h 

listing:
	cat *.h > scs.list
	cat $(SRCCODE) >> scs.list

clean:
	rm -f scs 
	rm -f *.on scs.sym
	rm -f *.o

#
# Makefile
# glh extension header
# Copyright 2006-2008, Universitaet Stuttgart (VISUS).
# Alle Rechte vorbehalten.
#

# defines
MAIN=extgen

CC=g++

CFLAGS= -DUNIX -Wall -g

INCLUDES= -I..

SRCS =  extgen.cpp

OBJS = $(SRCS:.cpp=.o)

#
# main target
#
glh_genext.h: extfile.txt $(MAIN)
	./$(MAIN)

#
# extension file
#
extfile.txt: genextfile.pl GL/glext.h
	cat GL/glext.h | perl genextfile.pl > extfile.txt

#
# generator
#
$(MAIN):  $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(MAIN) $(LFLAGS)
.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

#
# cleanup
#
clean:
	rm -f *.o *~ $(MAIN) extfile.txt glh_genext.h

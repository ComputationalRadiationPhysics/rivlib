#
# glutInclude.visglut.mk
#
# Copyright (C) 2007 - 2009 by VISUS (Universitaet Stuttgart).
# Alle Rechte vorbehalten.
#
# Include this file for the visglut library.
#

ifeq ($(BITS), 64)
	LDFLAGS += -L/home/schnei49/libs/visglut/freeglut/lib/lin64d -lfreeglut
else
	LDFLAGS += -L/home/schnei49/libs/visglut/freeglut/lib/lin32d -lfreeglut
endif

IncludeDir += /home/schnei49/libs/visglut/include \
              /home/schnei49/libs/visglut/freeglut/include

#
# glutInclude.visglut.mk
#
# Copyright (C) 2007 - 2009 by VISUS (Universitaet Stuttgart).
# Alle Rechte vorbehalten.
#
# Include this file for the visglut library.
#

ifeq ($(BITS), 64)
	LDFLAGS += -L/home/grottel/dev/insituvolvis/libs/visglut/freeglut/lib/lin64d -lfreeglut
else
	LDFLAGS += -L/home/grottel/dev/insituvolvis/libs/visglut/freeglut/lib/lin32d -lfreeglut
endif

IncludeDir += /home/grottel/dev/insituvolvis/libs/visglut/include \
              /home/grottel/dev/insituvolvis/libs/visglut/freeglut/include

#
# Makefile
#
# Copyright (C) 2007 - 2008 by Universitaet Stuttgart (VIS).
# Alle Rechte vorbehalten.
#

# Applications
AR = ar
CPP = gcc
LINK = gcc
MAKE = make
SHELL = /bin/bash


# Set Verbosity
ifndef VISGLUT_VERBOSE
    VISGLUT_VERBOSE = 0
endif
ifeq ($(VISGLUT_VERBOSE), 1)
    Q =
    ARFLAGS = -cvr
else
    Q = @
    ARFLAGS = -cr
endif


# Determine the plaform architecture
ifeq ($(shell uname -m), x86_64)
	PLATFORM := x64
	BITS := 64
else
	PLATFORM := x86
	BITS = 32
endif


# check whether we have the terminal for color outout
ifeq ($(TERM), xterm)
        COLORACTION = '\E[1;32;40m'
        COLORINFO =  '\E[0;32;40m'
        CLEARTERMCMD = tput sgr0
else
        COLORACTION =
        COLORINFO =
        CLEARTERMCMD = true
endif


# Add clib version to the bits field for binary compatibility
ifeq ($(BITS), 64)
	CLIBFILE := $(shell ldconfig -p  |grep libc.so | grep -e "x86.64" | perl -ne '/=>\s+(.*?)$$/ && print $$1;')
else
	CLIBFILE := $(shell ldconfig -p  |grep libc.so | grep -e "x86.64" -v | perl -ne '/=>\s+(.*?)$$/ && print $$1;')
endif
CLIBVER := $(shell $(CLIBFILE) | perl -ne '/C\s+Library.*?version\s+([\d\.]+)/ && ($$a = $$1) && ($$a =~ s/\./_/g) && print $$a;')
BITSEX := $(BITS)_clib$(CLIBVER)


# The projects target name
TargetName := visglut
LegTargetName := freeglut


# The default input directory
InputDir := ./../freeglut/src

# The default include directories:
SrcIncludeDir := ./../freeglut/include
IncludeDir := $(SrcIncludeDir) .

# List of system include directories:
SystemIncludeDir := /usr/include/g++ /usr/include/g++/bits /usr/include/g++/ext /usr/X11R6/include

# The default output directory
OutDir := ./../lib
LegOutDir := ./../freeglut/lib
TarIncDir := ./../include

# Default source exclude patterns:
ExcludeFromBuild := 


# The configuration intermetidate directories
DebugDir := Debug
ReleaseDir := Release


# Common compiler flags
CompilerFlags := -DUNIX -D_GNU_SOURCE -D_LIN$(BITS) -DHAVE_CONFIG_H -Wall -ansi -pedantic -fPIC

# Additional compiler flags for special configurations
DebugCompilerFlags := -DDEBUG -D_DEBUG -O0 -ggdb
ReleaseCompilerFlags := -DNDEBUG -D_NDEBUG -O3 -g0


# Common linker flags
LinkerFlags := -lX11 -lXext -lm

# Additional linker flags for special configurations
DebugLinkerFlags :=
ReleaseLinkerFlags :=


CPP_SRCS := $(filter-out $(ExcludeFromBuild), $(wildcard $(InputDir)/*.c))
CPP_DEPS_DEBUG := $(addprefix $(DebugDir)$(BITSEX)/, $(notdir $(patsubst %.c, %.d, $(CPP_SRCS))))
CPP_DEPS_RELEASE := $(addprefix $(ReleaseDir)$(BITSEX)/, $(notdir $(patsubst %.c, %.d, $(CPP_SRCS))))
CPP_DEPS := $(CPP_DEPS_DEBUG) $(CPP_DEPS_RELEASE)

OBJS_DEBUG := $(addprefix $(DebugDir)$(BITSEX)/, $(notdir $(patsubst %.c, %.o, $(CPP_SRCS))))
OBJS_RELEASE := $(addprefix $(ReleaseDir)$(BITSEX)/, $(notdir $(patsubst %.c, %.o, $(CPP_SRCS))))

CPPFLAGS := $(CompilerFlags) $(addprefix -I, $(IncludeDir)) $(addprefix -isystem, $(SystemIncludeDir))
LDFLAGS := $(LinkerFlags)

INCFILES := $(notdir $(wildcard $(SrcIncludeDir)/GL/*.h))
TARINCFILES := $(addprefix $(TarIncDir)/GL/, $(INCFILES))


all: VersionInfo $(TargetName)d $(TargetName)


# Rules for archives in $(OutDir):
$(TargetName)d: $(DebugDir)$(BITSEX)/lib$(TargetName).a
	@mkdir -p $(OutDir)
	@mkdir -p $(LegOutDir)
	@mkdir -p $(LegOutDir)/lin$(BITS)d
	$(Q)cp $< $(OutDir)/lib$(TargetName)$(BITS)d.a
	$(Q)cp $< $(OutDir)/lib$(TargetName)$(BITSEX)d.a
	$(Q)cp $< $(LegOutDir)/lin$(BITS)d/lib$(LegTargetName).a

$(TargetName): $(ReleaseDir)$(BITSEX)/lib$(TargetName).a
	@mkdir -p $(OutDir)
	@mkdir -p $(LegOutDir)
	@mkdir -p $(LegOutDir)/lin$(BITS)
	$(Q)cp $< $(OutDir)/lib$(TargetName)$(BITS).a
	$(Q)cp $< $(OutDir)/lib$(TargetName)$(BITSEX).a
	$(Q)cp $< $(LegOutDir)/lin$(BITS)/lib$(LegTargetName).a


# Rules for intermediate archives:
$(DebugDir)$(BITSEX)/lib$(TargetName).a: $(OBJS_DEBUG)
	@echo -e $(COLORACTION)"AR "$(COLORINFO)"$@ "
	@$(CLEARTERMCMD)
	$(Q)$(AR) $(ARFLAGS) $@ $^

$(ReleaseDir)$(BITSEX)/lib$(TargetName).a: $(OBJS_RELEASE)
	@echo -e $(COLORACTION)"AR "$(COLORINFO)"$@ "
	@$(CLEARTERMCMD)
	$(Q)$(AR) $(ARFLAGS) $@ $^


# Rules for copying include files
IncludeFiles: $(TARINCFILES) VersionInfo

$(TarIncDir)/GL/%.h: $(SrcIncludeDir)/GL/%.h
	@mkdir -p $(TarIncDir)
	@mkdir -p $(TarIncDir)/GL
	$(Q)cp $< $@

VersionInfo:
	@echo -e $(COLORACTION)"GEN "$(COLORINFO)"visglutversion.h"
	@$(CLEARTERMCMD)
	$(Q)perl visglutversion.pl .. visglutversion.template.h $(TarIncDir)/visglutversion.h

$(TarIncDir)/visglutversion.h:
	@echo -e $(COLORACTION)"GEN "$(COLORINFO)"visglutversion.h"
	@$(CLEARTERMCMD)
	$(Q)perl visglutversion.pl .. visglutversion.template.h $(TarIncDir)/visglutversion.h


# Rules for dependencies:
$(DebugDir)$(BITSEX)/%.d: $(InputDir)/%.c $(TarIncDir)/visglutversion.h
	@mkdir -p $(dir $@)
	@echo -e $(COLORACTION)"DEP "$(COLORINFO)"$@ "
	@$(CLEARTERMCMD)
	@echo -n $(dir $@) > $@
	$(Q)$(CPP) -MM $(CPPFLAGS) -I$(<D) $(DebugCompilerFlags) $< >> $@
#	$(Q)$(CPP) -MM $(CPPFLAGS) $(DebugCompilerFlags) $(filter %.c, $^) | sed -e 's/\(..*\)\.o\s*\:/$(DebugDir)$(BITSEX)\/\1.d $(DebugDir)$(BITSEX)\/\1.o:/g' > $@

$(ReleaseDir)$(BITSEX)/%.d: $(InputDir)/%.c $(TarIncDir)/visglutversion.h
	@mkdir -p $(dir $@)
	@echo -e $(COLORACTION)"DEP "$(COLORINFO)"$@ "
	@$(CLEARTERMCMD)
	@echo -n $(dir $@) > $@
	$(Q)$(CPP) -MM $(CPPFLAGS) -I$(<D) $(DebugCompilerFlags) $< >> $@
#	$(Q)$(CPP) -MM $(CPPFLAGS) $(ReleaseCompilerFlags) $(filter %.c, $^) | sed -e 's/\(..*\)\.o\s*\:/$(ReleaseDir)$(BITSEX)\/\1.d $(ReleaseDir)$(BITSEX)\/\1.o:/g' > $@


ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), sweep)
-include $(CPP_DEPS)
endif
endif


# Rules for object files:
$(DebugDir)$(BITSEX)/%.o:
	@mkdir -p $(dir $@)
	@echo -e $(COLORACTION)"CPP "$(COLORINFO)"$@ "
	@$(CLEARTERMCMD)
	$(Q)$(CPP) -c $(CPPFLAGS) $(DebugCompilerFlags) -o $@ $<

$(ReleaseDir)$(BITSEX)/%.o:
	@mkdir -p $(dir $@)
	@echo -e $(COLORACTION)"CPP "$(COLORINFO)"$@ "
	@$(CLEARTERMCMD)
	$(Q)$(CPP) -c $(CPPFLAGS) $(ReleaseCompilerFlags) -o $@ $<


# Cleanup rules:
clean: sweep
	rm -rf $(TarIncDir)/GL
	rm -f $(TarIncDir)/visglutversion.h
#	rm -f $(OutDir)/lin$(BITS)d/lib$(TargetName).a $(OutDir)/lin$(BITS)/lib$(TargetName).a

sweep:
	rm -rf $(ReleaseDir)$(BITSEX)/* $(DebugDir)$(BITSEX)/*

rebuild: 
	@$(MAKE) clean
	@$(MAKE) all

.PHONY: clean sweep rebuild tags VersionInfo

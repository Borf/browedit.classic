#!make

export

# Target folders
TOPDIR := $(shell echo `pwd`)
PLUGIN_BIN := $(TOPDIR)/plugins

# Compiler flags
LIBS = -lGL -lSDL -lgd -lGLU -lSDL_mixer -lcurl -lplugin_base
INCLUDES = -Isrc -DGRF_STATIC -D__NOXML__ -DGLEW_STATIC \
	-I$(TOPDIR)/libs/include \
	-I$(TOPDIR)/libs/bengine \
	-I$(TOPDIR)/libs/bengine/lib/include \
	-I$(TOPDIR)/src/wm \
	-I$(TOPDIR)/src
	
CXX = g++
CC = gcc
DEFINES = 
CFLAGS = -Wall -Wno-unknown-pragmas -pipe
# where are source files located?
SOURCE_DIRS= src src/wm src/tinyxml src/grflib src/windows src/undo

# Host platform
UNAME=$(shell uname -s | sed -e 's/_.*$$//')

#####
## PLATFORM DETECTION CODE
#####
ifeq ($(PLATFORM),)
UNAME_CPU=$(shell uname -m)

## Cygwin
ifeq ($(UNAME),CYGWIN)
# can't do linux build anyway on cygwin
PLATFORM=win32
endif

## Linux
ifeq ($(UNAME),Linux)
# detect 64bit
ifeq ($(UNAME_CPU),x86_64)
PLATFORM=linux64
else
PLATFORM=linux32
endif
endif

endif

#####
## END OF PLATFORM DETECTION CODE
#####

#####
## PER-PLATFORM SETTINGS
#####

# Linux 32bit
ifeq ($(PLATFORM),linux32)
CFLAGS += -m32
CC=gcc
CXX=g++
BINARY_EXT=
LIBRARY_EXT=.so
LIBRARY_PREFIX=lib
LIBS = -lGL -lSDL -lgd -lGLU -lSDL_mixer -lcurl -lplugin_base -L. -Llibs/lib -lbengine -Llibs/bengine
endif

# Linux 64bit
ifeq ($(PLATFORM),linux64)
CFLAGS += -m64
CC=gcc
CXX=g++
BINARY_EXT=
LIBRARY_EXT=.so
LIBRARY_PREFIX=lib
endif

# Windows 32bit
ifeq ($(PLATFORM),win32)
# Mh, we don't use GUI only mode, but console instead?
CFLAGS += -mconsole
ifeq ($(UNAME),CYGWIN)
# On cygwin, use provided gcc, but tell to not use cygwin's dlls
CC=gcc
CXX=g++
CFLAGS += -mno-cygwin -DNEEDS_DLLMAIN
else
DEBIAN=$(shell which i586-mingw32msvc-g++)
ifeq ($(DEBIAN),/usr/bin/i586-mingw32msvc-g++)
CC=i586-mingw32msvc-gcc
CXX=i586-mingw32msvc-g++
AR=i586-mingw32msvc-ar
else
CC=mingw32-gcc
CXX=mingw32-g++
AR=mingw32-ar
endif

endif
WINDRES=mingw32-windres
BINARY_EXT=.exe
LIBRARY_EXT=.dll
LIBRARY_PREFIX=
LIBS = -L. -Llibs/lib -lzlib1 -lSDL -lbgd -lopengl32 -lglu32 -lws2_32 -lcomdlg32 -lcurl -lSDL_mixer -lplugin_base -lwinmm -lgdi32
# ws2_32.lib sdl.lib sdlmain.lib zlib.lib bgd.lib opengl32.lib glu32.lib 
endif

#####
## END OF PER-PLATFORM SETTINGS
#####

## Debug build?
ifeq ($(DEBUG),)
DEBUG=yes
endif

ifeq ($(DEBUG),yes)
CFLAGS += -g -ggdb -D_DEBUG
else
# We suppose everyone have SSE. If this cause problems, switch mfpmath back to 387
CFLAGS += -O3 -fomit-frame-pointer -momit-leaf-frame-pointer -mfpmath=387
endif

TARGET=roworldedit_$(PLATFORM)$(BINARY_EXT)

# Define path where make will find source files :)
VPATH=$(subst  ,:,$(SOURCE_DIRS))

# list all source files
SOURCE_ALL=$(foreach dir,$(SOURCE_DIRS),$(wildcard $(dir)/*.c $(dir)/*.cpp))

# list all object files
OBJECTS_ALL=$(patsubst %,obj/%_$(PLATFORM).o,$(notdir $(basename $(SOURCE_ALL))))

# list all .dep files
DEP_ALL=$(patsubst %,obj/%_$(PLATFORM).dep,$(notdir $(basename $(filter %.c %.cpp,$(SOURCE_ALL)))))

ifeq ($(PLATFORM),win32)
# Fix: Win32 build needs this one
OBJECTS_SRC += obj/src_Script1_rc_$(PLATFORM).o
endif

all: plugins_all bengine $(TARGET)

bengine:
	make -C libs/bengine

plugins_all :
	make -C src/plugins

# Depencies

dep: obj/depencies_$(PLATFORM).mak

obj/depencies_$(PLATFORM).mak: $(DEP_ALL)
	@echo -e "    \033[1mDEP\033[1m\t\033[22;34m$@\033[39m                           "
# build depencies for objects
	@cat $^ | sed -r -e 's#^([a-zA-Z0-9]+)\.o#obj/\1_$(PLATFORM).o#' >$@
# build depencies for depencies
	@cat $^ | sed -r -e 's#^([a-zA-Z0-9]+)\.o#obj/\1_$(PLATFORM).dep#' >>$@

#-include obj/depencies_$(PLATFORM).mak

# type-specific targets (compile, target = %.o)

obj/plugins/%_$(PLATFORM).o: src/plugins/%.cpp
	@echo -e "    \033[1mCC\033[1m\t\033[22;34m$<\033[39m"
	@$(CXX) $(CFLAGS) $(INCLUDES) -DPLUGIN_EXPORTS -D_USRDLL -c -o $@ $<



obj/%_$(PLATFORM).o: %.c
	@echo -e "    \033[1mCC\033[1m\t\033[22;34m$<\033[39m"
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

obj/%_rc_$(PLATFORM).o: %.rc
	@echo -e "    \033[1mRC\033[1m\t\033[22;34m$<\033[39m"
	@$(WINDRES) -I src -i $< -o $@

obj/%_$(PLATFORM).o: %.cpp
	@echo -e "    \033[1mCC\033[1m\t\033[22;34m$<\033[39m"
	@$(CXX) $(CFLAGS) $(INCLUDES) -c -o $@ $<

# depencies

obj/%_$(PLATFORM).dep: %.c
	@echo -en "    \033[1mDEP\033[1m\t\033[22;34m$<\033[39m                           \r"
	@$(CC) $(CFLAGS) $(INCLUDES) -MM -MF $@ $<

obj/%_$(PLATFORM).dep: %.cpp
	@echo -en "    \033[1mDEP\033[1m\t\033[22;34m$<\033[39m                           \r"
	@$(CXX) $(CFLAGS) $(INCLUDES) -MM -MF $@ $<

# Main target

$(TARGET): $(OBJECTS_ALL)
	@echo -e "    \033[1mLD\033[1m\t\033[22;35m$@\033[39m"
	@$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)


plugin_base: obj/plugins/base/base_$(PLATFORM).o
	@echo -e "    \033[1mLD\033[1m\t\033[22;35m$@\033[39m"
	@$(AR) rcs libs/lib/libplugin_base.a obj/plugins/base/base_$(PLATFORM).o


plugin_clearmap: obj/plugins/clearmap/clearmap_$(PLATFORM).o obj/plugins/clearmap/plugin_$(PLATFORM).o
	@echo -e "    \033[1mLD\033[1m\t\033[22;35m$@\033[39m"
	@$(CXX) $(CFLAGS) $(LDFLAGS) -Llibs/lib -W1 --out-implib -shared -o plugins/clearmap.dll $^ -lopengl32 -lglu32 -lplugin_base

plugin_generators: obj/plugins/generators/mazegenerator_$(PLATFORM).o obj/plugins/generators/culvertgenerator_$(PLATFORM).o obj/plugins/generators/plugin_$(PLATFORM).o obj/vector3_$(PLATFORM).o obj/vector2_$(PLATFORM).o obj/math_$(PLATFORM).o obj/stream_$(PLATFORM).o
	@echo -e "    \033[1mLD\033[1m\t\033[22;35m$@\033[39m"
	@$(CXX) $(CFLAGS) $(LDFLAGS) -Llibs/lib -W1 --out-implib -shared -o plugins/generators.dll $^ -lopengl32 -lglu32 -lplugin_base

plugin_gat: obj/plugins/gat/setgatheight_$(PLATFORM).o obj/plugins/gat/plugin_$(PLATFORM).o
	@echo -e "    \033[1mLD\033[1m\t\033[22;35m$@\033[39m"
	@$(CXX) $(CFLAGS) $(LDFLAGS) -Llibs/lib -W1 --out-implib -shared -o plugins/gat.dll $^ -lopengl32 -lglu32 -lplugin_base

plugin_lightmap: obj/plugins/lightmap/lightmapexport_$(PLATFORM).o obj/plugins/lightmap/lightmapimport_$(PLATFORM).o obj/plugins/lightmap/plugin_$(PLATFORM).o
	@echo -e "    \033[1mLD\033[1m\t\033[22;35m$@\033[39m"
	@$(CXX) $(CFLAGS) $(LDFLAGS) -Llibs/lib -W1 --out-implib -shared -o plugins/lightmap.dll $^ -lopengl32 -lglu32 -lplugin_base -lbgd

plugin_3ds2rsm: obj/plugins/3ds2rsm/3ds2rsmplugin_$(PLATFORM).o obj/plugins/3ds2rsm/3ds_$(PLATFORM).o obj/plugins/3ds2rsm/plugin_$(PLATFORM).o
	@echo -e "    \033[1mLD\033[1m\t\033[22;35m$@\033[39m"
	@$(CXX) $(CFLAGS) $(LDFLAGS) -Llibs/lib -W1 --out-implib -shared -o plugins/3ds2rsm.dll $^ -lopengl32 -lglu32 -lplugin_base

#NEWTARGET#

depends :
	make -C libs/bengine depends
	make -C src/plugins depends

clean :
	make -C src/plugins clean

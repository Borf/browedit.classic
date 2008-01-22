#!make
LIBS = -lz -lGL -lSDL -lgd -lGLU
INCLUDES = -Isrc -DGRF_STATIC -D__NOXML__
CXX = g++
CC = gcc
DEFINES = 
CFLAGS = -Wall -pipe

#####
## PLATFORM DETECTION CODE
#####
ifeq ($(PLATFORM),)
UNAME=$(shell uname -s | sed -e 's/_.*$$//')
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

## Per-platform settings

ifeq ($(PLATFORM),linux32)
CFLAGS += -m32
CC=gcc
CXX=g++
endif

ifeq ($(PLATFORM),linux64)
CFLAGS += -m64
CC=gcc
CXX=g++
endif

ifeq ($(PLATFORM),win32)
CFLAGS += -mwindows
CC=mingw32-gcc
CXX=mingw32-g++
endif

## Debug build?

ifeq ($(DEBUG),)
DEBUG=yes
endif

ifeq ($(DEBUG),yes)
CFLAGS += -g -ggdb
else
CFLAGS += -O3
endif

TARGET=roworldedit_$(PLATFORM)

OBJECTS_SRC=$(patsubst src/%.cpp,obj/src_%_$(PLATFORM).o,$(wildcard src/*.cpp))
OBJECTS_WM=$(patsubst src/wm/%.cpp,obj/wm_%_$(PLATFORM).o,$(wildcard src/wm/*.cpp))
OBJECTS_TINYXML=$(patsubst src/tinyxml/%.cpp,obj/tinyxml_%_$(PLATFORM).o,$(wildcard src/tinyxml/*.cpp))
OBJECTS_GRFLIB=$(patsubst src/grflib/%.c,obj/grflib_%_$(PLATFORM).o,$(wildcard src/grflib/*.c))
OBJECTS_ZLIB=$(patsubst src/grflib/zlib/%.c,obj/zlib_%_$(PLATFORM).o,$(wildcard src/grflib/zlib/*.c))

OBJECTS_ALL=$(OBJECTS_SRC) $(OBJECTS_WM) $(OBJECTS_TINYXML) $(OBJECTS_GRFLIB) $(OBJECTS_ZLIB)

all: $(TARGET)

obj/src_%_$(PLATFORM).o: src/%.cpp
	@echo -e "    [1mCC\033[1m\t\033[22;34m$<\033[39m"
	@$(CXX) $(CFLAGS) $(INCLUDES) $(DEFINES) -c -o $@ $<

obj/src_%_$(PLATFORM).o: src/%.c
	@echo -e "    [1mCC\033[1m\t\033[22;34m$<\033[39m"
	@$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c -o $@ $<

obj/wm_%_$(PLATFORM).o: src/wm/%.cpp
	@echo -e "    [1mCC\033[1m\t\033[22;34m$<\033[39m"
	@$(CXX) $(CFLAGS) $(INCLUDES) $(DEFINES) -c -o $@ $<

obj/tinyxml_%_$(PLATFORM).o: src/tinyxml/%.cpp
	@echo -e "    [1mCC\033[1m\t\033[22;34m$<\033[39m"
	@$(CXX) $(CFLAGS) $(INCLUDES) $(DEFINES) -c -o $@ $<

obj/grflib_%_$(PLATFORM).o: src/grflib/%.c
	@echo -e "    [1mCC\033[1m\t\033[22;34m$<\033[39m"
	@$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c -o $@ $<

obj/zlib_%_$(PLATFORM).o: src/grflib/zlib/%.c
	@echo -e "    [1mCC\033[1m\t\033[22;34m$<\033[39m"
	@$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c -o $@ $<


$(TARGET): $(OBJECTS_ALL)
	@echo -e "    \033[1mLD\033[1m\t\033[22;35m$@\033[39m"
	@$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	$(RM) obj/*.o $(TARGET)


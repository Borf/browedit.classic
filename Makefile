#!make
# Makefile for Spacial
CC = /usr/bin/g++
CFLAGS = -pipe -Wall -O2
LIBS = 
AR = /usr/bin/ar

export MAKE CC CFLAGS LIBS AR

all: title browedit


title:
	@echo -e "BrowEdit...."
	@echo -e ""

browedit:
	@echo -e " - \033[1mDIR\033[1m\t\033[22;32msrc\033[39m"
	@$(MAKE) --no-print-directory -C src/ ../browedit

clean:
	@$(MAKE) -C src/ clean
	rm -f roworldedit

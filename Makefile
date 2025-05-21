OBJS=main.o game.o ui.o words.o stats.o
LIBS=
OS:=$(shell uname -s)
ifeq ($(OS), Linux)
	OBJS += platform_ncurses.o
	EXE = typing_master
	LIBS+=-lncurses
else
	OBJS += platform_win.o
	CFLAGS=-stdс=99
	EXE = typing_master.exe
endif

all: $(EXE)
	
.PHONY: all


%.o:%.c
	gcc $(CFLAGS) -c -o $@ $<

$(EXE): $(OBJS)
	gcc -o $@ $^ $(LIBS)


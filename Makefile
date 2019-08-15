CC=gcc
CFLAGS=-Wall
CLIBS=
DEBUG ?= 0
GAME ?= ttt
RENDERER ?= stdio

ifeq ($(OS), Windows_NT)
	PLATFORM:=windows
else
	PLATFORM:=linux
endif

ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG
else
	CFLAGS += -DNDEBUG
endif

.PHONY: client server clean
all: clean server client

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

client: client.o connect_$(PLATFORM).o game_$(GAME).o renderer_$(GAME)_$(RENDERER).c
	$(CC) $(CFLAGS) -o $@.out $^ $(CLIBS)

server: server.o listen_$(PLATFORM).o game_$(GAME).o
	$(CC) $(CFLAGS) -o $@.out $^ $(CLIBS)

clean:
	rm -f *.o *.out

CC=gcc
CFLAGS=-g -Wall -Wpedantic -Iinclude

SRCS=$(wildcard src/*c)
OBJS=$(patsubst src/%.c, obj/%.o, $(SRCS))

all: sim

sim: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

obj/%.o: src/%.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

obj:
	mkdir -p obj

clean:
	rm -f sim obj/*.o

run: sim
	./sim

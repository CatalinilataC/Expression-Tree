CC=gcc
CFLAGS=-Wall -Wextra -c
EXE=tema3
build: tema3.o
	$(CC) $^  -lm -o $(EXE)
tema3.o: main.c
	$(CC) $(CFLAGS) $^ -o $@
clean:
	rm -rf tema3.o $(EXE)

CC = gcc
INC = include
OBJ_P = obj/
CFLAGS = -Wall -I $(INC) -lSDL2 -lSDL2_ttf -lpthread

SRC = $(wildcard src/*.c)
OBJ = (src:.obj=.o)


emu: $(SRC)
	$(CC) $? -o $(OBJ_P)$@.o $(CFLAGS)

run: emu
	./$(OBJ_P)$?.o $(argument)

clean:
	rm -f $(OBJ_P)/*.o 

CC = gcc
INC = include
OBJ_P = obj
CFLAGS = -Wall -I $(INC)

SRC = $(wildcard src/*.c)
OBJ = (src:.obj=.o)


ines_loader: $(SRC)
	$(CC) $(CFLAGS) src/$@.c -o obj/$@.o

clean:
	rm -f $(OBJ_P)/*.o 

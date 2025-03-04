CC=gcc
CFLAGS=-Wall -std=c99 -lm -c
OBJ=main.o per_proc.o linked_list.o
EXEC=Main

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

main.o: main.c per_proc.h
	$(CC) main.c $(CFLAGS)

per_proc.o: per_proc.c per_proc.h linked_list.h
	$(CC) per_proc.c $(CFLAGS)

linked_list.o: linked_list.c linked_list.h
	$(CC) linked_list.c $(CFLAGS)

clean:
	rm -f $(OBJ) $(EXEC)

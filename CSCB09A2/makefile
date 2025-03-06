CC=gcc
CFLAGS=-Wall -std=c99 -lm -c
OBJ=main.o file_proc.o linked_list.o per_process.o sys_wide.o composite.o vnodes.o summary.o threshold.o
EXEC=Main

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

main.o: main.c file_proc.h
	$(CC) main.c $(CFLAGS)

file_proc.o: file_proc.c file_proc.h linked_list.h
	$(CC) file_proc.c $(CFLAGS)

linked_list.o: linked_list.c linked_list.h
	$(CC) linked_list.c $(CFLAGS)

per_process.o: per_process.c per_process.h linked_list.h
	$(CC) per_process.c $(CFLAGS)

sys_wide.o: sys_wide.c sys_wide.h linked_list.h
	$(CC) sys_wide.c $(CFLAGS)

composite.o: composite.c composite.h linked_list.h
	$(CC) composite.c $(CFLAGS)

vnodes.o: vnodes.c vnodes.h linked_list.h
	$(CC) vnodes.c $(CFLAGS)

summary.o: summary.c summary.h linked_list.h
	$(CC) summary.c $(CFLAGS)

threshold.o: threshold.c threshold.h linked_list.h
	$(CC) threshold.c $(CFLAGS)

clean:
	rm -f $(OBJ) $(EXEC)

CC=gcc
CFLAGS=-Wall -std=c99 -lm -c -s
OBJ=myMonitoringTool.o aux.o info_fetcher.o flag_handler.o signal_handler.o
EXEC=Main

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

myMonitoringTool.o: myMonitoringTool.c aux.h info_fetcher.h flag_handler.h
	$(CC) myMonitoringTool.c $(CFLAGS)

aux.o: aux.c aux.h
	$(CC) aux.c $(CFLAGS)

info_fetcher.o: info_fetcher.c info_fetcher.h
	$(CC) info_fetcher.c $(CFLAGS)

flag_handler.o: flag_handler.c flag_handler.h
	$(CC) flag_handler.c $(CFLAGS)

signal_handler.o: signal_handler.c signal_handler.h 
	$(CC) signal_handler.c $(CFLAGS)

clean:
	rm -f $(OBJ) $(EXEC)
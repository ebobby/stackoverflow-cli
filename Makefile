
all: stackoverflow-cli

OBJ = stackoverflow.o requests.o arguments.o stackoverflow-cli.o

stackoverflow.o: stackoverflow.c stackoverflow-cli.h
requests.o: requests.c stackoverflow-cli.h
arguments.o: arguments.c stackoverflow-cli.h
stackoverflow-cli.o: stackoverflow-cli.c stackoverflow-cli.h

%.o: %.c
	$(CC) -ggdb -std=c99 -c $< -o $@

stackoverflow-cli: $(OBJ)
	$(CC) -ggdb -std=c99 -o $@ -lcurl -ljson $(OBJ)

clean:
	rm -rf *.o stackoverflow-cli
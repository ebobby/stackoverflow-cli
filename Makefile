UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
  CFLAGS ?=-std=c99 -ggdb -I/opt/local/include
  CCLINK ?=-ljson -lcurl -L/opt/local/lib
else
  CFLAGS ?=-std=c99 -ggdb
  CCLINK ?=-ljson -lcurl
endif

all: stackoverflow-cli

OBJ = stackoverflow.o requests.o arguments.o stackoverflow-cli.o

stackoverflow.o: stackoverflow.c stackoverflow-cli.h
requests.o: requests.c stackoverflow-cli.h
arguments.o: arguments.c stackoverflow-cli.h
stackoverflow-cli.o: stackoverflow-cli.c stackoverflow-cli.h

CCOPT=$(CFLAGS)

%.o: %.c
	$(CC) $(CCOPT) -c $< -o $@

stackoverflow-cli: $(OBJ)
	$(CC) -o $@ $(CCLINK) $(OBJ)

clean:
	rm -rf *.o stackoverflow-cli


INCPATHS=
LIBPATHS=
LDFLAGS=
CFLAGS= -Wall -Og -g2 -O0
CC=$(CROSS_COMPILE)gcc
AR=$(CROSS_COMPILE)ar

# ------------ MAGIC BEGINS HERE -------------

# Automatic generation of some important lists
OBJECTS=$(SOURCES:.c=.o)
INCFLAGS=$(foreach TMP,$(INCPATHS),-I$(TMP))
LIBFLAGS=$(foreach TMP,$(LIBPATHS),-L$(TMP))
BINARY=example interrupt_example


all: $(OBJECTS) $(BINARY)


.c.o:
	$(CC) $(INCFLAGS) $(CFLAGS) -fPIC $< -o $@

distclean: clean
	rm -f $(BINARY) $(OBJECTS)

example: example.c
	$(CC) $(CFLAGS) -o example example.c -I../src -L../src -lmcp23s17

interrupt_example: interrupt_example.c
	$(CC) $(CFLAGS) -o interrupt_example interrupt_example.c -I../src -L../src -lmcp23s17

clean:
	rm -f $(OBJECTS) $(BINARY)

install: $(BINARY)
	


INCPATHS= ${HOME}/local/include
LIBPATHS= ${HOME}/local/lib
LDFLAGS=
CFLAGS=-c -Wall 
CC=$(CROSS_COMPILE)gcc
AR=$(CROSS_COMPILE)ar

# ------------ MAGIC BEGINS HERE -------------

# Automatic generation of some important lists
OBJECTS=$(SOURCES:.c=.o)
INCFLAGS=$(foreach TMP,$(INCPATHS),-I$(TMP))
LIBFLAGS=$(foreach TMP,$(LIBPATHS),-L$(TMP))
BINARY=mcp-console


all: $(OBJECTS) $(BINARY)


.c.o:
	$(CC) $(INCFLAGS) $(CFLAGS) -fPIC $< -o $@

distclean: clean
	rm -f $(BINARY) $(OBJECTS)

mcp-console: mcp-console.c
	$(CC) -Og -g2 -O0 -o mcp-console mcp-console.c $(INCFLAGS) $(LIBFLAGS) -lmcp23s17

clean:
	rm -f $(OBJECTS) $(BINARY)

install: $(BINARY)
	install mcp-console $${HOME}/local/bin
	

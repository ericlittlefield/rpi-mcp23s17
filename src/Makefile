PROJECT=mcp23s17
SOURCES=mcp23s17.c
LIBRARY=static
INCPATHS= /usr/include
LIBPATHS=
LDFLAGS=
CFLAGS=-c -Wall -Og -g2 -O0
CC=$(CROSS_COMPILE)gcc
AR=$(CROSS_COMPILE)ar

# ------------ MAGIC BEGINS HERE -------------

# Automatic generation of some important lists
OBJECTS=$(SOURCES:.c=.o)
INCFLAGS=$(foreach TMP,$(INCPATHS),-I$(TMP))
LIBFLAGS=$(foreach TMP,$(LIBPATHS),-L$(TMP))

# Set up the output file names for the different output types
ifeq "$(LIBRARY)" "shared"
    BINARY=lib$(PROJECT).so
    LDFLAGS += -shared
else ifeq "$(LIBRARY)" "static"
    BINARY=lib$(PROJECT).a
else
    BINARY=$(PROJECT)
endif

all: $(SOURCES) $(BINARY)

$(BINARY): $(OBJECTS)
	$(AR) rcs $(BINARY) $(OBJECTS)
	
.c.o:
	$(CC) $(INCFLAGS) $(CFLAGS) -fPIC $< -o $@

distclean: clean
	rm -f $(BINARY)


clean:
	rm -f $(OBJECTS) *.a

install: $(BINARY)
	install mcp23s17.h $${HOME}/local/include
	install libmcp23s17.a $${HOME}/local/lib/

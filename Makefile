INCPATHS=
LIBPATHS=
LDFLAGS=
CFLAGS=-c -Wall
CC=gcc

# ------------ MAGIC BEGINS HERE -------------

# Automatic generation of some important lists
all: lib test

lib: clean
	cd src && make
	
test:
	cd tst && make
	



distclean: clean
	cd src && make distclean
	cd tst && make distclean
	rm -f *.a


clean:
	cd src && make clean
	cd tst && make clean

uninstall: distclean
	@test -f /usr/local/include/mcp23s17.h && rm /usr/local/include/mcp23s17.h || true
	@test -f /usr/local/lib/libmcp23s17.a && rm /usr/local/lib/libmcp23s17.a || true
	
install: $(BINARY)
	install src/mcp23s17.h /usr/local/include
	install $(BINARY) /usr/local/lib

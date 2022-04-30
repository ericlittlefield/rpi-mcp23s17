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
	
test: lib install
	cd tst && make
	



distclean: clean
	cd src && make distclean
	cd tst && make distclean
	rm -f *.a


clean:
	cd src && make clean
	cd tst && make clean

uninstall: distclean
	@test -f ~/local/include/mcp23s17.h && rm ~/local/include/mcp23s17.h || true
	@test -f ~/local/lib/libmcp23s17.a && rm ~/local/lib/libmcp23s17.a || true
	
install: $(BINARY)
	
	@test -f ~/local/include || install -d ~/local/include
	@test -f ~/local/lib     || install -d ~/local/lib
	cd src && make install
	

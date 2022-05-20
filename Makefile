ARCH ?= arm64
CROSS_COMPILE ?= ${CROSS_COMPILE}
LIBPATHS=
LDFLAGS=
#SYSROOT=$(SYSROOT_PATH)
export

all: lib test

lib: clean
	cd src && make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)
	
test: lib install
	cd tst && make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)
	
tools: lib install
	cd tools && make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)


distclean: clean
	cd src && make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) distclean
	cd tst && make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) distclean
	rm -f *.a


clean:
	cd src && make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean
	cd tst && make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean
	cd tools && make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean

uninstall: distclean
	@test -f $${HOME}/local/include/mcp23s17.h && rm $${HOME}/local/include/mcp23s17.h || true
	@test -f $${HOME}/local/lib/libmcp23s17.a && rm $${HOME}/local/lib/libmcp23s17.a || true

install: $(BINARY)
	@test -f $${HOME}/local/include || install -d $${HOME}/local/include
	@test -f $${HOME}/local/lib     || install -d $${HOME}/local/lib
	cd src && make install ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)
	

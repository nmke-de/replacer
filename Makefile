
LIBC = /usr/lib/libc.a
obj  = readc.o
mobj = replace.o $(obj)
#tobj = $(obj)

LDFLAGS += -static -s -z norelro -z noseparate-code

build: replace

#check: test/test
#	@cd test/ ; ./test

replace: $(mobj) $(LIBC)
	ld $(LDFLAGS) -o $@ $^

#test/test: $(tobj) $(LIBC)
#	ld $(LDFLAGS) -o $@ $^

clean:
	rm -f scalp *.o

.PHONY: build clean

all: build clean

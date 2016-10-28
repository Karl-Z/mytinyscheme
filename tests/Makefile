TS=$(shell ls *.c | sed 's/\.c$$//')
TSDIR=../tinyscheme-1.41
LIBS=$(TSDIR)/libtinyscheme.a $(TSDIR)/re/libre.a $(TSDIR)/tsx/libtsx.a -lm -ldl
CFLAGS+=-O2 -I$(TSDIR) -I$(TSDIR)/re -I$(TSDIR)/tsx -DCONFIG_FILE=\"$(@).scm\" -DUSE_REGEX -DUSE_TSX
CONFIG_FILE=

%: %.c
	gcc -o $@ $< $(CFLAGS) $(LIBS)

.PHONY: all clean	
all: $(TS)

clean:
	-rm -f $(TS)

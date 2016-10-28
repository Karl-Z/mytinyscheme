TS=$(shell ls *.c | sed 's/\.c$$//')
TSDIR=../tinyscheme-1.41
LIBS=$(TSDIR)/libtinyscheme.a -lm -ldl
CFLAGS+=-I$(TSDIR)
CONFIG_FILE=

%: %.c
	gcc -o $@ $< -DCONFIG_FILE=\"$(@).scm\" $(CFLAGS) $(LIBS)

.PHONY: all clean	
all: $(TS)

clean:
	-rm -f $(TS)

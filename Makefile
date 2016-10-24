SRCURL=http://downloads.sourceforge.net/project/tinyscheme/tinyscheme/tinyscheme-1.41/tinyscheme-1.41.zip
SRCPKG=tinyscheme-1.41.zip
SRCDIR=tinyscheme-1.41
SRCURL1=http://downloads.sourceforge.net/project/tinyscheme/tinyscheme-regex/1.3/re-1.3.tar.gz
SRCPKG1=re-1.3.tar.gz
SRCDIR1=$(SRCDIR)/re
SRCURL2=http://heras-gilsanz.com/manuel/tsx-1.1.tgz
SRCPKG2=tsx-1.1.tgz
SRCDIR2=$(SRCDIR)/tsx-1.1

all: .builded

download: .downloaded
.downloaded: $(SRCPKG) $(SRCPKG1) $(SRCPKG2)
	touch $@

$(SRCPKG):
	wget -c  -O $@ $(SRCURL)
$(SRCPKG1):
	wget -c  -O $@ $(SRCURL1)
$(SRCPKG2):
	wget -c  -O $@ $(SRCURL2)

unpack: .unpacked
.unpacked: .downloaded
	unzip $(SRCPKG)
	tar xzf $(SRCPKG1) -C $(SRCDIR)
	tar xzf $(SRCPKG2) -C $(SRCDIR)
	touch $@

patch: .patched
.patched: .unpacked
	test -e "$(PATCH1)" && cat $(PATCH1) | patch -d $(SRCDIR) -p1 ||:
	touch $@

build: .builded
.builded: .patched
	$(MAKE) -C $(SRCDIR)
	$(MAKE) -C $(SRCDIR1) -f re.makefile  SCHEME_H_DIR=..
	$(MAKE) -C $(SRCDIR2)
	touch $@

.PHONY: clean
clean:
	rm -rf .*ed
	rm -rf $(SRCDIR)

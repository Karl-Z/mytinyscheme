SRCURL=http://downloads.sourceforge.net/project/tinyscheme/tinyscheme/tinyscheme-1.41/tinyscheme-1.41.zip
SRCPKG=tinyscheme-1.41.zip
SRCDIR=tinyscheme-1.41
SRCURL1=http://downloads.sourceforge.net/project/tinyscheme/tinyscheme-regex/1.3/re-1.3.tar.gz
SRCPKG1=re-1.3.tar.gz
SRCDIR1=$(SRCDIR)/re
SRCURL2=http://heras-gilsanz.com/manuel/tsx-1.1.tgz
SRCPKG2=tsx-1.1.tgz
SRCDIR2=$(SRCDIR)/tsx-1.1
PATCHES=patches/0001-add-variable_ref-function.patch
TESTSDIR=tests

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
	for p in $(PATCHES); do test -e "$$p" && cat $$p | patch -d $(SRCDIR) -p1 ||:; done
	touch $@

build: .builded
.builded: .patched
	$(MAKE) -C $(SRCDIR)
	$(MAKE) -C $(SRCDIR1) -f re.makefile  SCHEME_H_DIR=..
	$(MAKE) -C $(SRCDIR2)
	$(MAKE) -C $(SRCDIR) clean && $(MAKE) -C $(SRCDIR) PLATFORM_FEATURES=-DSTANDALONE=0 libtinyscheme.a
	touch $@

test: .tested
.tested: .builded
	$(MAKE) -C $(TESTSDIR)
	touch $@

patches: $(PATCHES)
	-@diff -ZBb -E -u -x '*.o' -x '*.so' -x '*.a' -r tinyscheme-1.41 tinyscheme-1.41.my |grep -v '^Only' 2>/dev/null

.PHONY: clean patches
clean:
	rm -rf .*ed
	rm -rf $(SRCDIR)

SHARED_TARGET = dist/shared/lib45d_conf.so
STATIC_TARGET = dist/static/lib45d_conf.a
CC = g++
CFLAGS = -Wall -Wextra -Isrc/incl -fpic -std=c++11
LIBS =

CFLAGS += $(EXTRA_CFLAGS) # allow additional cflags passed with `make EXTRA_CFLAGS="-flag1 -flag2"`
LIBS += $(EXTRA_LIBS) # allow additional link flags passed with `make EXTRA_LIBS="-flag1 -flag2"`

SOURCE_FILES := $(shell find src/impl -name *.cpp)
OBJECT_FILES := $(patsubst src/impl/%.cpp, build/%.o, $(SOURCE_FILES))
HEADER_FILES := $(shell find src/incl -name *.hpp)

LD_CONF := /etc/ld.so.conf.d/45drives.conf
HEADER_INSTALL_TARGET = /opt/45drives/include/45d_conf.hpp

ifeq ($(PREFIX),)
	PREFIX := /opt/45drives/lib
endif

ifdef DEVEL
default: dev
else
default: shared
endif
all: default

static: $(STATIC_TARGET)
shared: $(SHARED_TARGET)
dev: static shared

$(SHARED_TARGET): $(OBJECT_FILES)
	mkdir -p $(dir $@)
	$(CC) -shared $(OBJECT_FILES) -Wall $(LIBS) -o $@

$(STATIC_TARGET): $(OBJECT_FILES)
	mkdir -p $(dir $@)
	ar crvs $@ $(OBJECT_FILES)

build/%.o: src/impl/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean: clean-build clean-target

clean-build:
	-rm -rf build

clean-target:
	-rm -f $(SHARED_TARGET) $(STATIC_TARGET)

ifdef DEVEL
install: dev $(HEADER_INSTALL_TARGET) install-ld-conf
else
install: default install-ld-conf
endif
	mkdir -p $(DESTDIR)$(PREFIX)
	cp -f $(SHARED_TARGET) $(STATIC_TARGET) $(DESTDIR)$(PREFIX)

install-ld-conf:
	mkdir -p $(dir $(DESTDIR)$(LD_CONF))
	echo "$(PREFIX)" > $(DESTDIR)$(LD_CONF)

# install header
/opt/45drives/include/%.hpp: src/incl/%.hpp
	mkdir -p $(dir $@)
	cp -f $< $@

uninstall:
	-rm -f $(DESTDIR)$(PREFIX)$(SHARED_TARGET) $(DESTDIR)$(PREFIX)$(STATIC_TARGET) $(DESTDIR)$(HEADER_INSTALL_TARGET)

test:
	cd tests && make test

clean-tests:
	cd tests && make clean

docs: api-doc dev-doc

%-doc: doc/%-doc.doxyfile $(HEADER_FILES) doc/main-page.dox
	-rm $@ -rf
	doxygen $<
	mv doc/html $@

clean-docs:
	-rm -rf doc/html
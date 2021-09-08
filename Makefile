SHARED_TARGET = dist/shared/lib45d.so
STATIC_TARGET = dist/static/lib45d.a
CC = g++
CFLAGS = -Wall -Wextra -Isrc/incl -fpic -std=c++11
LIBS =

CFLAGS += $(EXTRA_CFLAGS) # allow additional cflags passed with `make EXTRA_CFLAGS="-flag1 -flag2"`
LIBS += $(EXTRA_LIBS) # allow additional link flags passed with `make EXTRA_LIBS="-flag1 -flag2"`

SOURCE_FILES := $(shell find src/impl -name *.cpp)
OBJECT_FILES := $(patsubst src/impl/%.cpp, build/%.o, $(SOURCE_FILES))
HEADER_FILES := $(shell find src/incl/45d -name *.hpp)

INCLUDE_PREFIX = /usr/include
LIB_PREFIX = /usr/lib

HEADER_INSTALL_TARGETS := $(patsubst src/incl/45d/%.hpp, $(DESTDIR)$(INCLUDE_PREFIX)/45d/%.hpp, $(HEADER_FILES))

ifdef DEVEL
default: dev
else
default: shared
endif
all: default

static: $(STATIC_TARGET)
shared: $(SHARED_TARGET)
dev: static shared

$(SHARED_TARGET): $(OBJECT_FILES) $(HEADER_FILES)
	mkdir -p $(dir $@)
	$(CC) -shared $(OBJECT_FILES) -Wall $(LIBS) -o $@

$(STATIC_TARGET): $(OBJECT_FILES) $(HEADER_FILES)
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
install: dev $(HEADER_INSTALL_TARGETS)
	echo $(HEADER_INSTALL_TARGETS)
else
install: default
endif
	mkdir -p $(DESTDIR)$(LIB_PREFIX)
	cp -f $(SHARED_TARGET) $(DESTDIR)$(LIB_PREFIX)
ifdef DEVEL
	cp -f $(STATIC_TARGET) $(DESTDIR)$(LIB_PREFIX)
endif

# install headers
$(DESTDIR)$(INCLUDE_PREFIX)/45d/%.hpp: src/incl/45d/%.hpp
	mkdir -p $(dir $@)
	cp -f $< $@

uninstall:
	-rm -f $(DESTDIR)$(LIB_PREFIX)/$(notdir $(SHARED_TARGET)) $(DESTDIR)$(LIB_PREFIX)/$(notdir $(STATIC_TARGET)) $(HEADER_INSTALL_TARGETS)

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
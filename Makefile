SHARED_TARGET = dist/shared/lib45d_conf.so
STATIC_TARGET = dist/static/lib45d_conf.a
HEADER_TARGET = src/incl/45d_conf.hpp
CC = g++
CFLAGS = -Wall -Wextra -Isrc/incl -fpic
LIBS =

CFLAGS += $(EXTRA_CFLAGS) # allow additional cflags passed with `make EXTRA_CFLAGS="-flag1 -flag2"`
LIBS += $(EXTRA_LIBS) # allow additional link flags passed with `make EXTRA_LIBS="-flag1 -flag2"`

SOURCE_FILES := $(shell find src/impl -name *.cpp)
OBJECT_FILES := $(patsubst src/impl/%.cpp, build/%.o, $(SOURCE_FILES))

LD_CONF := /etc/ld.so.conf.d/45drives.conf

ifeq ($(PREFIX),)
	PREFIX := /opt/45drives/lib
endif

default: $(SHARED_TARGET)
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

install: $(SHARED_TARGET) install-ld-conf
	mkdir -p $(DESTDIR)$(PREFIX)
	cp -f $(SHARED_TARGET) $(DESTDIR)$(PREFIX)

install-dev: $(SHARED_TARGET) $(STATIC_TARGET) $(HEADER_TARGET) install-ld-conf
	mkdir -p $(DESTDIR)$(PREFIX)
	cp -f $(SHARED_TARGET) $(STATIC_TARGET) $(DESTDIR)$(PREFIX)

install-ld-conf:
	mkdir -p $(dir $(DESTDIR)$(LD_CONF))
	echo "$(PREFIX)" > $(DESTDIR)$(LD_CONF)

# install header
src/incl/%.hpp: /opt/45drives/include/%.hpp
	mkdir -p $(dir $<)
	cp -f $@ $<

# test: build-test
# 	for i in tests/test_confs/*.conf; do \
# 		echo "################ INPUT ################"; \
# 		cat $$i; \
# 		echo "################ OUTPUT ################"; \
# 		dist/tests/test $$i; \
# 	done

# build-test: $(STATIC_TARGET)
# 	mkdir -p dist/tests
# 	$(CC) -g -std=c++17 tests/*.cpp -Isrc/incl -static -Ldist/static -l45d_conf -o dist/tests/test

docs:
	doxygen doc/Doxyfile

clean-docs:
	-rm -rf doc/html
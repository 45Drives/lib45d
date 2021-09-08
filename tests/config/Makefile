LIB_LOCATION = ../..

TEST_SOURCES = $(shell find . -name *.cpp)
TEST_TARGETS = $(patsubst %.cpp, %.test, $(TEST_SOURCES))
TEST_INPUTS = $(shell find . -name *.conf)
TEST_OUTPUTS = $(patsubst %.conf, %.out, $(TEST_INPUTS))

CC = g++
CFLAGS = -g -std=c++11 -Wall -Wextra -I$(LIB_LOCATION)/src/incl
LIBS =-static -L$(LIB_LOCATION)/dist/static -l45d

all: $(TEST_TARGETS)
all: $(TEST_OUTPUTS)

%.test: %.cpp $(LIB_LOCATION)/dist/static/lib45d.a
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

%.out: %.conf
	@$(shell find $(dir $@) -name *.test) $< > $@ 2>&1
	@diff $@ $(patsubst %.out, %.gold, $@) && echo $@ PASSED || echo $@ FAILED

clean:
	-rm -f $(TEST_TARGETS) $(TEST_OUTPUTS)
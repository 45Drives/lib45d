LIB_LOCATION = ../..

TEST_SOURCES = $(wildcard */*.cpp)
TEST_TARGETS = $(patsubst %.cpp, %.test, $(TEST_SOURCES))
TEST_INPUTS = $(wildcard */*.conf)
TEST_OUTPUTS = $(patsubst %.conf, %.out, $(TEST_INPUTS))

CC = g++
CFLAGS = -g -std=c++11 -Wall -Wextra -I$(LIB_LOCATION)/src/incl
LIBS =-static -L$(LIB_LOCATION)/dist/static -l45d

all: $(TEST_OUTPUTS)
test: all

%.test: %.cpp $(LIB_LOCATION)/dist/static/lib45d.a
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

%.out: %.conf %.gold $(filter $(dir $@)%.test, $(TEST_TARGETS))
	@$(filter $(dir $@)%.test, $(TEST_TARGETS)) $< > $@ 2>&1
	@diff $@ $(patsubst %.out, %.gold, $@) && echo $@ PASSED || ( echo $@ FAILED && exit 1 )

clean:
	-rm -f $(TEST_TARGETS) $(TEST_OUTPUTS)
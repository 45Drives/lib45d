TEST_SOURCES = $(shell find . -name *.cpp)
TEST_TARGETS = $(patsubst %.cpp, %.test, $(TEST_SOURCES))
TEST_INPUTS = $(shell find . -name *.conf)
TEST_OUTPUTS = $(patsubst %.conf, %.out, $(TEST_INPUTS))

CC = g++
CFLAGS = -g -std=c++17 -Wall -Wextra -I../src/incl
LIBS =-static -L../dist/static -l45d_conf

all: $(TEST_TARGETS)

%.test: %.cpp ../dist/static/lib45d_conf.a
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

../dist/static/lib45d_conf.a:
	cd .. && make static && cd -

%.out: %.conf
	@$(shell find $(dir $@) -name *.test) $< > $@ 2>&1
	@diff $@ $(patsubst %.out, %.gold, $@) && echo $@ PASSED || echo $@ FAILED

test: clean
test: all $(TEST_OUTPUTS)

clean:
	-rm -f $(TEST_TARGETS) $(TEST_OUTPUTS)
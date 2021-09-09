LIB_LOCATION = ../..

TEST_SOURCES := $(wildcard *.cpp)
TEST_TARGETS := $(patsubst %.cpp, %.test, $(TEST_SOURCES))
TEST_OUTPUTS := $(patsubst %.test, %.out, $(TEST_TARGETS))

CC = g++
CFLAGS = -g -std=c++11 -Wall -Wextra -I$(LIB_LOCATION)/src/incl
LIBS = -static -L$(LIB_LOCATION)/dist/static -l45d

all: print-message $(TEST_TARGETS) $(TEST_OUTPUTS)
	@echo "################################"
	@echo "# CLASS TESTS COMPLETE"
	@echo "################################"
test: all

print-message:
	@echo "################################"
	@echo "# TESTING CLASSES"
	@echo "################################"

%.test: %.cpp $(LIB_LOCATION)/dist/static/lib45d.a
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

%.out: %.test FORCE
	@echo -n "$<: " && ./$< > $@ 2>&1 && echo PASSED || ( echo FAILED && cat $@ && exit 1 )

clean:
	-rm -f $(TEST_TARGETS) $(TEST_OUTPUTS)

FORCE:

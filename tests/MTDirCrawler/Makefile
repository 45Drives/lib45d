LIB_LOCATION = ../..

TEST_SOURCES := $(wildcard *.cpp)
TEST_TARGETS := $(patsubst %.cpp, %.test, $(TEST_SOURCES))
TEST_OUTPUTS := $(patsubst %.test, %.out, $(TEST_TARGETS))

TEST_ENV = ./test_env

CC = g++
CFLAGS = -g -std=c++17 -Wall -Wextra -I$(LIB_LOCATION)/src/incl
LIBS = -static -static-libstdc++ -Wl,--whole-archive -lpthread -Wl,--no-whole-archive

all: $(TEST_ENV) $(TEST_TARGETS)

test: print-message all $(TEST_OUTPUTS)
	@echo -e " #"
	@echo -e " # ${call colour_text,MTDirCrawler TESTS COMPLETE,$(GREEN)}"
	@echo -e " ################################"

print-message:
	@echo -e " ################################"
	@echo -e " # ${call colour_text,TESTING MTDirCrawler,$(CYAN)}"
	@echo -e " #"

$(TEST_ENV): $(TEST_ENV).tar.gz
	@echo Extracting $<
	@tar -xf $<

%.test: %.cpp $(LIB_LOCATION)/dist/static/lib45d.a
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

%.out: %.test FORCE
	@echo -n "$<: " && ./$< "$(shell pwd)/$(TEST_ENV)" '*' $(shell nproc) > $@ 2>&1 && [[ $$(cat $@) == "200" ]] && echo -e "${call colour_text,PASSED,$(GREEN)}" || ( echo -e "${call colour_text,FAILED,$(RED)}" && cat $@ && exit 1 )

clean:
	-rm -rf $(TEST_ENV) $(TEST_TARGETS) $(TEST_OUTPUTS)

FORCE:

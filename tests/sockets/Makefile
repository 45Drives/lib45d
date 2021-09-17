LIB_LOCATION = ../..

TEST_SOURCES := $(wildcard *.cpp)
TEST_TARGETS := $(patsubst %.cpp, %.test, $(TEST_SOURCES))
TEST_OUTPUTS := $(patsubst %.test, %.out, $(TEST_TARGETS))

all: print-message $(TEST_TARGETS) $(TEST_OUTPUTS)
	@echo -e " #"
	@echo -e " # ${call colour_text,SOCKET TESTS COMPLETE,$(GREEN)}"
	@echo -e " ################################"
test: all

print-message:
	@echo -e " ################################"
	@echo -e " # ${call colour_text,RUNNING SOCKET TESTS,$(CYAN)}"
	@echo -e " #"

FORCE:

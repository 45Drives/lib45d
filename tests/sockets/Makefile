LIB_LOCATION = ../..

TEST_SUITES := $(wildcard */.)

TEST_TARGETS := $(patsubst %.cpp, %.test, $(wildcard */*.cpp))

all: build-tests

test: all print-message run-tests
	@echo -e " #"
	@echo -e " # ${call colour_text,SOCKET TESTS COMPLETE,$(GREEN)}"
	@echo -e " ################################"

print-message:
	@echo -e " ################################"
	@echo -e " # ${call colour_text,RUNNING SOCKET TESTS,$(CYAN)}"
	@echo -e " #"

build-tests: $(TEST_TARGETS)

$(TEST_TARGETS):
	@for SUITE in $(TEST_SUITES); do $(MAKE) -C $$SUITE --no-print-directory || exit 1; done

run-tests:
	@for SUITE in $(TEST_SUITES); do $(MAKE) -C $$SUITE test --no-print-directory || exit 1; done

clean:
	@for SUITE in $(TEST_SUITES); do $(MAKE) -C $$SUITE clean; done

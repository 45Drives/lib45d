TEST_SUITES := $(wildcard */.)

export RED=1;31m
export GREEN=1;32m
export YELLOW=1;33m
export CYAN=1;96m

define colour_text
\\033[$$(2)$$(1)\\033[0m
endef
export colour_text

define colour_text_top
\033[$(2)$(1)\033[0m
endef

all: print-build-message build-tests
	@echo -e "#"
	@echo -e "# ${call colour_text_top,TESTS BUILT,$(GREEN)}"
	@echo -e "################################"

print-build-message:
	@echo -e "################################"
	@echo -e "# ${call colour_text_top,BUILDING TESTS,$(CYAN)}"
	@echo -e "#"

test: all print-test-message run-tests
	@echo -e "#"
	@echo -e "# ${call colour_text_top,TESTS COMPLETE,$(GREEN)}"
	@echo -e "################################"

print-test-message:
	@echo -e "################################"
	@echo -e "# ${call colour_text_top,RUNNING TESTS,$(CYAN)}"
	@echo -e "#"

build-tests:
	@for SUITE in $(TEST_SUITES); do $(MAKE) -C $$SUITE --no-print-directory || exit 1; done

run-tests:
	@for SUITE in $(TEST_SUITES); do $(MAKE) -C $$SUITE test --no-print-directory || exit 1; done

clean:
	@for SUITE in $(TEST_SUITES); do $(MAKE) -C $$SUITE clean; done

FORCE:

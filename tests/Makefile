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

all: print-message $(TEST_SUITES)
	@echo -e "#"
	@echo -e "# ${call colour_text_top,TESTS COMPLETE,$(GREEN)}"
	@echo -e "################################"

print-message:
	@echo -e "################################"
	@echo -e "# ${call colour_text_top,RUNNING TESTS,$(CYAN)}"
	@echo -e "#"

$(TEST_SUITES): FORCE
	@$(MAKE) -C $@ --no-print-directory

clean:
	for SUITE in $(TEST_SUITES); do $(MAKE) -C $$SUITE clean; done

FORCE:


LIB_LOCATION = ../..

TEST_SUITES := $(wildcard */.)

all: print-message $(TEST_SUITES)
	@echo -e " #"
	@echo -e " # ${call colour_text,SOCKET TESTS COMPLETE,$(GREEN)}"
	@echo -e " ################################"
test: all

print-message:
	@echo -e " ################################"
	@echo -e " # ${call colour_text,RUNNING SOCKET TESTS,$(CYAN)}"
	@echo -e " #"

$(TEST_SUITES): FORCE
	@$(MAKE) -C $@ --no-print-directory

clean:
	for SUITE in $(TEST_SUITES); do $(MAKE) -C $$SUITE clean; done

FORCE:

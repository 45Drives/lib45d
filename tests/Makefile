TEST_SUITES := $(wildcard */.)

all: print-message $(TEST_SUITES)
	@echo "#"
	@echo "# TESTS COMPLETE"
	@echo "################################"

print-message:
	@echo "################################"
	@echo "# RUNNING TESTS"
	@echo "#"

$(TEST_SUITES): FORCE
	@$(MAKE) -C $@ --no-print-directory

clean:
	for SUITE in $(TEST_SUITES); do $(MAKE) -C $$SUITE clean; done

FORCE:


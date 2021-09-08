TEST_SUITES = $(wildcard */.)

$(TEST_SUITES): FORCE
	$(MAKE) -C $@

clean:
	for SUITE in $(TEST_SUITES); do $(MAKE) -C $$SUITE clean; done

FORCE:


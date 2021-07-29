TESTS_SOURCE = $(shell find . -name *.cpp)
TESTS_TARGET = $(patsubst %.cpp, %.test, $(TESTS_SOURCE))

all: TESTS_TARGET

%.test: %.cpp ../dist/static/lib45d_conf.a
	mkdir -p $(dir $@)
	$(CC) -g -std=c++17 $@ -I../src/incl -static -L../dist/static -l45d_conf -o $<

../dist/static/lib45d_conf.a:
	cd .. && make static && cd -
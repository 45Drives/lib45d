LIB_LOCATION := ../../..

CLIENT_SOURCE := client.cpp
SERVER_SOURCE := server.cpp
CLIENT := $(patsubst %.cpp,%.test,$(CLIENT_SOURCE))
SERVER := $(patsubst %.cpp,%.test,$(SERVER_SOURCE))

CC = g++
CFLAGS = -g -std=c++11 -Wall -Wextra -I$(LIB_LOCATION)/src/incl
LIBS = -static -L$(LIB_LOCATION)/dist/static -l45d

test: $(CLIENT) $(SERVER)
	@echo -n "unix domain socket: " && ./test.sh && echo -e "${call colour_text,PASSED,$(GREEN)}" || ( echo -e "${call colour_text,FAILED,$(RED)}" && cat client.out server.out && exit 1 )

%.test: %.cpp $(LIB_LOCATION)/dist/static/lib45d.a
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

clean:
	-rm -f $(CLIENT) $(SERVER) *.out *.socket

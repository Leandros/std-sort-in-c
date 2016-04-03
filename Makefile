CC		= clang
CXX		= clang++
CFLAGS	= -O0 -march=native -lm

all: sortcc sortc

sortcc: sort.cc
	$(CXX) -o $@ $(CFLAGS) $^

sortc: sort.c
	$(CC) -o $@ $(CFLAGS) $^

.PHONY: clean
clean:
	rm -f sortcc sortc

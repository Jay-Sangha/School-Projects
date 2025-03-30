CC = clang
CFLAGS = -Werror -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -pedantic
LFLAGS = -lm


EXEC = calc

OBJS = calc.o stack.o mathlib.o operators.o

TEST_OBJS = tests.o mathlib.o

TEST_FILE = sin.csv cos.csv tan.csv

all: calc test

calc : $(OBJS)
	$(CC) $(LFLAGS) -o calc $(OBJS)


test: $(TEST_OBJS)
	$(CC) $(LFLAGS) -o test $(TEST_OBJS)

%.o: %.c 
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f calc test $(OBJS) $(TEST_OBJS) $(TEST_FILE)

format:
	clang-format -i -style=file *.[ch]


scan-build: clean
	scan-build --use-cc=$(CC) make


.PHONY: all clean format



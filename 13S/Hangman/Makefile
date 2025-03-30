CC = clang
CFLAGS = -Werror -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -pedantic
LFLAGS = 
EXEC = hangman
HEADERS = hangman_helpers.h

all: $(EXEC)

$(EXEC): $(EXEC).o hangman_helpers.o
	$(CC) $^ $(LFLAGS) -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

tests: test_functionality test_helpers






test_functionaliyty: test_functionality.sh
	./test_functionality.sh

test_helpers: test_helpers.c
	$(CC) -o test_helpers test_helpers.c
	./test_helpers

clean:
	rm -rf $(EXEC) *.o test_helpers

format:
	clang-format -i -style=file *.[ch]

.PHONY: all clean format



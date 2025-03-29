CC=clang
CFLAGS = -Werror -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -pedantic

HUFFBIN= huff
HUFFOBJS= bitwriter.o bitreader.o node.o pq.o huff.o
DEHUFFBIN = dehuff
DEHUFFOBJS= bitreader.o bitwriter.o node.o pq.o dehuff.o

BRTEST= brtest
BROBJS= bitreader.o brtest.o

BWTEST= bwtest
BWOBJS= bitwriter.o bwtest.o

NODETEST= nodetest
NODEOBJS= node.o nodetest.o

PQTEST= pqtest
PQOBJS= node.o pq.o pqtest.o

all: huff dehuff brtest bwtest nodetest pqtest

$(HUFFBIN): $(HUFFOBJS)
	$(CC) -o $@ $^

$(DEHUFFBIN): $(DEHUFFOBJS)
	$(CC) -o $@ $^

$(BRTEST): $(BROBJS)
	$(CC) -o $@ $^

$(BWTEST): $(BWOBJS)
	$(CC) -o $@ $^
$(NODETEST): $(NODEOBJS)
	$(CC) -o $@ $^

$(PQTEST): $(PQOBJS)
	$(CC) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(HUFFBIN) $(HUFFOBJS) $(DEHUFFBIN) $(DEHUFFOBJS) $(BRTEST) $(BROBJS) $(BWTEST) $(BWOBJS) $(NODETEST) $(NODEOBJS) $(PQTEST) $(PQOBJS)

format:
	clang-format -i --style=file *.[ch]

.PHONY: all debug release clean

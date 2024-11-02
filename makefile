CC=gcc
CFLAGS=-I.
LFLAGS=-lusb-1.0
MNAME=RR_Infector
MSRC=main

$(MNAME): $(MSRC).o
	$(CC) -o $@ $(MSRC).o $(LFLAGS)

$(MSRC).o: $(MSRC).c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm $(MNAME) $(MSRC).o


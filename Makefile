INCLUDEDIR=./Includes
AESDIR=./AES
CFLAGS=-std=c11 -Wall -Wextra -g
CPPFLAGS=-I$(INCLUDEDIR)
.PHONY= all clean remove help # test

all: main

main: main.o AES.o SubBytes.o ShiftRows.o MixColumns.o KeyExpansion.o
	$(CC) $(CFLAGS) -o aes $^

main.o: main.c $(INCLUDEDIR)/main.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $^

AES.o: $(AESDIR)/AES.c $(INCLUDEDIR)/AES.h 
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $^

SubBytes.o: $(AESDIR)/SubBytes.c $(INCLUDEDIR)/SubBytes.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $^

ShiftRows.o: $(AESDIR)/ShiftRows.c $(INCLUDEDIR)/ShiftRows.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $^	

MixColumns.o: $(AESDIR)/MixColumns.c $(INCLUDEDIR)/MixColumns.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $^

KeyExpansion.o:  $(AESDIR)/KeyExpansion.c $(INCLUDEDIR)/KeyExpansion.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $^

clean:
	@rm -f *.o $(INCLUDEDIR)/*.gch 

remove: clean
	@rm -f aes

# test : hitori hitori_test
# 	./prog_test
# 	valgrind -s ./hitori -a ../tests/grids_web/solving_x_solutions.txt
# 	valgrind -s ./hitori -g8


help: 
	@echo "make all : build the project"
	@echo "make clean : remove intermediate files of the compilation"
	@echo "make remove : like make clean but remove also the main binary"
#	@echo "make test : test the program"
	@echo "make help : print this help"


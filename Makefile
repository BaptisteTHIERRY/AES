INCLUDEDIR=./Includes
AESDIR=./AES
MODEDIR=./Modes
CFLAGS=-std=c11 -Wall -Wextra -g -O3
CPPFLAGS=-I$(INCLUDEDIR)
.PHONY= all clean remove help # test

all: main

main: main.o AES.o SubBytes.o ShiftRows.o MixColumns.o KeyExpansion.o ECB.o blockXOR.o CBC.o CFB.o
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

ECB.o: $(MODEDIR)/ECB.c $(INCLUDEDIR)/ECB.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $^

blockXOR.o: $(MODEDIR)/blockXOR.c $(INCLUDEDIR)/blockXOR.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $^

CBC.o: $(MODEDIR)/CBC.c $(INCLUDEDIR)/CBC.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $^

CFB.o: $(MODEDIR)/CFB.c $(INCLUDEDIR)/CFB.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $^

clean:
	@rm -f *.o $(INCLUDEDIR)/*.gch 

remove: clean
	@rm -f aes test

test: AES.o SubBytes.o ShiftRows.o MixColumns.o KeyExpansion.o ECB.o test.o
	$(CC) $(CFLAGS) -o test $^
	@echo "Starting the test"
	@./test
	@echo "End of the test"
	@make clean

test.o: ./Tests/test.c $(INCLUDEDIR)/test.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $^

test_ECB : main
	@./aes -i ./Tests/alice.sage -o ./Tests/alice.crypt
	@./aes -d -i ./Tests/alice.crypt -o ./Tests/alice.decrypt
	@( if [[ ! '$(diff -q ./Tests/alice.sage ./Tests/alice.decrypt)' = '' ]]; \
		then echo "Erreur lors du chiffrement/dechiffrement"; \
	  else \
	  	echo "Chiffrement/Dechiffrement réussi sans erreurs"; \
	  fi \
	)
	@make clean

test_CBC: main
	@./aes -i ./Tests/alice.sage -o ./Tests/alice.crypt -m CBC -I aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
	@./aes -d -i ./Tests/alice.crypt -o ./Tests/alice.decrypt -m CBC -I aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
	@( if [[ ! '$(diff -q ./Tests/alice.sage ./Tests/alice.decrypt)' = '' ]]; \
		then echo "Erreur lors du chiffrement/dechiffrement"; \
	  else \
	  	echo "Chiffrement/Dechiffrement réussi sans erreurs"; \
	  fi \
	)
	@make clean

test_CFB: main
	@./aes -i ./Tests/alice.sage -o ./Tests/alice.crypt -m CFB -I aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
	@./aes -d -i ./Tests/alice.crypt -o ./Tests/alice.decrypt -m CFB -I aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
	@( if [[ ! '$(diff -q ./Tests/alice.sage ./Tests/alice.decrypt)' = '' ]]; \
		then echo "Erreur lors du chiffrement/dechiffrement"; \
	  else \
	  	echo "Chiffrement/Dechiffrement réussi sans erreurs"; \
	  fi \
	)
	@make clean

help: 
	@echo "make all : build the project"
	@echo "make clean : remove intermediate files of the compilation"
	@echo "make remove : like clean but remove also the binary"
	@echo "make test : test the program and compute execution time"
	@echo "make help : print this help"


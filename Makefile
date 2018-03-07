# NAME: Virgil Jose, Christopher Ngai
# EMAIL: virgil@ucla.edu, cngai1223@gmail.com
# ID: 904765891, 404795904

CC = gcc
CFLAGS = -Wall -Wextra

default:
	$(CC) $(CFLAGS) -o lab3a lab3a.c

dist:
	tar -czf lab3a-404795904.tar.gz lab3a.c Makefile README

clean:
	rm -rf lab3a lab3a-404795904.tar.gz
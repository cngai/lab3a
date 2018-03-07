# NAME: Virgil Jose, Christopher Ngai
# EMAIL: virgil@ucla.edu, cngai1223@gmail.com
# ID: 000000000, 000000000

CC = gcc
CFLAGS = -Wall -Wextra

default:
	$(CC) $(CFLAGS) -o lab3a lab3a.c

dist:
	tar -czf lab3a-404795904.tar.gz lab3a.c Makefile README ext2_fs.h

clean:
	rm -rf lab3a lab3a-404795904.tar.gz

CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
   
all: s21_cat s21_grep

s21_cat: cat/s21_cat.o
	${CC} cat/s21_cat.o -o cat/s21_cat

s21_cat.o: s21_cat.c
	${CC} ${CFLAGS} -c cat/s21_cat.c -o cat/s21_cat.o

s21_grep: grep/s21_grep.o
	${CC} grep/s21_grep.o -o grep/s21_grep

s21_grep.o: s21_grep.o
	${CC} ${CFLAGS} -c grep/s21_grep.c -o grep/s21_grep.o

check_cat:
	cppcheck cat/s21_cat.c
	CK_FORK=no leaks --atExit -- cat/s21_cat

check_grep:
	cppcheck grep/s21_grep.c
	CK_FORK=no leaks --atExit -- grep/s21_grep

test: clean_test test_cat test_grep 
	${CC} ${CFLAGS} -c test.c -o test.o
	${CC} test.o -o test -lcheck
	./test

test_cat:
	cat/s21_cat -b cat/test.txt >> cat/cat_full.txt
	cat/s21_cat -n cat/test.txt >> cat/cat_full.txt
	cat/s21_cat -b -n cat/test.txt >> cat/cat_full.txt
	cat/s21_cat -t cat/test.txt >> cat/cat_full.txt
	cat/s21_cat -e cat/test.txt >> cat/cat_full.txt

test_grep:
	grep/s21_grep 13 grep/test.txt >> grep/grep_full.txt
	grep/s21_grep -e he -e k grep/test.txt >> grep/grep_full.txt
	grep/s21_grep n -i grep/test.txt >> grep/grep_full.txt
	grep/s21_grep a -v grep/test.txt >> grep/grep_full.txt
	grep/s21_grep -e a -e e -v grep/test.txt >> grep/grep_full.txt
	grep/s21_grep 13 -c grep/test.txt >> grep/grep_full.txt
	grep/s21_grep -c -e v -e 13 grep/test.txt >> grep/grep_full.txt
	grep/s21_grep -c -e v -e 13 -v grep/test.txt >> grep/grep_full.txt
	grep/s21_grep s -l grep/test.txt >> grep/grep_full.txt
	grep/s21_grep s -l -v grep/test.txt >> grep/grep_full.txt
	grep/s21_grep a -n grep/test.txt >> grep/grep_full.txt
	grep/s21_grep v -h grep/test.txt >> grep/grep_test_file.txt

clean_test:
	rm -rf cat/*file.txt
	rm -rf grep/*file.txt
	rm -rf test

clean:
	rm -rf cat/s21_cat.o grep/s21_grep.o
	rm -rf cat/s21_cat grep/s21_grep
	rm -rf a.out
	rm -rf *.o
	rm -rf test
	
rebuild: clean all

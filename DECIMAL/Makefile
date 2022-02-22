CC = gcc
CFLAGS = -Wall -Werror -Wextra
FLAG = --coverage
CFILE = s21_add.c s21_sub.c s21_mul.c s21_div.c s21_mod.c s21_is_less.c s21_is_less_or_equal.c \
		s21_is_greater.c s21_is_greater_or_equal.c s21_is_equal.c s21_is_not_equal.c \
		s21_from_int_to_decimal.c s21_from_float_to_decimal.c s21_from_decimal_to_int.c \
		s21_from_decimal_to_float.c s21_floor.c s21_round.c s21_truncate.c s21_negate.c
OFILE = s21_add.o s21_sub.o s21_mul.o s21_div.o s21_mod.o s21_is_less.o s21_is_less_or_equal.o \
		s21_is_greater.o s21_is_greater_or_equal.o s21_is_equal.o s21_is_not_equal.o \
		s21_from_int_to_decimal.o s21_from_float_to_decimal.o s21_from_decimal_to_int.o \
		s21_from_decimal_to_float.o s21_floor.o s21_round.o s21_truncate.o s21_negate.o
OS := $(shell uname -s)

all: clean s21_decimal.a test

s21_add.o:
	$(CC) $(CFLAGS) -c s21_add.c -o s21_add.o

s21_sub.o:
	$(CC) $(CFLAGS) -c s21_sub.c -o s21_sub.o

s21_mul.o:
	$(CC) $(CFLAGS) -c s21_mul.c -o s21_mul.o

s21_div.o:
	$(CC) $(CFLAGS) -c s21_div.c -o s21_div.o

s21_mod.o:
	$(CC) $(CFLAGS) -c s21_mod.c -o s21_mod.o

s21_is_less.o:
	$(CC) $(CFLAGS) -c s21_is_less.c -o s21_is_less.o

s21_is_less_or_equal.o:
	$(CC) $(CFLAGS) -c s21_is_less_or_equal.c -o s21_is_less_or_equal.o

s21_is_greater.o:
	$(CC) $(CFLAGS) -c s21_is_greater.c -o s21_is_greater.o

s21_is_greater_or_equal.o:
	$(CC) $(CFLAGS) -c s21_is_greater_or_equal.c -o s21_is_greater_or_equal.o

s21_is_equal.o:
	$(CC) $(CFLAGS) -c s21_is_equal.c -o s21_is_equal.o

s21_is_not_equal.o:
	$(CC) $(CFLAGS) -c s21_is_not_equal.c -o s21_is_not_equal.o

s21_from_int_to_decimal.o:
	$(CC) $(CFLAGS) -c s21_from_int_to_decimal.c -o s21_from_int_to_decimal.o

s21_from_float_to_decimal.o:
	$(CC) $(CFLAGS) -c s21_from_float_to_decimal.c -o s21_from_float_to_decimal.o

s21_from_decimal_to_int.o:
	$(CC) $(CFLAGS) -c s21_from_decimal_to_int.c -o s21_from_decimal_to_int.o

s21_from_decimal_to_float.o:
	$(CC) $(CFLAGS) -c s21_from_decimal_to_float.c -o s21_from_decimal_to_float.o

s21_floor.o:
	$(CC) $(CFLAGS) -c s21_floor.c -o s21_floor.o

s21_round.o:
	$(CC) $(CFLAGS) -c s21_round.c -o s21_round.o

s21_truncate.o:
	$(CC) $(CFLAGS) -c s21_truncate.c -o s21_truncate.o

s21_negate.o:
	$(CC) $(CFLAGS) -c s21_negate.c -o s21_negate.o

add_function.o:
	$(CC) $(CFLAGS) -c add_function.c -o add_function.o

s21_decimal.a: $(OFILE)
	ar rcs s21_decimal.a $(OFILE)
	cp s21_decimal.a libs21_decimal
	cp s21_decimal.a ls21_decimal

test: s21_math.a
ifeq ($(OS), Darwin)
	$(CC) $(CFLAGS) -c test.c -o test.o -lcheck
	$(CC) $(CFLAGS) test.o s21_decimal.a -o test -lcheck
	./test
else
	$(CC) $(CFLAGS) test.c s21_decimal.a -o test -lcheck -lsubunit -lrt -lpthread -lm
	./test
endif

gcov:
ifeq ($(OS), Darwin)
	$(CC) $(CFLAGS) $(FLAG) $(CFILE) $(LCOV) -o test -lcheck
else
	$(CC) $(CFLAGS) $(FLAG) $(CFILE) $(LCOV) -o test -lcheck -lsubunit -lrt -lpthread -lm
endif

gcov_report: gcov
	./test
	lcov -t “test” -o test.info -c -d .
	genhtml -o report test.info
	open report/index.html

clean:
	rm -rf *o  *.gc* report *.a test libs21_decimal ls21_decimal

cpp:
	cp ../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../materials/linters/cpplint.py --extensions=c *.h *.c
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h
	rm -rf CPPLINT.cfg


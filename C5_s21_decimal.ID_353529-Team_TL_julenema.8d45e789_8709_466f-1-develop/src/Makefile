OFLAGS = -std=c11 -Wall -Wextra -Werror -pedantic
TFLAGS = -lcheck -lm -lpthread -lsubunit
#lcov для создаение отчета (нужно устанавливать)

SOURCES = s21_decimal.c s21_bank_round.c s21_mantissa.c
OBJECTS = $(SOURCES:.c=.o)

all: s21_decimal.a

s21_decimal.a: $(OBJECTS)
	ar rc s21_decimal.a $(OBJECTS)
	ranlib s21_decimal.a

%.o: %.c
	gcc $(OFLAGS) -c $< -o $@

test: s21_decimal_test
	chmod +x ./s21_decimal_test
	./s21_decimal_test

s21_decimal_test: tests/s21_decimal_test.c s21_decimal.a
	gcc $(OFLAGS) -o s21_decimal_test tests/s21_decimal_test.c s21_decimal.a $(TFLAGS)

gcov_report: $(SOURCES) tests/s21_decimal_test.c
	gcc $(OFLAGS) --coverage -o test_gcov $(SOURCES) tests/s21_decimal_test.c $(TFLAGS)

	chmod +x ./test_gcov
	./test_gcov

	lcov -t "s21_decimal_test" -o s21_decimal.info -c -d .

	lcov --remove s21_decimal.info '*/tests/*' -o s21_decimal_filtered.info --ignore-errors unused

	genhtml -o report s21_decimal_filtered.info

	rm -f *.gcda *.gcno test_gcov

clean: 
	rm -rf *.o *.gch s21_decimal s21_decimal_test s21_decimal.a  test_gcov *.gcda *.gcno *.info report/ 

rebuild: clean all

.PHONY: all test clean rebuild gcov_report
	
#include "../s21_decimal.h"

#include <check.h>
#include <limits.h>
#include <math.h>

#include "../s21_mantissa.h"

#pragma region s21_add
// сложение положительных
START_TEST(test_add_positive_positive) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(5, &a);
  s21_from_int_to_decimal(3, &b);
  ck_assert_int_eq(s21_add(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, 8);
}
END_TEST

// сложение двух отрицательных
START_TEST(test_add_negative_negative) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(-5, &a);
  s21_from_int_to_decimal(-3, &b);
  ck_assert_int_eq(s21_add(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, -8);
}
END_TEST

// положительное + отрицательное
START_TEST(test_add_positive_negative) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(5, &a);
  s21_from_int_to_decimal(-3, &b);
  ck_assert_int_eq(s21_add(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, 2);
}
END_TEST

// отрицательное + положительное
START_TEST(test_add_negative_positive) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(-5, &a);
  s21_from_int_to_decimal(3, &b);
  ck_assert_int_eq(s21_add(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, -2);
}
END_TEST

// результат 0
START_TEST(test_add_zero_result) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(5, &a);
  s21_from_int_to_decimal(-5, &b);
  ck_assert_int_eq(s21_add(a, b, &res), 0);
  ck_assert_int_eq(s21_is_mantissa_zero(res), 1);
  ck_assert_int_eq(GET_SIGN(res), 0);
}
END_TEST

// сложение с нулём
START_TEST(test_add_with_zero) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(42, &a);
  s21_from_int_to_decimal(0, &b);
  ck_assert_int_eq(s21_add(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, 42);
}
END_TEST

// числа с разным scale
START_TEST(test_add_different_scale) {
  s21_decimal a = {{15, 0, 0, 0}};  // 1.5
  SET_SCALE(a, 1);
  s21_decimal b = {{235, 0, 0, 0}};  // 2.35
  SET_SCALE(b, 2);
  s21_decimal res;
  ck_assert_int_eq(s21_add(a, b, &res), 0);
  ck_assert_int_eq(GET_SCALE(res), 2);
  float out;
  s21_from_decimal_to_float(res, &out);
  ck_assert_float_eq_tol(out, 3.85f, 1e-3f);
}
END_TEST

// NULL результат
START_TEST(test_add_null_result) {
  s21_decimal a, b;
  s21_from_int_to_decimal(1, &a);
  s21_from_int_to_decimal(2, &b);
  ck_assert_int_eq(s21_add(a, b, NULL), 1);
}
END_TEST
#pragma endregion

#pragma region s21_sub
// вычитание положительных
START_TEST(test_sub_positive_positive) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(10, &a);
  s21_from_int_to_decimal(3, &b);
  ck_assert_int_eq(s21_sub(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, 7);
}
END_TEST

// вычитание отрицательных
START_TEST(test_sub_negative_negative) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(-10, &a);
  s21_from_int_to_decimal(-3, &b);
  ck_assert_int_eq(s21_sub(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, -7);
}
END_TEST

// вычитание отрицательного
START_TEST(test_sub_positive_negative) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(5, &a);
  s21_from_int_to_decimal(-3, &b);
  ck_assert_int_eq(s21_sub(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, 8);
}
END_TEST

// вычитание положительного из отрицательного
START_TEST(test_sub_negative_positive) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(-5, &a);
  s21_from_int_to_decimal(3, &b);
  ck_assert_int_eq(s21_sub(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, -8);
}
END_TEST

// вычитание нуля
START_TEST(test_sub_zero) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(42, &a);
  s21_from_int_to_decimal(0, &b);
  ck_assert_int_eq(s21_sub(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, 42);
}
END_TEST

// вычитание из нуля
START_TEST(test_sub_from_zero) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(0, &a);
  s21_from_int_to_decimal(5, &b);
  ck_assert_int_eq(s21_sub(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, -5);
}
END_TEST

// дробные числа с разным scale
START_TEST(test_sub_different_scale) {
  s21_decimal a = {{500, 0, 0, 0}};
  SET_SCALE(a, 2);
  s21_decimal b = {{15, 0, 0, 0}};
  SET_SCALE(b, 1);
  s21_decimal res;
  ck_assert_int_eq(s21_sub(a, b, &res), 0);
  ck_assert_int_eq(GET_SCALE(res), 2);
  float out;
  s21_from_decimal_to_float(res, &out);
  ck_assert_float_eq_tol(out, 3.5f, 1e-3f);
}
END_TEST

// NULL результат
START_TEST(test_sub_null_result) {
  s21_decimal a, b;
  s21_from_int_to_decimal(1, &a);
  s21_from_int_to_decimal(2, &b);
  ck_assert_int_eq(s21_sub(a, b, NULL), 1);
}
END_TEST
#pragma endregion

#pragma region s21_mul
// 7 * 8
START_TEST(test_mul_simple_positive) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(7, &a);
  s21_from_int_to_decimal(8, &b);
  ck_assert_int_eq(s21_mul(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, 56);
}
END_TEST

// -3 * 4
START_TEST(test_mul_negative_positive) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(-3, &a);
  s21_from_int_to_decimal(4, &b);
  ck_assert_int_eq(s21_mul(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, -12);
}
END_TEST

// 3 * -4
START_TEST(test_mul_positive_negative) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(3, &a);
  s21_from_int_to_decimal(-4, &b);
  ck_assert_int_eq(s21_mul(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, -12);
}
END_TEST

// минус на минус
START_TEST(test_mul_negative_negative) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(-3, &a);
  s21_from_int_to_decimal(-4, &b);
  ck_assert_int_eq(s21_mul(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, 12);
}
END_TEST

// 0 слева
START_TEST(test_mul_zero_left) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(0, &a);
  s21_from_int_to_decimal(12345, &b);
  ck_assert_int_eq(s21_mul(a, b, &res), 0);
  ck_assert_int_eq(s21_is_mantissa_zero(res), 1);
}
END_TEST

// 0 справа
START_TEST(test_mul_zero_right) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(12345, &a);
  s21_from_int_to_decimal(0, &b);
  ck_assert_int_eq(s21_mul(a, b, &res), 0);
  ck_assert_int_eq(s21_is_mantissa_zero(res), 1);
}
END_TEST

// оба нули
START_TEST(test_mul_zero_zero) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(0, &a);
  s21_from_int_to_decimal(0, &b);
  ck_assert_int_eq(s21_mul(a, b, &res), 0);
  ck_assert_int_eq(s21_is_mantissa_zero(res), 1);
  ck_assert_int_eq(GET_SIGN(res), 0);
}
END_TEST

// умножение на 1
START_TEST(test_mul_by_one) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(1, &a);
  s21_from_int_to_decimal(424242, &b);
  ck_assert_int_eq(s21_mul(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, 424242);
}
END_TEST

// 0.1 * 0.1
START_TEST(test_mul_scale_simple) {
  s21_decimal a = {{1, 0, 0, 0}};
  SET_SCALE(a, 1);
  s21_decimal b = {{1, 0, 0, 0}};
  SET_SCALE(b, 1);
  s21_decimal res;
  ck_assert_int_eq(s21_mul(a, b, &res), 0);
  ck_assert_int_eq(res.bits[0], 1);
  ck_assert_int_eq(GET_SCALE(res), 2);
}
END_TEST

// 1.5 * 2
START_TEST(test_mul_scaled_by_int) {
  s21_decimal a = {{15, 0, 0, 0}};
  SET_SCALE(a, 1);
  s21_decimal b;
  s21_from_int_to_decimal(2, &b);
  s21_decimal res;
  ck_assert_int_eq(s21_mul(a, b, &res), 0);
  float out;
  s21_from_decimal_to_float(res, &out);
  ck_assert_float_eq_tol(out, 3.0f, 1e-6f);
}
END_TEST

// 2.5 * -4
START_TEST(test_mul_scaled_negative) {
  s21_decimal a = {{25, 0, 0, 0}};
  SET_SCALE(a, 1);
  s21_decimal b;
  s21_from_int_to_decimal(-4, &b);
  s21_decimal res;
  ck_assert_int_eq(s21_mul(a, b, &res), 0);
  float out;
  s21_from_decimal_to_float(res, &out);
  ck_assert_float_eq_tol(out, -10.0f, 1e-6f);
}
END_TEST

// 0.05 * 0.04
START_TEST(test_mul_small_fractions) {
  s21_decimal a = {{5, 0, 0, 0}};
  SET_SCALE(a, 2);
  s21_decimal b = {{4, 0, 0, 0}};
  SET_SCALE(b, 2);
  s21_decimal res;
  ck_assert_int_eq(s21_mul(a, b, &res), 0);
  float out;
  s21_from_decimal_to_float(res, &out);
  ck_assert_float_eq_tol(out, 0.002f, 1e-6f);
}
END_TEST

// max * 1
START_TEST(test_mul_max_by_one) {
  s21_decimal a = {{(int)0xFFFFFFFF, (int)0xFFFFFFFF, (int)0xFFFFFFFF, 0}};
  s21_decimal b;
  s21_from_int_to_decimal(1, &b);
  s21_decimal res;
  ck_assert_int_eq(s21_mul(a, b, &res), 0);
  ck_assert_uint_eq((unsigned int)res.bits[0], 0xFFFFFFFF);
  ck_assert_uint_eq((unsigned int)res.bits[1], 0xFFFFFFFF);
  ck_assert_uint_eq((unsigned int)res.bits[2], 0xFFFFFFFF);
}
END_TEST

// переполнение
START_TEST(test_mul_overflow) {
  s21_decimal a = {{(int)0xFFFFFFFF, (int)0xFFFFFFFF, (int)0xFFFFFFFF, 0}};
  s21_decimal b;
  s21_from_int_to_decimal(2, &b);
  s21_decimal res;
  ck_assert_int_eq(s21_mul(a, b, &res), 1);
}
END_TEST

// переполнение с минусом
START_TEST(test_mul_overflow_negative) {
  s21_decimal a = {{(int)0xFFFFFFFF, (int)0xFFFFFFFF, (int)0xFFFFFFFF, 0}};
  SET_SIGN(a, 1);
  s21_decimal b;
  s21_from_int_to_decimal(2, &b);
  s21_decimal res;
  ck_assert_int_eq(s21_mul(a, b, &res), 2);
}
END_TEST

// перенос в старшее слово
START_TEST(test_mul_carry_to_high) {
  s21_decimal a = {{(int)0xFFFFFFFF, 0, 0, 0}};
  s21_decimal b;
  s21_from_int_to_decimal(2, &b);
  s21_decimal res;
  ck_assert_int_eq(s21_mul(a, b, &res), 0);
  ck_assert_uint_eq((unsigned int)res.bits[0], 0xFFFFFFFE);
  ck_assert_int_eq(res.bits[1], 1);
}
END_TEST

// NULL результат
START_TEST(test_mul_null_result) {
  s21_decimal a, b;
  s21_from_int_to_decimal(2, &a);
  s21_from_int_to_decimal(3, &b);
  ck_assert_int_eq(s21_mul(a, b, NULL), 1);
}
END_TEST

// -5 * 0, проверка что знак сбрасывается
START_TEST(test_mul_zero_sign_clean) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(-5, &a);
  s21_from_int_to_decimal(0, &b);
  ck_assert_int_eq(s21_mul(a, b, &res), 0);
  ck_assert_int_eq(s21_is_mantissa_zero(res), 1);
}
END_TEST

// банковское округление до нуля
START_TEST(test_mul_bank_round_to_zero) {
  s21_decimal a = {{1, 0, 0, 0}};
  SET_SCALE(a, 28);
  s21_decimal b = {{5, 0, 0, 0}};
  SET_SCALE(b, 1);
  s21_decimal res;
  ck_assert_int_eq(s21_mul(a, b, &res), 0);
  ck_assert_int_eq(s21_is_mantissa_zero(res), 1);
}
END_TEST

// 7 * 9
START_TEST(test_mul_simple_63) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(7, &a);
  s21_from_int_to_decimal(9, &b);
  ck_assert_int_eq(s21_mul(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, 63);
}
END_TEST

// 1000 * 1000
START_TEST(test_mul_thousands) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(1000, &a);
  s21_from_int_to_decimal(1000, &b);
  ck_assert_int_eq(s21_mul(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, 1000000);
}
END_TEST

// ноль с разными scale
START_TEST(test_mul_zero_scaled) {
  s21_decimal a = {{0, 0, 0, 0}};
  SET_SCALE(a, 5);
  s21_decimal b = {{42, 0, 0, 0}};
  SET_SCALE(b, 3);
  s21_decimal res;
  ck_assert_int_eq(s21_mul(a, b, &res), 0);
  ck_assert_int_eq(s21_is_mantissa_zero(res), 1);
}
END_TEST

// 100000 * 100000
START_TEST(test_mul_billions) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(100000, &a);
  s21_from_int_to_decimal(100000, &b);
  ck_assert_int_eq(s21_mul(a, b, &res), 0);
  ck_assert_uint_eq((unsigned int)res.bits[0], 0x540BE400);
  ck_assert_int_eq(res.bits[1], 2);
  ck_assert_int_eq(res.bits[2], 0);
}
END_TEST

#pragma endregion

#pragma region s21_div
// 10 / 2
START_TEST(test_div_simple_positive) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(10, &a);
  s21_from_int_to_decimal(2, &b);
  ck_assert_int_eq(s21_div(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, 5);
}
END_TEST

// 100 / 10
START_TEST(test_div_clean_int) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(100, &a);
  s21_from_int_to_decimal(10, &b);
  ck_assert_int_eq(s21_div(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, 10);
}
END_TEST

// 1 / 2
START_TEST(test_div_half) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(1, &a);
  s21_from_int_to_decimal(2, &b);
  ck_assert_int_eq(s21_div(a, b, &res), 0);
  float out;
  s21_from_decimal_to_float(res, &out);
  ck_assert_float_eq_tol(out, 0.5f, 1e-6f);
}
END_TEST

// 7 / 2
START_TEST(test_div_with_remainder) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(7, &a);
  s21_from_int_to_decimal(2, &b);
  ck_assert_int_eq(s21_div(a, b, &res), 0);
  float out;
  s21_from_decimal_to_float(res, &out);
  ck_assert_float_eq_tol(out, 3.5f, 1e-6f);
}
END_TEST

// 10 / 4
START_TEST(test_div_25) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(10, &a);
  s21_from_int_to_decimal(4, &b);
  ck_assert_int_eq(s21_div(a, b, &res), 0);
  float out;
  s21_from_decimal_to_float(res, &out);
  ck_assert_float_eq_tol(out, 2.5f, 1e-6f);
}
END_TEST

// -10 / 4
START_TEST(test_div_neg_pos) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(-10, &a);
  s21_from_int_to_decimal(4, &b);
  ck_assert_int_eq(s21_div(a, b, &res), 0);
  ck_assert_int_eq(GET_SIGN(res), 1);
  float out;
  s21_from_decimal_to_float(res, &out);
  ck_assert_float_eq_tol(out, -2.5f, 1e-6f);
}
END_TEST

// 10 / -4
START_TEST(test_div_pos_neg) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(10, &a);
  s21_from_int_to_decimal(-4, &b);
  ck_assert_int_eq(s21_div(a, b, &res), 0);
  ck_assert_int_eq(GET_SIGN(res), 1);
  float out;
  s21_from_decimal_to_float(res, &out);
  ck_assert_float_eq_tol(out, -2.5f, 1e-6f);
}
END_TEST

// -10 / -4
START_TEST(test_div_neg_neg) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(-10, &a);
  s21_from_int_to_decimal(-4, &b);
  ck_assert_int_eq(s21_div(a, b, &res), 0);
  ck_assert_int_eq(GET_SIGN(res), 0);
  float out;
  s21_from_decimal_to_float(res, &out);
  ck_assert_float_eq_tol(out, 2.5f, 1e-6f);
}
END_TEST

// деление на ноль
START_TEST(test_div_by_zero) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(42, &a);
  s21_from_int_to_decimal(0, &b);
  ck_assert_int_eq(s21_div(a, b, &res), 3);
}
END_TEST

// 0 / x
START_TEST(test_div_zero_by_x) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(0, &a);
  s21_from_int_to_decimal(7, &b);
  ck_assert_int_eq(s21_div(a, b, &res), 0);
  ck_assert_int_eq(s21_is_mantissa_zero(res), 1);
}
END_TEST

// 0 / 0, тоже код 3
START_TEST(test_div_zero_by_zero) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(0, &a);
  s21_from_int_to_decimal(0, &b);
  ck_assert_int_eq(s21_div(a, b, &res), 3);
}
END_TEST

// NULL результат
START_TEST(test_div_null_result) {
  s21_decimal a, b;
  s21_from_int_to_decimal(1, &a);
  s21_from_int_to_decimal(2, &b);
  ck_assert_int_eq(s21_div(a, b, NULL), 1);
}
END_TEST

// 1 / 3, 28 знаков
START_TEST(test_div_one_third) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(1, &a);
  s21_from_int_to_decimal(3, &b);
  ck_assert_int_eq(s21_div(a, b, &res), 0);
  ck_assert_int_eq(GET_SCALE(res), 28);
  float out;
  s21_from_decimal_to_float(res, &out);
  ck_assert_float_eq_tol(out, 0.3333333f, 1e-6f);
}
END_TEST

// 2 / 3, последний разряд должен округлиться до 7
START_TEST(test_div_two_thirds) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(2, &a);
  s21_from_int_to_decimal(3, &b);
  ck_assert_int_eq(s21_div(a, b, &res), 0);
  ck_assert_int_eq(GET_SCALE(res), 28);
  float out;
  s21_from_decimal_to_float(res, &out);
  ck_assert_float_eq_tol(out, 0.6666667f, 1e-6f);
}
END_TEST

// 1 / 7
START_TEST(test_div_one_seventh) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(1, &a);
  s21_from_int_to_decimal(7, &b);
  ck_assert_int_eq(s21_div(a, b, &res), 0);
  ck_assert_int_eq(GET_SCALE(res), 28);
  float out;
  s21_from_decimal_to_float(res, &out);
  ck_assert_float_eq_tol(out, 0.1428571f, 1e-6f);
}
END_TEST

// 1.5 / 0.5
START_TEST(test_div_same_scale) {
  s21_decimal a = {{15, 0, 0, 0}};
  SET_SCALE(a, 1);
  s21_decimal b = {{5, 0, 0, 0}};
  SET_SCALE(b, 1);
  s21_decimal res;
  ck_assert_int_eq(s21_div(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, 3);
}
END_TEST

// 100 / 0.04
START_TEST(test_div_negative_scale_diff) {
  s21_decimal a;
  s21_from_int_to_decimal(100, &a);
  s21_decimal b = {{4, 0, 0, 0}};
  SET_SCALE(b, 2);
  s21_decimal res;
  ck_assert_int_eq(s21_div(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, 2500);
}
END_TEST

// 0.1 / 100
START_TEST(test_div_positive_scale_diff) {
  s21_decimal a = {{1, 0, 0, 0}};
  SET_SCALE(a, 1);
  s21_decimal b;
  s21_from_int_to_decimal(100, &b);
  s21_decimal res;
  ck_assert_int_eq(s21_div(a, b, &res), 0);
  float out;
  s21_from_decimal_to_float(res, &out);
  ck_assert_float_eq_tol(out, 0.001f, 1e-7f);
}
END_TEST

// max / 1
START_TEST(test_div_max_by_one) {
  s21_decimal a = {{(int)0xFFFFFFFF, (int)0xFFFFFFFF, (int)0xFFFFFFFF, 0}};
  s21_decimal b;
  s21_from_int_to_decimal(1, &b);
  s21_decimal res;
  ck_assert_int_eq(s21_div(a, b, &res), 0);
  ck_assert_uint_eq((unsigned int)res.bits[0], 0xFFFFFFFF);
  ck_assert_uint_eq((unsigned int)res.bits[1], 0xFFFFFFFF);
  ck_assert_uint_eq((unsigned int)res.bits[2], 0xFFFFFFFF);
  ck_assert_int_eq(GET_SCALE(res), 0);
}
END_TEST

// max / 0.1, переполнение
START_TEST(test_div_overflow_positive) {
  s21_decimal a = {{(int)0xFFFFFFFF, (int)0xFFFFFFFF, (int)0xFFFFFFFF, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  SET_SCALE(b, 1);
  s21_decimal res;
  ck_assert_int_eq(s21_div(a, b, &res), 1);
}
END_TEST

// -max / 0.1, переполнение
START_TEST(test_div_overflow_negative) {
  s21_decimal a = {{(int)0xFFFFFFFF, (int)0xFFFFFFFF, (int)0xFFFFFFFF, 0}};
  SET_SIGN(a, 1);
  s21_decimal b = {{1, 0, 0, 0}};
  SET_SCALE(b, 1);
  s21_decimal res;
  ck_assert_int_eq(s21_div(a, b, &res), 2);
}
END_TEST

// max / 3, без остатка
START_TEST(test_div_max_by_three) {
  s21_decimal a = {{(int)0xFFFFFFFF, (int)0xFFFFFFFF, (int)0xFFFFFFFF, 0}};
  s21_decimal b;
  s21_from_int_to_decimal(3, &b);
  s21_decimal res;
  ck_assert_int_eq(s21_div(a, b, &res), 0);
  ck_assert_uint_eq((unsigned int)res.bits[0], 0x55555555);
  ck_assert_uint_eq((unsigned int)res.bits[1], 0x55555555);
  ck_assert_uint_eq((unsigned int)res.bits[2], 0x55555555);
}
END_TEST

// 1 / max, ничтожно мало
START_TEST(test_div_one_by_max) {
  s21_decimal a;
  s21_from_int_to_decimal(1, &a);
  s21_decimal b = {{(int)0xFFFFFFFF, (int)0xFFFFFFFF, (int)0xFFFFFFFF, 0}};
  s21_decimal res;
  ck_assert_int_eq(s21_div(a, b, &res), 0);
  ck_assert_int_eq(s21_is_mantissa_zero(res), 1);
}
END_TEST

// 1 / 0.00001
START_TEST(test_div_by_small_scaled) {
  s21_decimal a;
  s21_from_int_to_decimal(1, &a);
  s21_decimal b = {{1, 0, 0, 0}};
  SET_SCALE(b, 5);
  s21_decimal res;
  ck_assert_int_eq(s21_div(a, b, &res), 0);
  int out;
  s21_from_decimal_to_int(res, &out);
  ck_assert_int_eq(out, 100000);
}
END_TEST

// 5 / 2
START_TEST(test_div_exact_25) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(5, &a);
  s21_from_int_to_decimal(2, &b);
  ck_assert_int_eq(s21_div(a, b, &res), 0);
  float out;
  s21_from_decimal_to_float(res, &out);
  ck_assert_float_eq_tol(out, 2.5f, 1e-6f);
}
END_TEST

// 1 / 8
START_TEST(test_div_exact_eighth) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(1, &a);
  s21_from_int_to_decimal(8, &b);
  ck_assert_int_eq(s21_div(a, b, &res), 0);
  float out;
  s21_from_decimal_to_float(res, &out);
  ck_assert_float_eq_tol(out, 0.125f, 1e-6f);
}
END_TEST

// (1/3) * 3, должно округлиться до 1.0
START_TEST(test_div_then_mul) {
  s21_decimal a, b, third, res;
  s21_from_int_to_decimal(1, &a);
  s21_from_int_to_decimal(3, &b);
  ck_assert_int_eq(s21_div(a, b, &third), 0);
  s21_from_int_to_decimal(3, &b);
  ck_assert_int_eq(s21_mul(third, b, &res), 0);
  float out;
  s21_from_decimal_to_float(res, &out);
  ck_assert_float_eq_tol(out, 1.0f, 1e-6f);
}
END_TEST
#pragma endregion

#pragma region s21_comparison

START_TEST(test_is_equal) {
  s21_decimal a = {{123, 0, 0, 0}};
  s21_decimal b = {{123, 0, 0, 0}};
  ck_assert_int_eq(s21_is_equal(a, b), 1);
  SET_SIGN(b, 1);
  ck_assert_int_eq(s21_is_equal(a, b), 0);
}
END_TEST

START_TEST(test_is_not_equal) {
  s21_decimal a = {{123, 0, 0, 0}};
  s21_decimal b = {{124, 0, 0, 0}};
  ck_assert_int_eq(s21_is_not_equal(a, b), 1);
}
END_TEST

START_TEST(test_is_less) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{20, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less(a, b), 1);
  SET_SIGN(a, 1);  // -10 < 20
  ck_assert_int_eq(s21_is_less(a, b), 1);
  SET_SIGN(b, 1);  // -10 < -20 (false)
  ck_assert_int_eq(s21_is_less(a, b), 0);
}
END_TEST

START_TEST(test_is_less_or_equal) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_is_greater) {
  s21_decimal a = {{100, 0, 0, 0}};
  s21_decimal b = {{50, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater(a, b), 1);
}
END_TEST

START_TEST(test_is_greater_or_equal) {
  s21_decimal a = {{100, 0, 0, 0}};
  s21_decimal b = {{100, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_compare_with_scales) {
  s21_decimal a = {{15, 0, 0, 0}};  // 1.5
  SET_SCALE(a, 1);
  s21_decimal b = {{150, 0, 0, 0}};  // 1.50
  SET_SCALE(b, 2);
  ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

#pragma endregion

#pragma region s21_from_int_to_decimal
// проверки идут через обратное преобразование
START_TEST(test_int_to_dec_zero) {
  s21_decimal d;
  ck_assert_int_eq(s21_from_int_to_decimal(0, &d), 0);
  ck_assert_int_eq(d.bits[0], 0);
  ck_assert_int_eq(GET_SCALE(d), 0);
  ck_assert_int_eq(GET_SIGN(d), 0);
}
END_TEST

START_TEST(test_int_to_dec_positive) {
  s21_decimal d;
  ck_assert_int_eq(s21_from_int_to_decimal(42, &d), 0);
  ck_assert_int_eq(d.bits[0], 42);
  ck_assert_int_eq(GET_SIGN(d), 0);
}
END_TEST

START_TEST(test_int_to_dec_negative) {
  s21_decimal d;
  ck_assert_int_eq(s21_from_int_to_decimal(-42, &d), 0);
  ck_assert_int_eq(d.bits[0], 42);
  ck_assert_int_eq(GET_SIGN(d), 1);
}
END_TEST

START_TEST(test_int_to_dec_int_max) {
  s21_decimal d;
  ck_assert_int_eq(s21_from_int_to_decimal(INT_MAX, &d), 0);
  ck_assert_int_eq(d.bits[0], INT_MAX);
  ck_assert_int_eq(GET_SIGN(d), 0);
}
END_TEST

START_TEST(test_int_to_dec_int_min) {
  s21_decimal d;
  ck_assert_int_eq(s21_from_int_to_decimal(INT_MIN, &d), 0);
  ck_assert_uint_eq((unsigned int)d.bits[0], 2147483648U);
  ck_assert_int_eq(GET_SIGN(d), 1);
}
END_TEST

START_TEST(test_int_to_dec_null) {
  ck_assert_int_eq(s21_from_int_to_decimal(1, NULL), 1);
}
END_TEST
#pragma endregion

#pragma region s21_from_float_to_decimal
START_TEST(test_float_to_dec_zero) {
  s21_decimal d;
  ck_assert_int_eq(s21_from_float_to_decimal(0.0f, &d), 0);
  ck_assert_int_eq(s21_is_mantissa_zero(d), 1);
}
END_TEST

START_TEST(test_float_to_dec_positive) {
  s21_decimal d;
  float f = 1.5f;
  ck_assert_int_eq(s21_from_float_to_decimal(f, &d), 0);
  ck_assert_int_eq(GET_SIGN(d), 0);
  // конверт обратно и сравниваем
  float out;
  s21_from_decimal_to_float(d, &out);
  ck_assert_float_eq_tol(out, 1.5f, 1e-6f);
}
END_TEST

START_TEST(test_float_to_dec_negative) {
  s21_decimal d;
  ck_assert_int_eq(s21_from_float_to_decimal(-3.14f, &d), 0);
  ck_assert_int_eq(GET_SIGN(d), 1);
  float out;
  s21_from_decimal_to_float(d, &out);
  ck_assert_float_eq_tol(out, -3.14f, 1e-5f);
}
END_TEST

START_TEST(test_float_to_dec_nan) {
  s21_decimal d;
  ck_assert_int_eq(s21_from_float_to_decimal(0.0f / 0.0f, &d), 1);
}
END_TEST

START_TEST(test_float_to_dec_inf) {
  s21_decimal d;
  ck_assert_int_eq(s21_from_float_to_decimal(1.0f / 0.0f, &d), 1);
}
END_TEST

START_TEST(test_float_to_dec_too_large) {
  s21_decimal d;
  ck_assert_int_eq(s21_from_float_to_decimal(8e+28f, &d), 1);
}
END_TEST

START_TEST(test_float_to_dec_too_small) {
  s21_decimal d;
  ck_assert_int_eq(s21_from_float_to_decimal(1e-29f, &d), 1);
}
END_TEST

START_TEST(test_float_to_dec_null) {
  ck_assert_int_eq(s21_from_float_to_decimal(1.0f, NULL), 1);
}
END_TEST
// округления
START_TEST(test_float_to_dec_boundary_1e_28_ok) {
  s21_decimal d = {{-1, -1, -1, -1}};
  ck_assert_int_eq(s21_from_float_to_decimal(1e-28f, &d), 0);
  ck_assert_int_eq(d.bits[0] == 0 && d.bits[1] == 0 && d.bits[2] == 0, 0);
}
END_TEST

START_TEST(test_float_to_dec_rounding_bank_even) {
  s21_decimal d = {{0}};
  ck_assert_int_eq(s21_from_float_to_decimal(1.2345655f, &d), 0);

  float out = 0.0f;
  ck_assert_int_eq(s21_from_decimal_to_float(d, &out), 0);
  ck_assert_float_eq_tol(out, 1.234566f, 1e-6f);
}
END_TEST

START_TEST(test_float_to_dec_error_zero_result) {
  s21_decimal d = {{123, 456, 789, 999}};
  ck_assert_int_eq(s21_from_float_to_decimal(1e-29f, &d), 1);
  ck_assert_int_eq(d.bits[0], 0);
  ck_assert_int_eq(d.bits[1], 0);
  ck_assert_int_eq(d.bits[2], 0);
  ck_assert_int_eq(d.bits[3], 0);
}
END_TEST
#pragma endregion

#pragma region s21_from_decimal_to_int
START_TEST(test_dec_to_int_simple) {
  s21_decimal d = {{99, 0, 0, 0}};
  int out;
  ck_assert_int_eq(s21_from_decimal_to_int(d, &out), 0);
  ck_assert_int_eq(out, 99);
}
END_TEST

START_TEST(test_dec_to_int_negative) {
  s21_decimal d = {{7, 0, 0, 0}};
  SET_SIGN(d, 1);
  int out;
  ck_assert_int_eq(s21_from_decimal_to_int(d, &out), 0);
  ck_assert_int_eq(out, -7);
}
END_TEST

START_TEST(test_dec_to_int_truncates) {
  // 3.7 - 3
  s21_decimal d = {{37, 0, 0, 0}};
  SET_SCALE(d, 1);
  int out;
  ck_assert_int_eq(s21_from_decimal_to_int(d, &out), 0);
  ck_assert_int_eq(out, 3);
}
END_TEST

START_TEST(test_dec_to_int_overflow) {
  // bits[1] занят -> не влезает в int
  s21_decimal d = {{0, 1, 0, 0}};
  int out;
  ck_assert_int_eq(s21_from_decimal_to_int(d, &out), 1);
}
END_TEST

START_TEST(test_dec_to_int_null) {
  s21_decimal d = {{1, 0, 0, 0}};
  ck_assert_int_eq(s21_from_decimal_to_int(d, NULL), 1);
}
END_TEST

START_TEST(test_dec_to_int_zero) {
  s21_decimal d = {{0, 0, 0, 0}};
  int out;
  ck_assert_int_eq(s21_from_decimal_to_int(d, &out), 0);
  ck_assert_int_eq(out, 0);
}
END_TEST

START_TEST(test_dec_to_int_negative_fraction) {
  s21_decimal d = {{37, 0, 0, 0}};
  SET_SCALE(d, 1);
  SET_SIGN(d, 1);

  int out = 123;
  ck_assert_int_eq(s21_from_decimal_to_int(d, &out), 0);
  ck_assert_int_eq(out, -3);
}
END_TEST

START_TEST(test_dec_to_int_overflow_high_bits) {
  s21_decimal d = {{0, 0, 1, 0}};
  int out = 0;
  ck_assert_int_eq(s21_from_decimal_to_int(d, &out), 1);
}
END_TEST
#pragma endregion

#pragma region s21_from_decimal_to_float
START_TEST(test_dec_to_float_simple) {
  s21_decimal d = {{100, 0, 0, 0}};
  SET_SCALE(d, 2);  // = 1.00
  float out;
  ck_assert_int_eq(s21_from_decimal_to_float(d, &out), 0);
  ck_assert_float_eq_tol(out, 1.0f, 1e-6f);
}
END_TEST

START_TEST(test_dec_to_float_negative) {
  s21_decimal d = {{500, 0, 0, 0}};
  SET_SCALE(d, 2);
  SET_SIGN(d, 1);  // = -5.00
  float out;
  ck_assert_int_eq(s21_from_decimal_to_float(d, &out), 0);
  ck_assert_float_eq_tol(out, -5.0f, 1e-6f);
}
END_TEST

START_TEST(test_dec_to_float_zero) {
  s21_decimal d = {{0, 0, 0, 0}};
  float out;
  ck_assert_int_eq(s21_from_decimal_to_float(d, &out), 0);
  ck_assert_float_eq_tol(out, 0.0f, 1e-9f);
}
END_TEST

START_TEST(test_dec_to_float_null) {
  s21_decimal d = {{1, 0, 0, 0}};
  ck_assert_int_eq(s21_from_decimal_to_float(d, NULL), 1);
}
END_TEST
START_TEST(test_dec_to_float_fraction) {
  s21_decimal d = {{1234567, 0, 0, 0}};
  SET_SCALE(d, 6);

  float out = 0.0f;
  ck_assert_int_eq(s21_from_decimal_to_float(d, &out), 0);
  ck_assert_float_eq_tol(out, 1.234567f, 1e-6f);
}
END_TEST
#pragma endregion

#pragma region s21_floor

START_TEST(test_floor_positive_int) {
  s21_decimal a, r;
  s21_from_int_to_decimal(5, &a);
  ck_assert_int_eq(s21_floor(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 5);
}
END_TEST

START_TEST(test_floor_positive_fraction) {
  s21_decimal a = {{27, 0, 0, 0}};
  SET_SCALE(a, 1);
  s21_decimal r;
  ck_assert_int_eq(s21_floor(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 2);
}
END_TEST

START_TEST(test_floor_negative_exact) {
  s21_decimal a = {{30, 0, 0, 0}};
  SET_SCALE(a, 1);
  SET_SIGN(a, 1);
  s21_decimal r;
  ck_assert_int_eq(s21_floor(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, -3);
}
END_TEST

START_TEST(test_floor_negative_fraction) {
  s21_decimal a = {{27, 0, 0, 0}};
  SET_SCALE(a, 1);
  SET_SIGN(a, 1);
  s21_decimal r;
  ck_assert_int_eq(s21_floor(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, -3);
}
END_TEST

START_TEST(test_floor_negative_int) {
  s21_decimal a;
  s21_from_int_to_decimal(-7, &a);
  s21_decimal r;
  ck_assert_int_eq(s21_floor(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, -7);
}
END_TEST

START_TEST(test_floor_zero_scale_positive) {
  s21_decimal a;
  s21_from_int_to_decimal(42, &a);
  s21_decimal r;
  ck_assert_int_eq(s21_floor(a, &r), 0);
  ck_assert_int_eq(GET_SCALE(r), 0);
  ck_assert_int_eq(r.bits[0], 42);
}
END_TEST

START_TEST(test_floor_zero) {
  s21_decimal a = {{0, 0, 0, 0}};
  SET_SCALE(a, 3);
  s21_decimal r;
  ck_assert_int_eq(s21_floor(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 0);
}
END_TEST

START_TEST(test_floor_null_result) {
  s21_decimal a;
  s21_from_int_to_decimal(5, &a);
  ck_assert_int_eq(s21_floor(a, NULL), 1);
}
END_TEST

START_TEST(test_floor_large_positive) {
  s21_decimal a = {{9999, 0, 0, 0}};
  SET_SCALE(a, 1);
  s21_decimal r;
  ck_assert_int_eq(s21_floor(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 999);
}
END_TEST

START_TEST(test_floor_negative_small_fraction) {
  s21_decimal a = {{11, 0, 0, 0}};
  SET_SCALE(a, 1);
  SET_SIGN(a, 1);
  s21_decimal r;
  ck_assert_int_eq(s21_floor(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, -2);
}
END_TEST

START_TEST(test_floor_multi_scale) {
  s21_decimal a = {{123, 0, 0, 0}};
  SET_SCALE(a, 2);
  s21_decimal r;
  ck_assert_int_eq(s21_floor(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 1);
}
END_TEST

START_TEST(test_floor_multi_scale_negative) {
  s21_decimal a = {{123, 0, 0, 0}};
  SET_SCALE(a, 2);
  SET_SIGN(a, 1);
  s21_decimal r;
  ck_assert_int_eq(s21_floor(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, -2);
}
END_TEST

START_TEST(test_floor_zero_sign) {
  s21_decimal a = {{5, 0, 0, 0}};
  SET_SCALE(a, 1);
  SET_SIGN(a, 1);
  s21_decimal r;
  s21_floor(a, &r);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, -1);
}
END_TEST

#pragma endregion

#pragma region s21_round

START_TEST(test_round_half_to_even_down) {
  s21_decimal a = {{25, 0, 0, 0}};
  SET_SCALE(a, 1);
  s21_decimal r;
  ck_assert_int_eq(s21_round(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 2);
}
END_TEST

START_TEST(test_round_half_to_even_up) {
  s21_decimal a = {{35, 0, 0, 0}};
  SET_SCALE(a, 1);
  s21_decimal r;
  ck_assert_int_eq(s21_round(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 4);
}
END_TEST

START_TEST(test_round_down) {
  s21_decimal a = {{24, 0, 0, 0}};
  SET_SCALE(a, 1);
  s21_decimal r;
  ck_assert_int_eq(s21_round(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 2);
}
END_TEST

START_TEST(test_round_up) {
  s21_decimal a = {{26, 0, 0, 0}};
  SET_SCALE(a, 1);
  s21_decimal r;
  ck_assert_int_eq(s21_round(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 3);
}
END_TEST

START_TEST(test_round_negative_half) {
  s21_decimal a = {{25, 0, 0, 0}};
  SET_SCALE(a, 1);
  SET_SIGN(a, 1);
  s21_decimal r;
  ck_assert_int_eq(s21_round(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, -2);
}
END_TEST

START_TEST(test_round_negative_up) {
  s21_decimal a = {{26, 0, 0, 0}};
  SET_SCALE(a, 1);
  SET_SIGN(a, 1);
  s21_decimal r;
  ck_assert_int_eq(s21_round(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, -3);
}
END_TEST

START_TEST(test_round_integer_unchanged) {
  s21_decimal a;
  s21_from_int_to_decimal(7, &a);
  s21_decimal r;
  ck_assert_int_eq(s21_round(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 7);
}
END_TEST

START_TEST(test_round_null_result) {
  s21_decimal a;
  s21_from_int_to_decimal(5, &a);
  ck_assert_int_eq(s21_round(a, NULL), 1);
}
END_TEST

START_TEST(test_round_multi_scale) {
  s21_decimal a = {{123, 0, 0, 0}};
  SET_SCALE(a, 2);
  s21_decimal r;
  ck_assert_int_eq(s21_round(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 1);
}
END_TEST

START_TEST(test_round_multi_scale_half) {
  s21_decimal a = {{125, 0, 0, 0}};
  SET_SCALE(a, 2);
  s21_decimal r;
  ck_assert_int_eq(s21_round(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 1);
}
END_TEST

START_TEST(test_round_zero) {
  s21_decimal a = {{0, 0, 0, 0}};
  SET_SCALE(a, 5);
  s21_decimal r;
  ck_assert_int_eq(s21_round(a, &r), 0);
  ck_assert_int_eq(r.bits[0], 0);
  ck_assert_int_eq(GET_SIGN(r), 0);
}
END_TEST

START_TEST(test_round_half_zero) {
  s21_decimal a = {{5, 0, 0, 0}};
  SET_SCALE(a, 1);
  s21_decimal r;
  ck_assert_int_eq(s21_round(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 0);
}
END_TEST

START_TEST(test_round_negative_int) {
  s21_decimal a;
  s21_from_int_to_decimal(-9, &a);
  s21_decimal r;
  ck_assert_int_eq(s21_round(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, -9);
}
END_TEST

START_TEST(test_round_149_is_1) {
  s21_decimal a = {{149, 0, 0, 0}};
  SET_SCALE(a, 2);
  s21_decimal r;
  ck_assert_int_eq(s21_round(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 1);
}
END_TEST

START_TEST(test_round_151_is_2) {
  s21_decimal a = {{151, 0, 0, 0}};
  SET_SCALE(a, 2);
  s21_decimal r;
  ck_assert_int_eq(s21_round(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 2);
}
END_TEST

START_TEST(test_round_tail_nonzero) {
  s21_decimal a = {{15001, 0, 0, 0}};
  SET_SCALE(a, 4);
  s21_decimal r;
  ck_assert_int_eq(s21_round(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 2);
}
END_TEST

START_TEST(test_round_2500_is_2) {
  s21_decimal a = {{2500, 0, 0, 0}};
  SET_SCALE(a, 3);
  s21_decimal r;
  ck_assert_int_eq(s21_round(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 2);
}
END_TEST

START_TEST(test_round_1500_is_2) {
  s21_decimal a = {{1500, 0, 0, 0}};
  SET_SCALE(a, 3);
  s21_decimal r;
  ck_assert_int_eq(s21_round(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 2);
}
END_TEST

#pragma endregion

#pragma region s21_truncate

START_TEST(test_truncate_positive) {
  s21_decimal a = {{123, 0, 0, 0}};
  SET_SCALE(a, 1);
  s21_decimal r;
  ck_assert_int_eq(s21_truncate(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 12);
}
END_TEST

START_TEST(test_truncate_negative) {
  s21_decimal a = {{123, 0, 0, 0}};
  SET_SCALE(a, 1);
  SET_SIGN(a, 1);
  s21_decimal r;
  ck_assert_int_eq(s21_truncate(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, -12);
}
END_TEST

START_TEST(test_truncate_no_scale) {
  s21_decimal a;
  s21_from_int_to_decimal(42, &a);
  s21_decimal r;
  ck_assert_int_eq(s21_truncate(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 42);
}
END_TEST

START_TEST(test_truncate_scale_zero) {
  s21_decimal a = {{9999, 0, 0, 0}};
  SET_SCALE(a, 2);
  s21_decimal r;
  s21_truncate(a, &r);
  ck_assert_int_eq(GET_SCALE(r), 0);
}
END_TEST

START_TEST(test_truncate_two_decimals) {
  s21_decimal a = {{9999, 0, 0, 0}};
  SET_SCALE(a, 2);
  s21_decimal r;
  ck_assert_int_eq(s21_truncate(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 99);
}
END_TEST

START_TEST(test_truncate_null_result) {
  s21_decimal a;
  s21_from_int_to_decimal(5, &a);
  ck_assert_int_eq(s21_truncate(a, NULL), 1);
}
END_TEST

START_TEST(test_truncate_zero_fractional) {
  s21_decimal a = {{0, 0, 0, 0}};
  SET_SCALE(a, 5);
  s21_decimal r;
  ck_assert_int_eq(s21_truncate(a, &r), 0);
  ck_assert_int_eq(r.bits[0], 0);
  ck_assert_int_eq(GET_SCALE(r), 0);
}
END_TEST

START_TEST(test_truncate_no_rounding) {
  s21_decimal a = {{29, 0, 0, 0}};
  SET_SCALE(a, 1);
  s21_decimal r;
  ck_assert_int_eq(s21_truncate(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 2);
}
END_TEST

START_TEST(test_truncate_sign_preserved) {
  s21_decimal a = {{55, 0, 0, 0}};
  SET_SCALE(a, 1);
  SET_SIGN(a, 1);
  s21_decimal r;
  s21_truncate(a, &r);
  ck_assert_int_eq(GET_SIGN(r), 1);
  ck_assert_int_eq(r.bits[0], 5);
}
END_TEST

START_TEST(test_truncate_negative_less_than_one) {
  s21_decimal a = {{9, 0, 0, 0}};
  SET_SCALE(a, 1);
  SET_SIGN(a, 1);
  s21_decimal r;
  s21_truncate(a, &r);
  ck_assert_int_eq(r.bits[0], 0);
}
END_TEST

START_TEST(test_truncate_large_scale) {
  s21_decimal a = {{123456789, 0, 0, 0}};
  SET_SCALE(a, 5);
  s21_decimal r;
  ck_assert_int_eq(s21_truncate(a, &r), 0);
  int out;
  s21_from_decimal_to_int(r, &out);
  ck_assert_int_eq(out, 1234);
}
END_TEST

#pragma endregion

#pragma region s21_negate

START_TEST(test_negate_positive) {
  s21_decimal a;
  s21_from_int_to_decimal(5, &a);
  s21_decimal r;
  ck_assert_int_eq(s21_negate(a, &r), 0);
  ck_assert_int_eq(GET_SIGN(r), 1);
  ck_assert_int_eq(r.bits[0], 5);
}
END_TEST

START_TEST(test_negate_negative) {
  s21_decimal a;
  s21_from_int_to_decimal(-5, &a);
  s21_decimal r;
  ck_assert_int_eq(s21_negate(a, &r), 0);
  ck_assert_int_eq(GET_SIGN(r), 0);
  ck_assert_int_eq(r.bits[0], 5);
}
END_TEST

START_TEST(test_negate_mantissa_unchanged) {
  s21_decimal a = {{123, 456, 789, 0}};
  s21_decimal r;
  s21_negate(a, &r);
  ck_assert_int_eq(r.bits[0], 123);
  ck_assert_int_eq(r.bits[1], 456);
  ck_assert_int_eq(r.bits[2], 789);
}
END_TEST

START_TEST(test_negate_scale_preserved) {
  s21_decimal a = {{100, 0, 0, 0}};
  SET_SCALE(a, 3);
  s21_decimal r;
  s21_negate(a, &r);
  ck_assert_int_eq(GET_SCALE(r), 3);
}
END_TEST

START_TEST(test_negate_double) {
  s21_decimal a;
  s21_from_int_to_decimal(42, &a);
  s21_decimal r1, r2;
  s21_negate(a, &r1);
  s21_negate(r1, &r2);
  ck_assert_int_eq(GET_SIGN(r2), GET_SIGN(a));
  ck_assert_int_eq(r2.bits[0], a.bits[0]);
}
END_TEST

START_TEST(test_negate_null_result) {
  s21_decimal a;
  s21_from_int_to_decimal(5, &a);
  ck_assert_int_eq(s21_negate(a, NULL), 1);
}
END_TEST

START_TEST(test_negate_zero) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal r;
  ck_assert_int_eq(s21_negate(a, &r), 0);
  ck_assert_int_eq(GET_SIGN(r), 1);
}
END_TEST

START_TEST(test_negate_int_max) {
  s21_decimal a;
  s21_from_int_to_decimal(2147483647, &a);
  s21_decimal r;
  ck_assert_int_eq(s21_negate(a, &r), 0);
  ck_assert_int_eq(GET_SIGN(r), 1);
  ck_assert_int_eq(r.bits[0], 2147483647);
}
END_TEST

START_TEST(test_negate_negative_scaled) {
  s21_decimal a = {{255, 0, 0, 0}};
  SET_SCALE(a, 2);
  SET_SIGN(a, 1);
  s21_decimal r;
  s21_negate(a, &r);
  ck_assert_int_eq(GET_SIGN(r), 0);
  ck_assert_int_eq(GET_SCALE(r), 2);
  ck_assert_int_eq(r.bits[0], 255);
}
END_TEST

#pragma endregion

#pragma region s21_mantissa_helpers
// тесты утилит мантиссы

START_TEST(test_is_zero_all_zero) {
  s21_decimal d = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_mantissa_zero(d), 1);
}
END_TEST

START_TEST(test_is_zero_bits0_not_zero) {
  s21_decimal d = {{5, 0, 0, 0}};
  ck_assert_int_eq(s21_is_mantissa_zero(d), 0);
}
END_TEST

START_TEST(test_is_zero_bits2_not_zero) {
  s21_decimal d = {{0, 0, 100, 0}};
  ck_assert_int_eq(s21_is_mantissa_zero(d), 0);
}
END_TEST

START_TEST(test_cmp_equal) {
  s21_decimal a = {{100, 200, 300, 0}};
  s21_decimal b = {{100, 200, 300, 0}};
  ck_assert_int_eq(s21_cmp_mantissa(a, b), 0);
}
END_TEST

START_TEST(test_cmp_a_greater_by_high) {
  s21_decimal a = {{0, 0, 5, 0}};
  s21_decimal b = {{999, 999, 3, 0}};
  ck_assert_int_eq(s21_cmp_mantissa(a, b), 1);
}
END_TEST

START_TEST(test_cmp_a_less_by_low) {
  s21_decimal a = {{5, 10, 20, 0}};
  s21_decimal b = {{7, 10, 20, 0}};
  ck_assert_int_eq(s21_cmp_mantissa(a, b), -1);
}
END_TEST

START_TEST(test_cmp_both_zero) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_cmp_mantissa(a, b), 0);
}
END_TEST

START_TEST(test_add_simple) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  int code = s21_add_mantissa(a, b, &res);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(res.bits[0], 8);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
}
END_TEST

START_TEST(test_add_with_carry) {
  // 0xFFFFFFFF + 1 должно дать перенос в bits[1]
  s21_decimal a = {{(int)0xFFFFFFFF, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  int code = s21_add_mantissa(a, b, &res);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 1);
  ck_assert_int_eq(res.bits[2], 0);
}
END_TEST

START_TEST(test_add_overflow) {
  // максимальная мантисса + 1 → переполнение
  s21_decimal a = {{(int)0xFFFFFFFF, (int)0xFFFFFFFF, (int)0xFFFFFFFF, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  int code = s21_add_mantissa(a, b, &res);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_sub_simple) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  int code = s21_sub_mantissa(a, b, &res);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(res.bits[0], 7);
}
END_TEST

START_TEST(test_sub_with_borrow) {
  // [0, 1, 0] - [1, 0, 0] = [0xFFFFFFFF, 0, 0]
  s21_decimal a = {{0, 1, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  int code = s21_sub_mantissa(a, b, &res);
  ck_assert_int_eq(code, 0);
  ck_assert_uint_eq((unsigned int)res.bits[0], 0xFFFFFFFF);
  ck_assert_int_eq(res.bits[1], 0);
}
END_TEST

START_TEST(test_sub_a_less_than_b) {
  // a < b → должна вернуть 1
  s21_decimal a = {{3, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  int code = s21_sub_mantissa(a, b, &res);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_mul10_simple) {
  s21_decimal d = {{5, 0, 0, 0}};
  int code = s21_mul_mantissa_by_10(&d);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(d.bits[0], 50);
}
END_TEST

START_TEST(test_mul10_with_carry) {
  // 0xFFFFFFFF * 10 = 0x9FFFFFFF6 → bits[0]=0xFFFFFFF6, bits[1]=9
  s21_decimal d = {{(int)0xFFFFFFFF, 0, 0, 0}};
  int code = s21_mul_mantissa_by_10(&d);
  ck_assert_int_eq(code, 0);
  ck_assert_uint_eq((unsigned int)d.bits[0], 0xFFFFFFF6);
  ck_assert_int_eq(d.bits[1], 9);
}
END_TEST

START_TEST(test_mul10_overflow) {
  // максимум * 10 → переполнение
  s21_decimal d = {{(int)0xFFFFFFFF, (int)0xFFFFFFFF, (int)0xFFFFFFFF, 0}};
  int code = s21_mul_mantissa_by_10(&d);
  ck_assert_int_eq(code, 1);
}
END_TEST

#pragma endregion

#pragma region suite
Suite* s21_decimal_suite(void) {
  Suite* suite = suite_create("s21_decimal");

  // тестовые кейсы для каждой функции

  TCase* tc_s21_add = tcase_create("s21_add");
  TCase* tc_s21_sub = tcase_create("s21_sub");
  TCase* tc_s21_mul = tcase_create("s21_mul");
  TCase* tc_s21_div = tcase_create("s21_div");
  TCase* tc_s21_is_less = tcase_create("s21_is_less");
  TCase* tc_s21_is_less_or_equal = tcase_create("s21_is_less_or_equal");
  TCase* tc_s21_is_greater = tcase_create("s21_is_greater");
  TCase* tc_s21_is_greater_or_equal = tcase_create("s21_is_greater_or_equal");
  TCase* tc_s21_is_equal = tcase_create("s21_is_equal");
  TCase* tc_s21_is_not_equal = tcase_create("s21_is_not_equal");
  TCase* tc_s21_comparison = tcase_create("s21_comparison");
  TCase* tc_s21_from_int_to_decimal = tcase_create("s21_from_int_to_decimal");
  TCase* tc_s21_from_float_to_decimal =
      tcase_create("s21_from_float_to_decimal");
  TCase* tc_s21_from_decimal_to_int = tcase_create("s21_from_decimal_to_int");
  TCase* tc_s21_from_decimal_to_float =
      tcase_create("s21_from_decimal_to_float");
  TCase* tc_s21_floor = tcase_create("s21_floor");
  TCase* tc_s21_round = tcase_create("s21_round");
  TCase* tc_s21_truncate = tcase_create("s21_truncate");
  TCase* tc_s21_negate = tcase_create("s21_negate");
  TCase* tc_mantissa = tcase_create("mantissa_helpers");

  tcase_add_test(tc_s21_from_int_to_decimal, test_int_to_dec_zero);
  tcase_add_test(tc_s21_from_int_to_decimal, test_int_to_dec_positive);
  tcase_add_test(tc_s21_from_int_to_decimal, test_int_to_dec_negative);
  tcase_add_test(tc_s21_from_int_to_decimal, test_int_to_dec_int_max);
  tcase_add_test(tc_s21_from_int_to_decimal, test_int_to_dec_int_min);
  tcase_add_test(tc_s21_from_int_to_decimal, test_int_to_dec_null);

  tcase_add_test(tc_s21_from_float_to_decimal, test_float_to_dec_zero);
  tcase_add_test(tc_s21_from_float_to_decimal, test_float_to_dec_positive);
  tcase_add_test(tc_s21_from_float_to_decimal, test_float_to_dec_negative);
  tcase_add_test(tc_s21_from_float_to_decimal, test_float_to_dec_nan);
  tcase_add_test(tc_s21_from_float_to_decimal, test_float_to_dec_inf);
  tcase_add_test(tc_s21_from_float_to_decimal, test_float_to_dec_too_large);
  tcase_add_test(tc_s21_from_float_to_decimal, test_float_to_dec_too_small);
  tcase_add_test(tc_s21_from_float_to_decimal, test_float_to_dec_null);
  tcase_add_test(tc_s21_from_float_to_decimal,
                 test_float_to_dec_boundary_1e_28_ok);
  tcase_add_test(tc_s21_from_float_to_decimal,
                 test_float_to_dec_rounding_bank_even);
  tcase_add_test(tc_s21_from_float_to_decimal,
                 test_float_to_dec_error_zero_result);

  tcase_add_test(tc_s21_from_decimal_to_int, test_dec_to_int_simple);
  tcase_add_test(tc_s21_from_decimal_to_int, test_dec_to_int_negative);
  tcase_add_test(tc_s21_from_decimal_to_int, test_dec_to_int_truncates);
  tcase_add_test(tc_s21_from_decimal_to_int, test_dec_to_int_overflow);
  tcase_add_test(tc_s21_from_decimal_to_int, test_dec_to_int_null);
  tcase_add_test(tc_s21_from_decimal_to_int, test_dec_to_int_zero);
  tcase_add_test(tc_s21_from_decimal_to_int, test_dec_to_int_negative_fraction);
  tcase_add_test(tc_s21_from_decimal_to_int,
                 test_dec_to_int_overflow_high_bits);

  tcase_add_test(tc_s21_from_decimal_to_float, test_dec_to_float_simple);
  tcase_add_test(tc_s21_from_decimal_to_float, test_dec_to_float_negative);
  tcase_add_test(tc_s21_from_decimal_to_float, test_dec_to_float_zero);
  tcase_add_test(tc_s21_from_decimal_to_float, test_dec_to_float_null);
  tcase_add_test(tc_s21_from_decimal_to_float, test_dec_to_float_fraction);

  // тесты утилит мантиссы
  tcase_add_test(tc_mantissa, test_is_zero_all_zero);
  tcase_add_test(tc_mantissa, test_is_zero_bits0_not_zero);
  tcase_add_test(tc_mantissa, test_is_zero_bits2_not_zero);
  // cmp
  tcase_add_test(tc_mantissa, test_cmp_equal);
  tcase_add_test(tc_mantissa, test_cmp_a_greater_by_high);
  tcase_add_test(tc_mantissa, test_cmp_a_less_by_low);
  tcase_add_test(tc_mantissa, test_cmp_both_zero);

  // add
  tcase_add_test(tc_mantissa, test_add_simple);
  tcase_add_test(tc_mantissa, test_add_with_carry);
  tcase_add_test(tc_mantissa, test_add_overflow);

  // sub
  tcase_add_test(tc_mantissa, test_sub_simple);
  tcase_add_test(tc_mantissa, test_sub_with_borrow);
  tcase_add_test(tc_mantissa, test_sub_a_less_than_b);

  // mul_by_10
  tcase_add_test(tc_mantissa, test_mul10_simple);
  tcase_add_test(tc_mantissa, test_mul10_with_carry);
  tcase_add_test(tc_mantissa, test_mul10_overflow);

  tcase_add_test(tc_s21_add, test_add_positive_positive);
  tcase_add_test(tc_s21_add, test_add_negative_negative);
  tcase_add_test(tc_s21_add, test_add_positive_negative);
  tcase_add_test(tc_s21_add, test_add_negative_positive);
  tcase_add_test(tc_s21_add, test_add_zero_result);
  tcase_add_test(tc_s21_add, test_add_with_zero);
  tcase_add_test(tc_s21_add, test_add_different_scale);
  tcase_add_test(tc_s21_add, test_add_null_result);

  tcase_add_test(tc_s21_sub, test_sub_positive_positive);
  tcase_add_test(tc_s21_sub, test_sub_negative_negative);
  tcase_add_test(tc_s21_sub, test_sub_positive_negative);
  tcase_add_test(tc_s21_sub, test_sub_negative_positive);
  tcase_add_test(tc_s21_sub, test_sub_zero);
  tcase_add_test(tc_s21_sub, test_sub_from_zero);
  tcase_add_test(tc_s21_sub, test_sub_different_scale);
  tcase_add_test(tc_s21_sub, test_sub_null_result);
  // mul
  tcase_add_test(tc_s21_mul, test_mul_simple_positive);
  tcase_add_test(tc_s21_mul, test_mul_negative_positive);
  tcase_add_test(tc_s21_mul, test_mul_positive_negative);
  tcase_add_test(tc_s21_mul, test_mul_negative_negative);
  tcase_add_test(tc_s21_mul, test_mul_zero_left);
  tcase_add_test(tc_s21_mul, test_mul_zero_right);
  tcase_add_test(tc_s21_mul, test_mul_zero_zero);
  tcase_add_test(tc_s21_mul, test_mul_by_one);
  tcase_add_test(tc_s21_mul, test_mul_scale_simple);
  tcase_add_test(tc_s21_mul, test_mul_scaled_by_int);
  tcase_add_test(tc_s21_mul, test_mul_scaled_negative);
  tcase_add_test(tc_s21_mul, test_mul_small_fractions);
  tcase_add_test(tc_s21_mul, test_mul_max_by_one);
  tcase_add_test(tc_s21_mul, test_mul_overflow);
  tcase_add_test(tc_s21_mul, test_mul_overflow_negative);
  tcase_add_test(tc_s21_mul, test_mul_carry_to_high);
  tcase_add_test(tc_s21_mul, test_mul_null_result);
  tcase_add_test(tc_s21_mul, test_mul_zero_sign_clean);
  tcase_add_test(tc_s21_mul, test_mul_bank_round_to_zero);
  tcase_add_test(tc_s21_mul, test_mul_simple_63);
  tcase_add_test(tc_s21_mul, test_mul_thousands);
  tcase_add_test(tc_s21_mul, test_mul_zero_scaled);
  tcase_add_test(tc_s21_mul, test_mul_billions);

  // div
  tcase_add_test(tc_s21_div, test_div_simple_positive);
  tcase_add_test(tc_s21_div, test_div_clean_int);
  tcase_add_test(tc_s21_div, test_div_half);
  tcase_add_test(tc_s21_div, test_div_with_remainder);
  tcase_add_test(tc_s21_div, test_div_25);
  tcase_add_test(tc_s21_div, test_div_neg_pos);
  tcase_add_test(tc_s21_div, test_div_pos_neg);
  tcase_add_test(tc_s21_div, test_div_neg_neg);
  tcase_add_test(tc_s21_div, test_div_by_zero);
  tcase_add_test(tc_s21_div, test_div_zero_by_x);
  tcase_add_test(tc_s21_div, test_div_zero_by_zero);
  tcase_add_test(tc_s21_div, test_div_null_result);
  tcase_add_test(tc_s21_div, test_div_one_third);
  tcase_add_test(tc_s21_div, test_div_two_thirds);
  tcase_add_test(tc_s21_div, test_div_one_seventh);
  tcase_add_test(tc_s21_div, test_div_same_scale);
  tcase_add_test(tc_s21_div, test_div_negative_scale_diff);
  tcase_add_test(tc_s21_div, test_div_positive_scale_diff);
  tcase_add_test(tc_s21_div, test_div_max_by_one);
  tcase_add_test(tc_s21_div, test_div_overflow_positive);
  tcase_add_test(tc_s21_div, test_div_overflow_negative);
  tcase_add_test(tc_s21_div, test_div_max_by_three);
  tcase_add_test(tc_s21_div, test_div_one_by_max);
  tcase_add_test(tc_s21_div, test_div_by_small_scaled);
  tcase_add_test(tc_s21_div, test_div_exact_25);
  tcase_add_test(tc_s21_div, test_div_exact_eighth);
  tcase_add_test(tc_s21_div, test_div_then_mul);

  tcase_add_test(tc_s21_is_equal, test_is_equal);
  tcase_add_test(tc_s21_is_not_equal, test_is_not_equal);
  tcase_add_test(tc_s21_is_less, test_is_less);
  tcase_add_test(tc_s21_is_less_or_equal, test_is_less_or_equal);
  tcase_add_test(tc_s21_is_greater, test_is_greater);
  tcase_add_test(tc_s21_is_greater_or_equal, test_is_greater_or_equal);
  tcase_add_test(tc_s21_comparison, test_compare_with_scales);

  // подключение кейсов к сьюту
  suite_add_tcase(suite, tc_mantissa);
  suite_add_tcase(suite, tc_s21_from_int_to_decimal);
  suite_add_tcase(suite, tc_s21_from_float_to_decimal);
  suite_add_tcase(suite, tc_s21_from_decimal_to_int);
  suite_add_tcase(suite, tc_s21_from_decimal_to_float);
  suite_add_tcase(suite, tc_s21_add);
  suite_add_tcase(suite, tc_s21_sub);
  suite_add_tcase(suite, tc_s21_mul);
  suite_add_tcase(suite, tc_s21_div);
  suite_add_tcase(suite, tc_s21_comparison);
  suite_add_tcase(suite, tc_s21_is_less);
  suite_add_tcase(suite, tc_s21_is_less_or_equal);
  suite_add_tcase(suite, tc_s21_is_greater);
  suite_add_tcase(suite, tc_s21_is_greater_or_equal);
  suite_add_tcase(suite, tc_s21_is_equal);
  suite_add_tcase(suite, tc_s21_is_not_equal);

  // floor
  tcase_add_test(tc_s21_floor, test_floor_positive_int);
  tcase_add_test(tc_s21_floor, test_floor_positive_fraction);
  tcase_add_test(tc_s21_floor, test_floor_negative_exact);
  tcase_add_test(tc_s21_floor, test_floor_negative_fraction);
  tcase_add_test(tc_s21_floor, test_floor_negative_int);
  tcase_add_test(tc_s21_floor, test_floor_zero_scale_positive);
  tcase_add_test(tc_s21_floor, test_floor_zero);
  tcase_add_test(tc_s21_floor, test_floor_null_result);
  tcase_add_test(tc_s21_floor, test_floor_large_positive);
  tcase_add_test(tc_s21_floor, test_floor_negative_small_fraction);
  tcase_add_test(tc_s21_floor, test_floor_multi_scale);
  tcase_add_test(tc_s21_floor, test_floor_multi_scale_negative);
  tcase_add_test(tc_s21_floor, test_floor_zero_sign);
  suite_add_tcase(suite, tc_s21_floor);

  // round
  tcase_add_test(tc_s21_round, test_round_half_to_even_down);
  tcase_add_test(tc_s21_round, test_round_half_to_even_up);
  tcase_add_test(tc_s21_round, test_round_down);
  tcase_add_test(tc_s21_round, test_round_up);
  tcase_add_test(tc_s21_round, test_round_negative_half);
  tcase_add_test(tc_s21_round, test_round_negative_up);
  tcase_add_test(tc_s21_round, test_round_integer_unchanged);
  tcase_add_test(tc_s21_round, test_round_null_result);
  tcase_add_test(tc_s21_round, test_round_multi_scale);
  tcase_add_test(tc_s21_round, test_round_multi_scale_half);
  tcase_add_test(tc_s21_round, test_round_zero);
  tcase_add_test(tc_s21_round, test_round_half_zero);
  tcase_add_test(tc_s21_round, test_round_negative_int);
  tcase_add_test(tc_s21_round, test_round_149_is_1);
  tcase_add_test(tc_s21_round, test_round_151_is_2);
  tcase_add_test(tc_s21_round, test_round_tail_nonzero);
  tcase_add_test(tc_s21_round, test_round_2500_is_2);
  tcase_add_test(tc_s21_round, test_round_1500_is_2);
  suite_add_tcase(suite, tc_s21_round);

  // truncate
  tcase_add_test(tc_s21_truncate, test_truncate_positive);
  tcase_add_test(tc_s21_truncate, test_truncate_negative);
  tcase_add_test(tc_s21_truncate, test_truncate_no_scale);
  tcase_add_test(tc_s21_truncate, test_truncate_scale_zero);
  tcase_add_test(tc_s21_truncate, test_truncate_two_decimals);
  tcase_add_test(tc_s21_truncate, test_truncate_null_result);
  tcase_add_test(tc_s21_truncate, test_truncate_zero_fractional);
  tcase_add_test(tc_s21_truncate, test_truncate_no_rounding);
  tcase_add_test(tc_s21_truncate, test_truncate_sign_preserved);
  tcase_add_test(tc_s21_truncate, test_truncate_negative_less_than_one);
  tcase_add_test(tc_s21_truncate, test_truncate_large_scale);
  suite_add_tcase(suite, tc_s21_truncate);

  // negate
  tcase_add_test(tc_s21_negate, test_negate_positive);
  tcase_add_test(tc_s21_negate, test_negate_negative);
  tcase_add_test(tc_s21_negate, test_negate_mantissa_unchanged);
  tcase_add_test(tc_s21_negate, test_negate_scale_preserved);
  tcase_add_test(tc_s21_negate, test_negate_double);
  tcase_add_test(tc_s21_negate, test_negate_null_result);
  tcase_add_test(tc_s21_negate, test_negate_zero);
  tcase_add_test(tc_s21_negate, test_negate_int_max);
  tcase_add_test(tc_s21_negate, test_negate_negative_scaled);
  suite_add_tcase(suite, tc_s21_negate);

  // (пустые tc_s21_add, tc_s21_sub

  return suite;
}
#pragma endregion

int main(void) {
  Suite* suite = s21_decimal_suite();
  SRunner* runner = srunner_create(suite);

  srunner_run_all(runner, CK_VERBOSE);

  int failed_count = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (failed_count == 0) ? 0 : 1;
}
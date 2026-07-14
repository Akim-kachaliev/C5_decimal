#include "s21_decimal.h"

#include <float.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "s21_mantissa.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  // проверка на NULL
  if (result == NULL) {
    return 1;
  }

  s21_decimal temp_value_1 = value_1;
  s21_decimal temp_value_2 = value_2;

  // выравнивание масштаба
  int scale1 = GET_SCALE(temp_value_1);
  int scale2 = GET_SCALE(temp_value_2);
  int max_scale = scale1;
  if (scale2 > scale1) {
    max_scale = scale2;
  }

  while (GET_SCALE(temp_value_1) < max_scale) {
    if (s21_mul_mantissa_by_10(&temp_value_1)) return 1;
    SET_SCALE(temp_value_1, GET_SCALE(temp_value_1) + 1);
  }

  while (GET_SCALE(temp_value_2) < max_scale) {
    if (s21_mul_mantissa_by_10(&temp_value_2)) return 1;
    SET_SCALE(temp_value_2, GET_SCALE(temp_value_2) + 1);
  }

  // опредение знака
  int sign1 = GET_SIGN(temp_value_1);
  int sign2 = GET_SIGN(temp_value_2);

  if (sign1 == sign2) {
    int ans = s21_add_mantissa(temp_value_1, temp_value_2, result);
    if (ans) return 1;  // переполнение

    SET_SCALE(*result, max_scale);
    SET_SIGN(*result, sign1);

    // если результат 0, знак должен быть положительным
    if (s21_is_mantissa_zero(*result)) SET_SIGN(*result, 0);
  } else {
    int ans = s21_cmp_mantissa(temp_value_1, temp_value_2);

    if (ans >= 0) {
      s21_sub_mantissa(temp_value_1, temp_value_2, result);
      SET_SIGN(*result, sign1);
    } else {
      s21_sub_mantissa(temp_value_2, temp_value_1, result);
      SET_SIGN(*result, sign2);
    }

    SET_SCALE(*result, max_scale);

    if (s21_is_mantissa_zero(*result)) SET_SIGN(*result, 0);
  }

  return 0;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (result == NULL) return 1;

  s21_decimal negated;

  // меняем знак второго числа
  s21_negate(value_2, &negated);
  // сложение с отрицательным
  return s21_add(value_1, negated, result);
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (result == NULL) return 1;

  int sign1 = GET_SIGN(value_1);
  int sign2 = GET_SIGN(value_2);
  int result_sign = sign1 ^ sign2;
  int result_scale = GET_SCALE(value_1) + GET_SCALE(value_2);

  int error = 0;

  if (s21_is_mantissa_zero(value_1) || s21_is_mantissa_zero(value_2)) {
    *result = (s21_decimal){{0, 0, 0, 0}};
  } else {
    s21_big_decimal big = s21_mul_big_mantissa(value_1, value_2);
    error = s21_fit_big_to_decimal(big, result_scale, result_sign, result);
  }

  return error;
}

// одна десятичная цифра (0..9), dividend < divisor*10 на входе
static void s21_div_step(s21_big_decimal* dividend, s21_big_decimal divisor,
                         unsigned int* digit) {
  unsigned int q = 0;
  while (s21_cmp_big(*dividend, divisor) >= 0) {
    s21_sub_big_inplace(dividend, divisor);
    q++;
  }
  *digit = q;
}

// целая часть через shift-and-subtract по битам.
// в лоб вычитанием не пойдёт, цифра в 96 битах огромная
static void s21_div_integer_part(s21_big_decimal* dividend,
                                 s21_big_decimal divisor,
                                 s21_big_decimal* quotient_int) {
  s21_big_decimal q = {{0}};
  s21_big_decimal r = {{0}};
  int n = s21_bitlen_big(*dividend);
  for (int i = n - 1; i >= 0; i--) {
    s21_shl_big_inplace(&r);
    if (s21_get_bit_big(*dividend, i)) s21_add_big_small(&r, 1);
    if (s21_cmp_big(r, divisor) >= 0) {
      s21_sub_big_inplace(&r, divisor);
      s21_shl_big_inplace(&q);
      s21_add_big_small(&q, 1);
    } else {
      s21_shl_big_inplace(&q);
    }
  }
  *quotient_int = q;
  *dividend = r;
}

// quotient = quotient * 10 + (dividend / divisor); dividend = остаток
static int s21_div_one_digit(s21_big_decimal* dividend, s21_big_decimal divisor,
                             s21_big_decimal* quotient, int* scale) {
  unsigned int digit = 0;
  int error = 0;
  s21_div_step(dividend, divisor, &digit);
  if (s21_mul_big_by_10(quotient)) error = 1;
  if (!error && s21_add_big_small(quotient, digit)) error = 1;
  if (!error) (*scale)++;
  return error;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (result == NULL) return 1;
  if (s21_is_mantissa_zero(value_2)) return 3;

  int sign = GET_SIGN(value_1) ^ GET_SIGN(value_2);
  int scale = GET_SCALE(value_1) - GET_SCALE(value_2);
  int error = 0;

  if (s21_is_mantissa_zero(value_1)) {
    *result = (s21_decimal){{0, 0, 0, 0}};
  } else {
    s21_big_decimal dividend = s21_load_mantissa_to_big(value_1);
    s21_big_decimal divisor = s21_load_mantissa_to_big(value_2);
    s21_big_decimal quotient = {{0}};

    s21_div_integer_part(&dividend, divisor, &quotient);

    // дробные цифры до scale=28
    while (!error && scale < 28 && !s21_is_big_zero(dividend)) {
      s21_mul_big_by_10(&dividend);
      error = s21_div_one_digit(&dividend, divisor, &quotient, &scale);
    }
    // ещё одна цифра для банковского округления в fit
    if (!error && !s21_is_big_zero(dividend)) {
      s21_mul_big_by_10(&dividend);
      error = s21_div_one_digit(&dividend, divisor, &quotient, &scale);
    }
    // догон если scale ушёл в минус (типа 100 / 0.04)
    while (!error && scale < 0) {
      if (s21_mul_big_by_10(&quotient)) error = (sign == 0) ? 1 : 2;
      scale++;
    }

    if (!error) error = s21_fit_big_to_decimal(quotient, scale, sign, result);
  }
  return error;
}

int s21_compare(s21_decimal value_1, s21_decimal value_2) {
  // 1. Проверка на ноль (по стандарту +0 == -0)
  if (s21_is_mantissa_zero(value_1) && s21_is_mantissa_zero(value_2)) {
    return 0;
  }

  int sign1 = GET_SIGN(value_1);
  int sign2 = GET_SIGN(value_2);

  // 2. Если знаки разные, положительное число всегда больше
  if (sign1 != sign2) {
    return (sign1 == 0) ? 1 : -1;
  }

  // 3. Подготовка к сравнению при одинаковых знаках
  // Используем big_decimal (192 бита), чтобы выровнять scale без переполнения
  s21_big_decimal b1 = s21_load_mantissa_to_big(value_1);
  s21_big_decimal b2 = s21_load_mantissa_to_big(value_2);
  int scale1 = GET_SCALE(value_1);
  int scale2 = GET_SCALE(value_2);

  // 4. Нормализация (приведение к общему знаменателю/scale)
  // Умножаем число с меньшим scale на 10, пока степени не сравняются
  while (scale1 < scale2) {
    s21_mul_big_by_10(&b1);
    scale1++;
  }
  while (scale2 < scale1) {
    s21_mul_big_by_10(&b2);
    scale2++;
  }

  // 5. Сравниваем мантиссы (уже в big_decimal)
  int res = s21_cmp_big(b1, b2);

  // 6. Если оба числа были отрицательными, результат инвертируется
  // (например, -5 < -3, хотя 5 > 3)
  if (sign1 == 1 && res != 0) {
    res = -res;
  }

  return res;
}

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  return s21_compare(value_1, value_2) == -1;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int res = s21_compare(value_1, value_2);
  return (res == -1 || res == 0);
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  return s21_compare(value_1, value_2) == 1;
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int res = s21_compare(value_1, value_2);
  return (res == 1 || res == 0);
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  return s21_compare(value_1, value_2) == 0;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return s21_compare(value_1, value_2) != 0;
}

int s21_from_int_to_decimal(int src, s21_decimal* dst) {
  if (dst == NULL) return 1;

  dst->bits[0] = 0;
  dst->bits[1] = 0;
  dst->bits[2] = 0;
  dst->bits[3] = 0;

  if (src < 0) {
    SET_SIGN(*dst, 1);
    // INT_MIN = -2147483648, -src переполнится так что тут я отдельно
    // обрабатываю
    dst->bits[0] = (src == INT_MIN) ? (int)2147483648U : -src;
  } else {
    dst->bits[0] = src;
  }

  return 0;
}

int s21_from_float_to_decimal(float src, s21_decimal* dst) {
  if (dst == NULL) return 1;
  if (isnan(src) || isinf(src)) return 1;

  memset(dst, 0, sizeof(s21_decimal));
  if (src == 0.0f) return 0;

  int sign = (src < 0.0f) ? 1 : 0;
  double abs_val = fabs((double)src);

  if (abs_val > 7.9228162514264337593543950335e28) return 1;
  if (abs_val < 1e-28) return 1;

  // степень 10
  int exp10 = (int)floor(log10(abs_val));

  // масштабирование до 8 значимых цифр (1 лишняя будет для банковского
  // округления)
  double factor = pow(10.0, 7 - exp10);
  long long m8 = (long long)(abs_val * factor);

  // корректировка exp10 если floor(log10) промахнулся на 1
  while (m8 < 10000000LL) {
    m8 *= 10;
    exp10--;
  }
  while (m8 >= 100000000LL) {
    m8 /= 10;
    exp10++;
  }

  // банковское округление, те 8 цифр -> 7 цифр
  int last = (int)(m8 % 10);
  long long m7 = m8 / 10;
  if (last > 5 || (last == 5 && (m7 & 1))) m7++;

  if (m7 >= 10000000LL) {
    m7 /= 10;
    exp10++;
  }

  int scale = 6 - exp10;

  if (scale > 28) {
    int excess = scale - 28;
    for (int i = 0; i < excess; i++) {
      int rem = (int)(m7 % 10);
      m7 /= 10;
      if (rem > 5 || (rem == 5 && (m7 & 1))) m7++;
    }
    scale = 28;
  }

  dst->bits[0] = (int)(m7 & 0xFFFFFFFFLL);
  dst->bits[1] = (int)((m7 >> 32) & 0xFFFFFFFFLL);

  if (scale < 0) {
    for (int i = 0; i < -scale; i++) {
      if (s21_mul_mantissa_by_10(dst)) return 1;
    }
    scale = 0;
  }

  SET_SCALE(*dst, scale);
  SET_SIGN(*dst, sign);
  if (s21_is_mantissa_zero(*dst)) SET_SIGN(*dst, 0);

  return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int* dst) {
  if (dst == NULL) return 1;

  s21_decimal truncated;
  if (s21_truncate(src, &truncated)) return 1;

  // если биты 1 или 2 заняты то в int не влезет
  if ((unsigned int)truncated.bits[1] != 0 ||
      (unsigned int)truncated.bits[2] != 0)
    return 1;

  int sign = GET_SIGN(truncated);
  unsigned int val = (unsigned int)truncated.bits[0];

  if (sign == 0) {
    if (val > (unsigned int)INT_MAX) return 1;
    *dst = (int)val;
  } else {
    if (val > 2147483648U) return 1;
    *dst = (val == 2147483648U) ? INT_MIN : -(int)val;
  }

  return 0;
}
int s21_from_decimal_to_float(s21_decimal src, float* dst) {
  if (dst == NULL) return 1;

  double result = 0.0;
  result += (double)(unsigned int)src.bits[0];
  result += (double)(unsigned int)src.bits[1] * 4294967296.0;            // 2^32
  result += (double)(unsigned int)src.bits[2] * 18446744073709551616.0;  // 2^64

  int scale = GET_SCALE(src);
  for (int i = 0; i < scale; i++) result /= 10.0;

  if (GET_SIGN(src) && result != 0.0) result = -result;

  *dst = (float)result;
  return 0;
}

int s21_floor(s21_decimal value, s21_decimal* result) {
  if (result == NULL) return 1;

  int sign = GET_SIGN(value);
  int scale = GET_SCALE(value);

  s21_decimal truncated;
  s21_truncate(value, &truncated);

  if (scale == 0) {
    *result = truncated;
    return 0;
  }

  // Проверяем: есть ли ненулевая дробная часть
  // Делаем копию и последовательно делим, если хоть один остаток ненулевой,
  // дробная часть была
  s21_decimal tmp = value;
  int had_nonzero_fraction = 0;
  for (int sc = scale; sc > 0; sc--) {
    int rem = div_mantissa_by_10(&tmp);
    if (rem != 0) {
      had_nonzero_fraction = 1;
      break;
    }
  }

  *result = truncated;

  // если отрицательное с ненулевой дробью, то вычитаем 1 (идём к минус
  // бесконечность)
  if (sign == 1 && had_nonzero_fraction) {
    unsigned long long carry = 1;
    for (int i = 0; i < 3 && carry; i++) {
      unsigned long long sum = (unsigned int)result->bits[i] + carry;
      result->bits[i] = (int)(sum & 0xFFFFFFFF);
      carry = sum >> 32;
    }
    SET_SIGN(*result, 1);
    if (carry) return 1;
  }

  return 0;
}

int s21_round(s21_decimal value, s21_decimal* result) {
  if (result == NULL) return 1;

  int scale = GET_SCALE(value);
  int sign = GET_SIGN(value);

  if (scale == 0) {
    *result = value;
    return 0;
  }

  s21_decimal truncated;
  s21_truncate(value, &truncated);

  s21_decimal tmp = value;
  SET_SIGN(tmp, 0);

  int has_tail = 0;
  for (int i = 0; i < scale - 1; i++) {
    int rem = div_mantissa_by_10(&tmp);
    if (rem != 0) has_tail = 1;
  }
  int first_frac = div_mantissa_by_10(&tmp);

  int do_round = 0;
  if (first_frac > 5) {
    do_round = 1;
  } else if (first_frac == 5) {
    if (has_tail) {
      do_round = 1;
    } else {
      do_round = (truncated.bits[0] & 1);
    }
  }

  *result = truncated;

  if (do_round) {
    unsigned long long carry = 1;
    for (int i = 0; i < 3 && carry; i++) {
      unsigned long long sum = (unsigned int)result->bits[i] + carry;
      result->bits[i] = (int)(sum & 0xFFFFFFFF);
      carry = sum >> 32;
    }
    if (carry) return 1;
  }

  SET_SIGN(*result, sign);
  if (s21_is_mantissa_zero(*result)) SET_SIGN(*result, 0);

  return 0;
}

int s21_truncate(s21_decimal value, s21_decimal* result) {
  if (result == NULL) return 1;

  *result = value;
  int scale = GET_SCALE(value);
  int sign = GET_SIGN(value);

  while (scale > 0) {
    div_mantissa_by_10(result);
    scale--;
  }

  result->bits[3] = 0;
  SET_SIGN(*result, sign);

  return 0;
}

int s21_negate(s21_decimal value, s21_decimal* result) {
  if (result == NULL) return 1;

  *result = value;
  SET_SIGN(*result, !GET_SIGN(value));

  return 0;
}
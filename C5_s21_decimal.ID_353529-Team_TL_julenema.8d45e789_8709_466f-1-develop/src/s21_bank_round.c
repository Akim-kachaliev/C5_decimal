#include "s21_decimal.h"

// делит мантиссу на 10, возвращает остаток
int div_mantissa_by_10(s21_decimal* d) {
  unsigned long long rem = 0;

  for (int i = 2; i >= 0; i--) {
    unsigned long long cur = (rem << 32) | (unsigned int)d->bits[i];
    d->bits[i] = (int)(cur / 10);
    rem = cur % 10;
  }

  return (int)rem;
}

// прибавляет 1 к мантиссе, возвращает 1 если переполнение (мантисса > 2^96-1)
static int inc_mantissa(s21_decimal* d) {
  unsigned long long carry = 1;

  for (int i = 0; i < 3 && carry; i++) {
    unsigned long long sum = (unsigned int)d->bits[i] + carry;
    d->bits[i] = (int)(sum & 0xFFFFFFFF);
    carry = sum >> 32;
  }

  return (int)carry;
}

// проверяет равна ли мантисса нулю
static int mantissa_is_zero(s21_decimal d) {
  return d.bits[0] == 0 && d.bits[1] == 0 && d.bits[2] == 0;
}

// Округляет число, уменьшая scale на 1 (банковское округление: 0.5 → до
// ближайшего чётного). Возвращает 1 при переполнении мантиссы.
int s21_bank_round(s21_decimal value, s21_decimal* result) {
  *result = value;

  int scale = GET_SCALE(value);
  int sign = GET_SIGN(value);

  if (scale == 0) return 0;

  int dropped = div_mantissa_by_10(result);
  SET_SCALE(*result, scale - 1);

  int do_round = 0;
  if (dropped > 5) {
    do_round = 1;
  } else if (dropped == 5) {
    do_round = result->bits[0] & 1;  // округляем если мантисса нечётная
  }

  if (do_round) {
    if (inc_mantissa(result)) return 1;
  }

  SET_SIGN(*result, sign);

  if (mantissa_is_zero(*result)) SET_SIGN(*result, 0);

  return 0;
}

// Уменьшает scale до target_scale, многократно применяя банковское округление.
// Возвращает 1 если при округлении произошло переполнение. Нужна для mul/div
int s21_normalize_scale(s21_decimal value, int target_scale,
                        s21_decimal* result) {
  *result = value;

  while (GET_SCALE(*result) > target_scale) {
    if (s21_bank_round(*result, result)) return 1;
  }

  return 0;
}
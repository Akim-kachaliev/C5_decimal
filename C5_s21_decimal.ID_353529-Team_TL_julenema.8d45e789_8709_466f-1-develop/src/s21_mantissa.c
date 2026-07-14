#include "s21_mantissa.h"

// проверка мантиссы на ноль
int s21_is_mantissa_zero(s21_decimal d) {
  int result = (d.bits[0] == 0 && d.bits[1] == 0 && d.bits[2] == 0);
  return result;
}

// сравнение мантиссы двух decimal и возврат
// -1, если мантисса a меньше мантиссы b:
// 1, если мантисса a больше мантиссы b
int s21_cmp_mantissa(s21_decimal a, s21_decimal b) {
  int result = 0;
  if ((unsigned int)a.bits[2] > (unsigned int)b.bits[2]) {
    result = 1;
  } else if ((unsigned int)a.bits[2] < (unsigned int)b.bits[2]) {
    result = -1;
  } else if ((unsigned int)a.bits[1] > (unsigned int)b.bits[1]) {
    result = 1;
  } else if ((unsigned int)a.bits[1] < (unsigned int)b.bits[1]) {
    result = -1;
  } else if ((unsigned int)a.bits[0] > (unsigned int)b.bits[0]) {
    result = 1;
  } else if ((unsigned int)a.bits[0] < (unsigned int)b.bits[0]) {
    result = -1;
  }
  return result;
}

// сложение мантиссы двух decimal и сохранение результата в res
int s21_add_mantissa(s21_decimal a, s21_decimal b, s21_decimal* res) {
  unsigned long long carry = 0;
  for (int i = 0; i < 3; i++) {
    unsigned long long sum = (unsigned long long)(unsigned int)a.bits[i] +
                             (unsigned int)b.bits[i] + carry;
    res->bits[i] = (unsigned int)sum;
    carry = sum >> 32;
  }
  int result = (carry != 0);
  return result;
}

// вычитание мантиссы b из мантиссы a и сохранение результата в res
int s21_sub_mantissa(s21_decimal a, s21_decimal b, s21_decimal* res) {
  unsigned long long borrow = 0;
  for (int i = 0; i < 3; i++) {
    unsigned long long minuend = (unsigned int)a.bits[i];
    unsigned long long subtrahend =
        (unsigned long long)(unsigned int)b.bits[i] + borrow;

    if (minuend < subtrahend) {
      minuend += (1ULL << 32);  // 2^32
      borrow = 1;
    } else {
      borrow = 0;
    }
    res->bits[i] = (unsigned int)(minuend - subtrahend);
  }
  int result = (borrow != 0);
  return result;
}

// умножение мантиссы decimal на 10 и сохранение результата в d
int s21_mul_mantissa_by_10(s21_decimal* d) {
  unsigned long long carry = 0;
  for (int i = 0; i < 3; i++) {
    unsigned long long product =
        (unsigned long long)(unsigned int)d->bits[i] * 10 + carry;
    d->bits[i] = (unsigned int)product;
    carry = product >> 32;
  }
  int result = (carry != 0);
  return result;
}

// умножение двух мантисс в столбик по 32-битным кускам
s21_big_decimal s21_mul_big_mantissa(s21_decimal a, s21_decimal b) {
  s21_big_decimal result = {{0}};
  for (int i = 0; i < 3; i++) {
    unsigned long long carry = 0;
    for (int j = 0; j < 3; j++) {
      unsigned long long product =
          (unsigned long long)(unsigned int)a.bits[i] *
              (unsigned long long)(unsigned int)b.bits[j] +
          result.parts[i + j] + carry;
      result.parts[i + j] = (unsigned int)product;
      carry = product >> 32;
    }
    result.parts[i + 3] = (unsigned int)carry;
  }
  return result;
}

// проверяет, что big_decimal влезает в 96 бит
// (parts[3], parts[4], parts[5] все нули)
static int s21_big_fits_in_96(s21_big_decimal big) {
  int result = (big.parts[3] == 0 && big.parts[4] == 0 && big.parts[5] == 0);
  return result;
}

// делит big_decimal на 10, возвращает остаток
static int s21_big_div_by_10(s21_big_decimal* big) {
  unsigned long long rem = 0;
  for (int i = 5; i >= 0; i--) {
    unsigned long long cur = (rem << 32) | big->parts[i];
    big->parts[i] = (unsigned int)(cur / 10);
    rem = cur % 10;
  }
  return (int)rem;
}

// ужимает big_decimal в decimal: сначала пока не влезает в 96 бит,
// потом ещё пока scale > 28 (стандарт допускает только 0..28).
// возвращает 0 при успехе, 1/2 при переполнении.
int s21_fit_big_to_decimal(s21_big_decimal big, int scale, int sign,
                           s21_decimal* result) {
  int last_rem = 0;
  int error = 0;

  while ((!s21_big_fits_in_96(big) || scale > 28) && scale > 0) {
    last_rem = s21_big_div_by_10(&big);
    scale--;
  }

  if (!s21_big_fits_in_96(big)) {
    error = (sign == 0) ? 1 : 2;
  } else {
    result->bits[0] = (int)big.parts[0];
    result->bits[1] = (int)big.parts[1];
    result->bits[2] = (int)big.parts[2];
    result->bits[3] = 0;
    SET_SCALE(*result, scale);
    SET_SIGN(*result, sign);

    if (last_rem > 5) {
      s21_decimal one = {{1, 0, 0, 0}};
      s21_add_mantissa(*result, one, result);
    } else if (last_rem == 5) {
      if (result->bits[0] & 1) {
        s21_decimal one = {{1, 0, 0, 0}};
        s21_add_mantissa(*result, one, result);
      }
    }
  }
  return error;
}

// === 192-битные хелперы для длинного деления ===

// big == 0?
int s21_is_big_zero(s21_big_decimal big) {
  int result = 1;
  for (int i = 0; i < 6; i++) {
    if (big.parts[i] != 0) result = 0;
  }
  return result;
}

// сравнение двух big, идём от старших слов к младшим
int s21_cmp_big(s21_big_decimal a, s21_big_decimal b) {
  int result = 0;
  for (int i = 5; i >= 0 && result == 0; i--) {
    if (a.parts[i] > b.parts[i]) {
      result = 1;
    } else if (a.parts[i] < b.parts[i]) {
      result = -1;
    }
  }
  return result;
}

// a -= b на месте, требуется a >= b
int s21_sub_big_inplace(s21_big_decimal* a, s21_big_decimal b) {
  unsigned long long borrow = 0;
  for (int i = 0; i < 6; i++) {
    unsigned long long minuend = a->parts[i];
    unsigned long long subtrahend = (unsigned long long)b.parts[i] + borrow;
    if (minuend < subtrahend) {
      minuend += (1ULL << 32);
      borrow = 1;
    } else {
      borrow = 0;
    }
    a->parts[i] = (unsigned int)(minuend - subtrahend);
  }
  return (int)borrow;
}

// big *= 10
int s21_mul_big_by_10(s21_big_decimal* big) {
  unsigned long long carry = 0;
  for (int i = 0; i < 6; i++) {
    unsigned long long product = (unsigned long long)big->parts[i] * 10 + carry;
    big->parts[i] = (unsigned int)product;
    carry = product >> 32;
  }
  return (carry != 0);
}

// big += x с каскадным переносом
int s21_add_big_small(s21_big_decimal* big, unsigned int x) {
  unsigned long long carry = x;
  for (int i = 0; i < 6 && carry; i++) {
    unsigned long long sum = (unsigned long long)big->parts[i] + carry;
    big->parts[i] = (unsigned int)sum;
    carry = sum >> 32;
  }
  return (int)carry;
}

// мантисса decimal в big, старшие слова в нули
s21_big_decimal s21_load_mantissa_to_big(s21_decimal d) {
  s21_big_decimal big = {{0}};
  big.parts[0] = (unsigned int)d.bits[0];
  big.parts[1] = (unsigned int)d.bits[1];
  big.parts[2] = (unsigned int)d.bits[2];
  return big;
}

// сдвиг big на 1 бит влево, старший бит уезжает в небытие
void s21_shl_big_inplace(s21_big_decimal* big) {
  unsigned int carry = 0;
  for (int i = 0; i < 6; i++) {
    unsigned int new_carry = (big->parts[i] >> 31) & 1u;
    big->parts[i] = (big->parts[i] << 1) | carry;
    carry = new_carry;
  }
}

// позиция старшего единичного бита + 1, для нуля 0
int s21_bitlen_big(s21_big_decimal big) {
  int len = 0;
  for (int i = 5; i >= 0 && len == 0; i--) {
    if (big.parts[i] != 0) {
      unsigned int v = big.parts[i];
      int bit = 0;
      while (v != 0) {
        v >>= 1;
        bit++;
      }
      len = i * 32 + bit;
    }
  }
  return len;
}

// i-й бит big
int s21_get_bit_big(s21_big_decimal big, int i) {
  int word = i / 32;
  int off = i % 32;
  return (int)((big.parts[word] >> off) & 1u);
}
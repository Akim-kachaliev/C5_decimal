#ifndef S21_MANTISSA_H
#define S21_MANTISSA_H

#include "s21_decimal.h"

// 192-битное число: для умножения 96x96 (mul) и для длинного деления (div)
typedef struct {
  unsigned int parts[6];
} s21_big_decimal;

// Проверяет, равна ли мантисса нулю
// Возвращает 1, если bits[0..2] все нули, иначе 0
int s21_is_mantissa_zero(s21_decimal d);

// Сравнивает мантиссы двух decimal (без учёта знака и scale)
// Возвращает 1 если a>b, -1 если a<b, 0 если равны.
int s21_cmp_mantissa(s21_decimal a, s21_decimal b);

// Складывает мантиссы a и b, результат кладёт в res->bits[0..2]
// Поле res->bits[3] не изменяется.
// Возвращает 1 при переполнении за 96 бит, иначе 0.
int s21_add_mantissa(s21_decimal a, s21_decimal b, s21_decimal* res);

// Вычитает мантиссу b из a, результат в res в bits[0..2]
// Требует, чтобы a >= b по мантиссе
// Поле res->bits[3] не изменяется
// Возвращает 0 при успехе, 1 если a < b.
int s21_sub_mantissa(s21_decimal a, s21_decimal b, s21_decimal* res);

// Умножает мантиссу d на 10. Поле bits[3] не изменяется
// Возвращает 1 при переполнении за 96 бит, иначе 0
int s21_mul_mantissa_by_10(s21_decimal* d);

// Умножает две 96-битные мантиссы. Результат всегда влезает в 192 бита
s21_big_decimal s21_mul_big_mantissa(s21_decimal a, s21_decimal b);

// Ужимает big_decimal в decimal, уменьшая scale и округляя
// возвращает 0 при успехе, 1/2 при переполнении (по знаку)
int s21_fit_big_to_decimal(s21_big_decimal big, int scale, int sign,
                           s21_decimal* result);

// big == 0?
int s21_is_big_zero(s21_big_decimal big);

// сравнение двух big
int s21_cmp_big(s21_big_decimal a, s21_big_decimal b);

// a -= b на месте, требуется a >= b
int s21_sub_big_inplace(s21_big_decimal* a, s21_big_decimal b);

// big *= 10
int s21_mul_big_by_10(s21_big_decimal* big);

// big += x с каскадным переносом
int s21_add_big_small(s21_big_decimal* big, unsigned int x);

// мантисса decimal в big, старшие слова в нули
s21_big_decimal s21_load_mantissa_to_big(s21_decimal d);

// сдвиг big на 1 бит влево
void s21_shl_big_inplace(s21_big_decimal* big);

// позиция старшего единичного бита + 1, для нуля 0
int s21_bitlen_big(s21_big_decimal big);

// i-й бит big
int s21_get_bit_big(s21_big_decimal big, int i);

#endif
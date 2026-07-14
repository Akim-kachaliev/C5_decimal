#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

// Макросы для работы с bits[3]

#define GET_SCALE(d) (((d).bits[3] >> 16) & 0xFF)
#define SET_SCALE(d, s) \
  ((d).bits[3] = ((d).bits[3] & ~(0xFF << 16)) | (((s) & 0xFF) << 16))
#define GET_SIGN(d) (((unsigned int)(d).bits[3] >> 31) & 1)
#define SET_SIGN(d, s) \
  ((d).bits[3] = ((d).bits[3] & 0x7FFFFFFF) | (((s) & 1) << 31))

typedef struct {
  int bits[4];
} s21_decimal;

int div_mantissa_by_10(s21_decimal* d);
int s21_bank_round(s21_decimal value, s21_decimal* result);

// (Ваня)
int s21_add(s21_decimal value_1, s21_decimal value_2,
            s21_decimal* result);  // +
// (Ваня)
int s21_sub(s21_decimal value_1, s21_decimal value_2,
            s21_decimal* result);  // -
// (Оля)
int s21_mul(s21_decimal value_1, s21_decimal value_2,
            s21_decimal* result);  // *
// (Оля)
int s21_div(s21_decimal value_1, s21_decimal value_2,
            s21_decimal* result);  // /

// (Аким)
int s21_is_less(s21_decimal, s21_decimal);  // <
// (Аким)
int s21_is_less_or_equal(s21_decimal, s21_decimal);  // <=
// (Аким)
int s21_is_greater(s21_decimal, s21_decimal);  // >
// (Аким)
int s21_is_greater_or_equal(s21_decimal, s21_decimal);  // >=
// (Аким)
int s21_is_equal(s21_decimal, s21_decimal);  // ==
// (Аким)
int s21_is_not_equal(s21_decimal, s21_decimal);  // !=

//(Ната)
int s21_from_int_to_decimal(int src, s21_decimal* dst);  // из int
//(Ната)
int s21_from_float_to_decimal(float src, s21_decimal* dst);  // из float
//(Ната)
int s21_from_decimal_to_int(s21_decimal src, int* dst);  // в int
//(Ната)
int s21_from_decimal_to_float(s21_decimal src, float* dst);  // в float

// Округляет указанное Decimal число до ближайшего целого числа в сторону
// отрицательной бесконечности.
int s21_floor(s21_decimal value, s21_decimal* result);  // (Артем)

// Округляет Decimal до ближайшего целого числа.
int s21_round(s21_decimal value, s21_decimal* result);  // (Артем)

// Возвращает целые цифры указанного Decimal числа; любые дробные цифры
// отбрасываются, включая конечные нули
int s21_truncate(s21_decimal value, s21_decimal* result);  // (Артем)

// Возвращает результат умножения указанного Decimal на -1.
int s21_negate(s21_decimal value, s21_decimal* result);  // (артем)

#endif  // S21_DECIMAL_H
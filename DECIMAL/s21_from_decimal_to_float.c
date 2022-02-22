#include "s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int scale = get_scale(src), pow_dec = my_pow(10, scale);
  int res = 0;

  s21_from_decimal_to_int(src, &res);
  *dst = (float)res / pow_dec;

  return 0;
}

#include "s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int sign = 0, res = 0, scale = 0;
  if (src < 0) {
    sign = 1;
    src *= -1;
  }
  float cln = src;

  while (cln) {
    while (cln > 0) cln -= 1;
    if (cln) cln = src;
    src *= 10;
    scale++;
  }
  res = (int)src;
  while (res % 10 == 0) {
    res /= 10;
    scale--;
  }

  s21_from_int_to_decimal(res, dst);
  set_scale(dst, scale);
  if (sign) set_sign(dst, 1);

  return 0;
}

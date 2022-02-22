#include "s21_decimal.h"

s21_decimal s21_floor(s21_decimal number) {
  int sign = get_sign(number), scale = get_scale(number);
  s21_decimal res = number, scale_dec = set_scale_to_dec(scale_dec, scale);

  number = mod_dec(number, scale_dec);
  res = sub_dec(res, number);
  res = div_dec(res, scale_dec);

  if (sign && number.bits[L]) {
    res.bits[L] += 1;
  }

  set_sign(&res, sign);
  return res;
}

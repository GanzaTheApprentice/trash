#include "s21_decimal.h"

s21_decimal s21_mod(s21_decimal one, s21_decimal two) {
  int sign_1 = get_sign(one), sign_2 = get_sign(two);
  int scale_1 = get_scale(one), scale_2 = get_scale(two);
  set_scale(&one, scale_1);
  set_scale(&two, scale_2);
  s21_decimal res;
  init_dec(&res);

  scale_for_div(&one, &two);

  res = mod_dec(one, two);
  if (empty_dec(res)) {
    if (scale_1 > scale_2) {
      set_scale(&res, scale_1);
    } else {
      set_scale(&res, scale_2);
    }
  }

  if (sign_1 && sign_2)
    set_sign(&res, 1);
  else if (!sign_1 && sign_2)
    set_sign(&res, 1);
  return res;
}

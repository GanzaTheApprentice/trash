#include "s21_decimal.h"

s21_decimal s21_div(s21_decimal one, s21_decimal two) {
  int sign_1 = get_sign(one), sign_2 = get_sign(two);
  s21_decimal res, one_plus, ten_mul;
  init_dec(&res);
  init_dec(&one_plus);
  init_dec(&ten_mul);
  one_plus.bits[L] = 1;
  ten_mul.bits[L] = 10;

  scale_for_div(&one, &two);

  int scale = 0;
  while (empty_dec(one)) {
    if (scale <= 28) {
      while (!s21_is_less(one, two)) {
        one = mul_dec(one, ten_mul);
        res = mul_dec(res, ten_mul);
        scale++;
      }
      one = sub_dec(one, two);
      res = add_dec(res, one_plus);
    } else {
      init_dec(&res);
      res.value_type = s21_INFINITY;
      break;
    }
  }
  if (scale <= 28 && scale != 0) {
    res.bits[S] = scale;
    set_scale(&res, scale);
  }
  if (sign_1 && !sign_2)
    set_sign(&res, 1);
  else if (!sign_1 && sign_2)
    set_sign(&res, 1);
  return res;
}

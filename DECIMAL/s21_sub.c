#include "s21_decimal.h"

s21_decimal s21_sub(s21_decimal one, s21_decimal two) {
  int flag = 0, scale_res, sign_1 = get_sign(one), sign_2 = get_sign(two);
  s21_decimal res;
  init_dec(&res);

  scale_for_sub(&one, &two, &res);
  scale_res = get_scale(res);
  set_sign(&one, sign_1);
  set_sign(&two, sign_2);

  if (s21_is_greater(one, two)) {
    if (sign_1 && !sign_2) {
      res = add_dec(one, two);
      flag = 1;
    } else if (sign_1 && sign_2) {
      res = sub_dec(one, two);
      flag = 1;
    } else {
      res = sub_dec(two, one);
      flag = 1;
    }
  } else {
    if (!sign_1 && sign_2) {
      res = add_dec(one, two);
    } else if (sign_1 && sign_2) {
      res = sub_dec(two, one);
    } else {
      res = sub_dec(one, two);
    }
  }
  set_scale(&res, scale_res);
  if (flag) set_sign(&res, 1);
  return res;
}

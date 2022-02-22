#include "s21_decimal.h"

s21_decimal s21_mul(s21_decimal one, s21_decimal two) {
  s21_decimal res, sub;
  init_dec(&res);
  init_dec(&sub);

  for (int i = 0; i < 96; i++) {
    int score = i;
    for (int j = 0; j < 96; j++) {
      if (get_bit(one, j) && get_bit(two, i)) {
        set_bit(&sub, j);
      } else {
        zero_bit(&sub, j);
      }
    }
    if (i > 0) {
      while (score) {
        great_shift(&sub);
        score--;
      }
    }
    res = add_dec(res, sub);
    init_dec(&sub);
  }

  scale_for_mul(&one, &two, &res);

  if (get_sign(one) && !get_sign(two))
    set_sign(&res, 1);
  else if (!get_sign(one) && get_sign(two))
    set_sign(&res, 1);

  return res;
}

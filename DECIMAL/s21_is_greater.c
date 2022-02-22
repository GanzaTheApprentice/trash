#include "s21_decimal.h"

int s21_is_greater(s21_decimal one, s21_decimal two) {
  int flag = 0, scl_1 = get_scale(one), scl_2 = get_scale(two);
  int scale = (scl_1 > scl_2) ? (scl_1 - scl_2) : (scl_2 - scl_1);
  int num_bits, sign_one = get_sign(one), sign_two = get_sign(two);
  s21_decimal scale_dec = set_scale_to_dec(scale_dec, scale);

  if (scl_1 > scl_2) {
    two = mul_dec(two, scale_dec);
  } else {
    one = mul_dec(one, scale_dec);
  }

  num_bits = which_bits(&one, &two);

  if (sign_one < sign_two) {
    flag = 0;
  } else if (sign_one > sign_two) {
    flag = 1;
  } else if (sign_one && sign_two) {
    if (one.bits[num_bits] > two.bits[num_bits]) flag = 1;
  } else if (!sign_one && !sign_two) {
    if (one.bits[num_bits] > two.bits[num_bits]) flag = 0;
  } else {
    flag = 1;
  }
  return flag;
}

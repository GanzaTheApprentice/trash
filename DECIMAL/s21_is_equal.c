#include "s21_decimal.h"

int s21_is_equal(s21_decimal one, s21_decimal two) {
  int flag = 0;
  for (int i = 0; i < 128; i++) {
    if (get_bit(one, i) != get_bit(two, i)) flag = 1;
    if (flag == 1) break;
  }
  return flag;
}

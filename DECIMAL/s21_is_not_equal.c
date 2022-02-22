#include "s21_decimal.h"

int s21_is_not_equal(s21_decimal one, s21_decimal two) {
  int flag = 1;
  for (int i = 0; i < 128; i++) {
    if (get_bit(one, i) != get_bit(two, i)) flag = 0;
    if (flag == 0) break;
  }
  return flag;
}

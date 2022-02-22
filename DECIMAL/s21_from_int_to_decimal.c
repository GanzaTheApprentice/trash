#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  if (src < 0) {
    set_sign(dst, 1);
    src *= -1;
  }
  for (int i = 0; i < 96; i++) {
    if (src % 2 != 0) {
      set_bit(dst, i);
      src -= 1;
      src /= 2;
    } else {
      zero_bit(dst, i);
      src /= 2;
    }
  }
  return 0;
}

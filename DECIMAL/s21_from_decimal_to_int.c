#include "s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int sign = get_sign(src);
  for (int i = 95; i >= 0; i--) {
    if (get_bit(src, i)) {
      *dst += my_pow(2, i);
    }
  }
  if (sign) {
    *dst *= -1;
  }
  return 0;
}

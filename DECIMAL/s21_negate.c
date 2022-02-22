#include "s21_decimal.h"

s21_decimal s21_negate(s21_decimal number) {
  swap_bit(&number, 127);
  if (number.value_type == s21_INFINITY) {
    number.value_type = s21_NEGATIVE_INFINITY;
  } else {
    number.value_type = s21_INFINITY;
  }
  return number;
}

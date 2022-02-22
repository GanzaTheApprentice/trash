#include <stdio.h>
#include <stdlib.h>
#include "subfunctions.h"

#define L 0
#define M 1
#define H 2
#define S 3
#define SCALE_MASK 0xFF0000

typedef enum {
  s21_NORMAL_VALUE = 0,
  s21_INFINITY = 1,
  s21_NEGATIVE_INFINITY = 2,
  s21_NAN = 3
} value_type_t;

typedef struct {
  unsigned int bits[4];
  value_type_t value_type;
} s21_decimal;

s21_decimal s21_add(s21_decimal one, s21_decimal two);
s21_decimal s21_mul(s21_decimal one, s21_decimal two);
s21_decimal s21_sub(s21_decimal one, s21_decimal two);
s21_decimal s21_div(s21_decimal one, s21_decimal two);
s21_decimal s21_mod(s21_decimal one, s21_decimal two);

int s21_is_equal(s21_decimal one, s21_decimal two);
int s21_is_not_equal(s21_decimal one, s21_decimal two);
int s21_is_greater(s21_decimal one, s21_decimal two);
int s21_is_greater_or_equal(s21_decimal one, s21_decimal two);
int s21_is_less(s21_decimal one, s21_decimal two);
int s21_is_less_or_equal(s21_decimal one, s21_decimal two);

s21_decimal s21_truncate(s21_decimal number);
s21_decimal s21_round(s21_decimal number);
s21_decimal s21_floor(s21_decimal number);
s21_decimal s21_negate(s21_decimal number);

int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

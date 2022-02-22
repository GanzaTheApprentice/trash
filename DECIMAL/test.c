#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int dec2bin(int num);
long long my_pow(int x, int y);
void output(s21_decimal result);
void init_dec(s21_decimal *number);
int which_bits(s21_decimal *one, s21_decimal *two);
void great_shift(s21_decimal *number);
void shift_one_bit(s21_decimal *number);
int get_bit(s21_decimal number, const int n);
int get_bit_ptr(s21_decimal *number, const int n);
void set_bit(s21_decimal *number, const int n);
void swap_bit(s21_decimal *number, const int n);
void zero_bit(s21_decimal *number, const int n);
int get_scale(s21_decimal number);
int get_sign(s21_decimal number);
void set_scale(s21_decimal *number, int scale);
void set_sign(s21_decimal *number, int sign);
s21_decimal mul_dec(s21_decimal one, s21_decimal two);
s21_decimal add_dec(s21_decimal one, s21_decimal two);
s21_decimal sub_dec(s21_decimal one, s21_decimal two);
s21_decimal div_dec(s21_decimal one, s21_decimal two);
s21_decimal mod_dec(s21_decimal one, s21_decimal two);
s21_decimal set_scale_to_dec(s21_decimal number, int scale);
int is_greater(s21_decimal one, s21_decimal two);
int is_greater_or_equal(s21_decimal one, s21_decimal two);
int empty_dec(s21_decimal number);
void scale_for_add(s21_decimal *one, s21_decimal *two, s21_decimal *res);
void scale_for_mul(s21_decimal *one, s21_decimal *two, s21_decimal *res);
void scale_for_div(s21_decimal *one, s21_decimal *two);
void scale_for_sub(s21_decimal *one, s21_decimal *two, s21_decimal *res);

s21_decimal s21_add(s21_decimal one, s21_decimal two);
s21_decimal s21_mul(s21_decimal one, s21_decimal two);
s21_decimal s21_sub(s21_decimal one, s21_decimal two);
s21_decimal s21_div(s21_decimal one, s21_decimal two);
int s21_is_equal(s21_decimal one, s21_decimal two);
int s21_is_greater(s21_decimal one, s21_decimal two);
int s21_is_greater_or_equal(s21_decimal one, s21_decimal two);
int s21_is_less(s21_decimal one, s21_decimal two);
int s21_is_less_or_equal(s21_decimal one, s21_decimal two);
s21_decimal s21_truncate(s21_decimal number);
s21_decimal s21_round(s21_decimal number);
s21_decimal s21_floor(s21_decimal number);
s21_decimal s21_mod(s21_decimal one, s21_decimal two);
s21_decimal s21_negate(s21_decimal number);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

int main() {
  s21_decimal one;
  s21_decimal two;
  s21_decimal result;

  /* Скейлы для примеров */
  int sc1 = 0b00000000000000010000000000000000;
  int sc2 = 0b00000000000000010000000000000000;
  //  Биты под скейл  ^^^^^^^^

  init_dec(&one);
  init_dec(&two);

  float src = -31.23;
  float res = 0;
  int intus = -1234;
  int resus = 0;

  s21_from_int_to_decimal(intus, &two);
  output(two);
  s21_from_decimal_to_int(two, &resus);
  printf("%d\n", resus);
  s21_from_float_to_decimal(src, &one);
  output(one);
  s21_from_decimal_to_float(one, &res);
  printf("%f\n", res);

  return 0;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int scale = get_scale(src), pow_dec = my_pow(10, scale);
  int res = 0;

  s21_from_decimal_to_int(src, &res);
  *dst = (float)res / pow_dec;

  return 0;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int sign = 0, res = 0, scale = 0;
  if (src < 0) {
    sign = 1;
    src *= -1;
  }
  float cln = src;

  while (cln) {
    while (cln > 0) cln -= 1;
    if (cln) cln = src;
    src *= 10;
    scale++;
  }
  res = (int)src;
  while (res % 10 == 0) {
    res /= 10;
    scale--;
  }

  s21_from_int_to_decimal(res, dst);
  set_scale(dst, scale);
  if (sign) set_sign(dst, 1);

  return 0;
}

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

long long my_pow(int x, int y) {
  long long z = 1;
  while (y) {
    z = z * x;
    y--;
  }
  return z;
}

s21_decimal s21_negate(s21_decimal number) {
  swap_bit(&number, 127);
  if (number.value_type == s21_INFINITY) {
    number.value_type = s21_NEGATIVE_INFINITY;
  } else {
    number.value_type = s21_INFINITY;
  }
  return number;
}

s21_decimal s21_truncate(s21_decimal number) {
  int sign = get_sign(number), scale = get_scale(number);
  s21_decimal res = number, scale_dec = set_scale_to_dec(scale_dec, scale);

  number = mod_dec(number, scale_dec);
  res = sub_dec(res, number);
  res = div_dec(res, scale_dec);

  set_sign(&res, sign);
  return res;
}

s21_decimal s21_mod(s21_decimal one, s21_decimal two) {
  int sign_1 = get_sign(one), sign_2 = get_sign(two);
  int scale_1 = get_scale(one), scale_2 = get_scale(two);
  set_scale(&one, scale_1);
  set_scale(&two, scale_2);
  s21_decimal res;
  init_dec(&res);

  scale_for_div(&one, &two);

  res = mod_dec(one, two);
  if (empty_dec(res)) {
    if (scale_1 > scale_2) {
      set_scale(&res, scale_1);
    } else {
      set_scale(&res, scale_2);
    }
  }

  if (sign_1 && sign_2)
    set_sign(&res, 1);
  else if (!sign_1 && sign_2)
    set_sign(&res, 1);
  return res;
}

s21_decimal mod_dec(s21_decimal one, s21_decimal two) {
  while (!is_greater_or_equal(one, two)) {
    one = sub_dec(one, two);
  }
  return one;
}

s21_decimal s21_floor(s21_decimal number) {
  int sign = get_sign(number), scale = get_scale(number);
  s21_decimal res = number, scale_dec = set_scale_to_dec(scale_dec, scale);

  number = mod_dec(number, scale_dec);
  res = sub_dec(res, number);
  res = div_dec(res, scale_dec);

  if (sign && number.bits[L]) {
    res.bits[L] += 1;
  }

  set_sign(&res, sign);
  return res;
}

s21_decimal s21_round(s21_decimal number) {
  int sign = get_sign(number), scale = get_scale(number);
  s21_decimal res = number, scale_dec = set_scale_to_dec(scale_dec, scale);

  number = mod_dec(number, scale_dec);
  res = sub_dec(res, number);
  res = div_dec(res, scale_dec);

  scale -= 1;
  set_scale(&number, scale);
  number = s21_truncate(number);

  if (number.bits[L] >= 5) {
    res.bits[L] += 1;
  }

  set_sign(&res, sign);
  return res;
}

s21_decimal div_dec(s21_decimal one, s21_decimal two) {
  s21_decimal res, one_plus;
  init_dec(&one_plus);
  init_dec(&res);
  one_plus.bits[L] = 1;

  while (empty_dec(one)) {
    one = sub_dec(one, two);
    res = add_dec(res, one_plus);
  }

  return res;
}

void scale_for_div(s21_decimal *one, s21_decimal *two) {
  int scl_1 = get_scale(*one), scl_2 = get_scale(*two);
  int scale = (scl_1 > scl_2) ? (scl_1 - scl_2) : (scl_2 - scl_1);
  s21_decimal scale_dec = set_scale_to_dec(scale_dec, scale);

  if (scl_1 > scl_2) {
    *two = mul_dec(*two, scale_dec);
  } else {
    *one = mul_dec(*one, scale_dec);
  }
}

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

int empty_dec(s21_decimal number) {
  int flag = 0;
  if (number.bits[H]) flag = 1;
  if (number.bits[M]) flag = 1;
  if (number.bits[L]) flag = 1;
  return flag;
}

void great_shift(s21_decimal *number) {
  if (get_bit_ptr(number, 31) && get_bit_ptr(number, 63)) {
    shift_one_bit(number);
    set_bit(number, 32);
    set_bit(number, 64);
  } else if (get_bit_ptr(number, 31) && !get_bit_ptr(number, 63)) {
    shift_one_bit(number);
    set_bit(number, 32);
  } else if (!get_bit_ptr(number, 31) && get_bit_ptr(number, 63)) {
    shift_one_bit(number);
    set_bit(number, 64);
  } else {
    shift_one_bit(number);
  }
}

int which_bits(s21_decimal *one, s21_decimal *two) {
  int flag = 2;
  if (one->bits[H] == two->bits[H]) {
    flag = 1;
    if (one->bits[M] == two->bits[M]) {
      flag = 0;
      if (one->bits[L] == two->bits[L]) {
        flag = 3;
      }
    }
  }
  return flag;
}

int is_greater(s21_decimal one, s21_decimal two) {
  int flag = 0, num_bits;

  num_bits = which_bits(&one, &two);

  if (one.bits[num_bits] > two.bits[num_bits]) {
    flag = 0;
  } else {
    flag = 1;
  }
  return flag;
}

int is_greater_or_equal(s21_decimal one, s21_decimal two) {
  int flag = 0, num_bits;

  num_bits = which_bits(&one, &two);

  if (num_bits != 3) {
    if (one.bits[num_bits] > two.bits[num_bits]) {
      flag = 0;
    } else {
      flag = 1;
    }
  }
  return flag;
}

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

int s21_is_equal(s21_decimal one, s21_decimal two) {
  int flag = 0;
  for (int i = 0; i < 128; i++) {
    if (get_bit(one, i) != get_bit(two, i)) flag = 1;
    if (flag == 1) break;
  }
  return flag;
}

int s21_is_greater_or_equal(s21_decimal one, s21_decimal two) {
  int flag = 0, scl_1 = get_scale(one), scl_2 = get_scale(two);
  int scale = (scl_1 > scl_2) ? (scl_1 - scl_2) : (scl_2 - scl_1);
  int num_bits, sign_one = get_sign(one), sign_two = get_sign(two);
  s21_decimal scale_dec = set_scale_to_dec(scale_dec, scale);

  if (!s21_is_equal(one, two)) {
    flag = 0;
  } else {
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
    } else if (sign_one == 1 && sign_two == 1) {
      if (one.bits[num_bits] > two.bits[num_bits]) flag = 1;
    } else if (one.bits[num_bits] > two.bits[num_bits]) {
      flag = 0;
    } else {
      flag = 1;
    }
  }
  return flag;
}

int s21_is_less_or_equal(s21_decimal one, s21_decimal two) {
  int flag = 0, scl_1 = get_scale(one), scl_2 = get_scale(two);
  int scale = (scl_1 > scl_2) ? (scl_1 - scl_2) : (scl_2 - scl_1);
  int num_bits, sign_one = get_sign(one), sign_two = get_sign(two);
  s21_decimal scale_dec = set_scale_to_dec(scale_dec, scale);

  if (!s21_is_equal(one, two)) {
    flag = 0;
  } else {
    if (scl_1 > scl_2) {
      two = mul_dec(two, scale_dec);
    } else {
      one = mul_dec(one, scale_dec);
    }
    num_bits = which_bits(&one, &two);

    if (sign_one < sign_two) {
      flag = 1;
    } else if (sign_one > sign_two) {
      flag = 0;
    } else if (sign_one == 1 && sign_two == 1) {
      if (one.bits[num_bits] > two.bits[num_bits]) flag = 0;
    } else if (one.bits[num_bits] < two.bits[num_bits]) {
      flag = 0;
    } else {
      flag = 1;
    }
  }
  return flag;
}

int s21_is_less(s21_decimal one, s21_decimal two) {
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
    flag = 1;
  } else if (sign_one > sign_two) {
    flag = 0;
  } else if (sign_one == 1 && sign_two == 1) {
    if (one.bits[num_bits] > two.bits[num_bits]) flag = 0;
  } else if (one.bits[num_bits] < two.bits[num_bits]) {
    flag = 0;
  } else {
    flag = 1;
  }
  return flag;
}

s21_decimal set_scale_to_dec(s21_decimal number, int scale) {
  s21_decimal ten;
  init_dec(&ten);
  init_dec(&number);
  number.bits[L] = 1;
  ten.bits[L] = 10;
  while (scale) {
    number = mul_dec(number, ten);
    scale--;
  }
  return number;
}

void shift_one_bit(s21_decimal *number) {
  number->bits[H] <<= 1;
  number->bits[M] <<= 1;
  number->bits[L] <<= 1;
}

void init_dec(s21_decimal *number) {
  number->bits[L] = 0;
  number->bits[M] = 0;
  number->bits[H] = 0;
  number->bits[S] = 0;
  number->value_type = s21_NORMAL_VALUE;
}

void scale_for_mul(s21_decimal *one, s21_decimal *two, s21_decimal *res) {
  int scl_1 = get_scale(*one), scl_2 = get_scale(*two);
  res->bits[S] = scl_1 + scl_2;
  set_scale(res, res->bits[S]);
}

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

s21_decimal mul_dec(s21_decimal one, s21_decimal two) {
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
  return res;
}

s21_decimal add_dec(s21_decimal one, s21_decimal two) {
  int v_ume = 0;
  s21_decimal res;
  init_dec(&res);

  for (int i = 0; i < 96; i++) {
    if (get_bit(one, i) && get_bit(two, i)) {
      if (v_ume == 0) {
        zero_bit(&res, i);
      } else {
        set_bit(&res, i);
      }
      v_ume = 1;
    } else if (get_bit(one, i) || get_bit(two, i)) {
      if (v_ume == 1) {
        zero_bit(&res, i);
      } else {
        set_bit(&res, i);
      }
    } else {
      if (v_ume == 1) {
        set_bit(&res, i);
        v_ume = 0;
      } else {
        zero_bit(&res, i);
      }
    }
  }
  return res;
}

void scale_for_sub(s21_decimal *one, s21_decimal *two, s21_decimal *res) {
  int scl_1 = get_scale(*one), scl_2 = get_scale(*two);
  int scale = (scl_1 > scl_2) ? (scl_1 - scl_2) : (scl_2 - scl_1);
  s21_decimal scale_dec = set_scale_to_dec(scale_dec, scale);

  if (scl_1 > scl_2) {
    *two = mul_dec(*two, scale_dec);
    res->bits[S] = scl_1;
    set_scale(res, scl_1);
  } else {
    *one = mul_dec(*one, scale_dec);
    res->bits[S] = scl_2;
    set_scale(res, scl_2);
  }
}

s21_decimal s21_sub(s21_decimal one, s21_decimal two) {
  int flag = 0, scale_res, sign_1 = get_sign(one), sign_2 = get_sign(two);
  s21_decimal res;
  init_dec(&res);

  scale_for_sub(&one, &two, &res);
  scale_res = get_scale(res);
  set_sign(&one, sign_1);
  set_sign(&two, sign_2);

  if (s21_is_greater(one, two)) {
    if (sign_1 && !sign_2) {    // -2 и 1
      res = add_dec(one, two);  // 2 + 1
      flag = 1;
    } else if (sign_1 && sign_2) {  // -2 и -1
      res = sub_dec(one, two);      // 2 - 1
      flag = 1;
    } else {
      res = sub_dec(two, one);  // 1 и 2 то 2 - 1
      flag = 1;
    }
  } else {
    if (!sign_1 && sign_2) {        // 2 и -1
      res = add_dec(one, two);      // 2 + 1 (2 - (-1))
    } else if (sign_1 && sign_2) {  // -1 и -2
      res = sub_dec(two, one);      // 2 - 1 = 1
    } else {
      res = sub_dec(one, two);  // 2 - 1 = 1
    }
  }
  set_scale(&res, scale_res);
  if (flag) set_sign(&res, 1);
  return res;
}

s21_decimal sub_dec(s21_decimal one, s21_decimal two) {
  s21_decimal res;
  init_dec(&res);
  int v_ume = 0;
  for (int i = 0; i < 96; i++) {
    if (get_bit(one, i) && get_bit(two, i)) {
      if (v_ume == 1) zero_bit(&one, i);
    } else if (get_bit(one, i) && !get_bit(two, i)) {
      if (v_ume == 1) {
        zero_bit(&one, i);
        v_ume = 0;
      }
    } else if (!get_bit(one, i) && get_bit(two, i)) {
      if (v_ume == 1) set_bit(&one, i);
      v_ume = 1;
    } else if (!get_bit(one, i) && !get_bit(two, i)) {
      if (v_ume == 1) set_bit(&one, i);
    }
  }
  for (int i = 0; i < 96; i++) {
    if (get_bit(one, i) && get_bit(two, i)) {
      zero_bit(&res, i);
    } else if (get_bit(one, i) && !get_bit(two, i)) {
      set_bit(&res, i);
    } else if (!get_bit(one, i) && get_bit(two, i)) {
      set_bit(&res, i);
    } else if (!get_bit(one, i) && !get_bit(two, i)) {
      zero_bit(&res, i);
    }
  }
  return res;
}

void scale_for_add(s21_decimal *one, s21_decimal *two, s21_decimal *res) {
  int scl_1 = get_scale(*one), scl_2 = get_scale(*two);
  int scale = (scl_1 > scl_2) ? (scl_1 - scl_2) : (scl_2 - scl_1);
  s21_decimal scale_dec = set_scale_to_dec(scale_dec, scale);

  if (scl_1 > scl_2) {
    *two = mul_dec(*two, scale_dec);
    res->bits[S] = scl_1;
    set_scale(res, scl_1);
  } else {
    *one = mul_dec(*one, scale_dec);
    res->bits[S] = scl_2;
    set_scale(res, scl_2);
  }
}

s21_decimal s21_add(s21_decimal one, s21_decimal two) {
  int flag = 0, scale_res, sign_1 = get_sign(one), sign_2 = get_sign(two);
  s21_decimal res;
  init_dec(&res);

  scale_for_add(&one, &two, &res);
  scale_res = get_scale(res);
  set_sign(&one, sign_1);
  set_sign(&two, sign_2);

  if (s21_is_greater(one, two)) {
    if (sign_1 && sign_2) {
      res = add_dec(one, two);
      flag = 1;
    } else if (sign_1 && !sign_2) {
      if (is_greater(one, two)) {
        res = sub_dec(two, one);
      } else {
        res = sub_dec(one, two);
        flag = 1;
      }
    } else {
      res = add_dec(one, two);
    }
  } else {
    if (sign_1 && sign_2) {
      res = add_dec(one, two);
      flag = 1;
    } else if (!sign_1 && sign_2) {
      if (is_greater(one, two)) {
        res = sub_dec(two, one);
        flag = 1;
      } else {
        res = sub_dec(one, two);
      }
    } else {
      res = add_dec(one, two);
    }
  }

  set_scale(&res, scale_res);
  if (flag) set_sign(&res, 1);
  return res;
}

int get_sign(s21_decimal number) { return get_bit(number, 127); }

void set_sign(s21_decimal *number, int sign) {
  if (sign == 0) {
    zero_bit(number, 127);
  } else {
    set_bit(number, 127);
  }
}

int get_scale(s21_decimal number) {
  return ((number.bits[S] & SCALE_MASK) >> 16);
}

void set_scale(s21_decimal *number, int scale) {
  if (scale <= 28) {
    number->bits[S] = scale << 16;
  }
}

int get_bit(s21_decimal number, const int n) {
  return (number.bits[n / 32] & (1 << (n % 32))) != 0;
}

int get_bit_ptr(s21_decimal *number, const int n) {
  return (number->bits[n / 32] & (1 << (n % 32))) != 0;
}

void zero_bit(s21_decimal *number, const int n) {
  number->bits[n / 32] &= ~(1 << (n % 32));
}

void swap_bit(s21_decimal *number, const int n) {
  number->bits[n / 32] ^= (1 << (n % 32));
}

void set_bit(s21_decimal *number, const int n) {
  number->bits[n / 32] |= (1 << (n % 32));
}

int get_bit_int(const int number, const int n) {
  return (number & (1 << n)) != 0;
}

int dec2bin(int num) {
  unsigned int binary = 0b00000000000000000000000000000000;
  unsigned int mask = 0b00000000000000000000000000000001;
  int score = 1;   // только для вывода
  char buf[32];    // только для вывода
  buf[32] = '\0';  // только для вывода
  for (int i = 0; i <= 31; i++) {
    if (num % 2 != 0) {
      binary = binary + mask;
      num -= 1;
      num /= 2;
      mask <<= 1u;
      buf[i] = '1';  // только для вывода
      score++;       // только для вывода
    } else {
      mask <<= 1u;
      num /= 2;
      buf[i] = '0';  // только для вывода
      score++;       // только для вывода
    }
  }
  // только для вывода
  for (int j = 31; j >= 0; j--) {
    if (j == 27) printf(" ");
    if (j == 23) printf(" ");
    if (j == 19) printf(" ");
    if (j == 15) printf(" ");
    if (j == 11) printf(" ");
    if (j == 7) printf(" ");
    if (j == 3) printf(" ");
    printf("%c", buf[j]);
  }
  printf(" ");
  // только для вывода
  return binary;
}

void output(s21_decimal result) {
  printf("\n");
  dec2bin(result.bits[S]);
  printf("\n");
  dec2bin(result.bits[H]);
  printf("    ЧИСЛО: %u", result.bits[L]);
  printf("\n");
  dec2bin(result.bits[M]);
  printf("    СКЕЙЛ: %d", get_scale(result));
  printf("\n");
  dec2bin(result.bits[L]);
  printf("    ЗНАКС: %d", get_bit(result, 127));
  printf("\n\n");
}

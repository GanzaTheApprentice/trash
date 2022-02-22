#include "s21_decimal.h"

long long my_pow(int x, int y) {
  long long z = 1;
  while (y) {
    z = z * x;
    y--;
  }
  return z;
}

s21_decimal mod_dec(s21_decimal one, s21_decimal two) {
  while (!is_greater_or_equal(one, two)) {
    one = sub_dec(one, two);
  }
  return one;
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

void scale_for_mul(s21_decimal *one, s21_decimal *two, s21_decimal *res) {
  int scl_1 = get_scale(*one), scl_2 = get_scale(*two);
  res->bits[S] = scl_1 + scl_2;
  set_scale(res, res->bits[S]);
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

void shift_one_bit(s21_decimal *number) {
  number->bits[H] <<= 1;
  number->bits[M] <<= 1;
  number->bits[L] <<= 1;
}

int empty_dec(s21_decimal number) {
  int flag = 0;
  if (number.bits[H]) flag = 1;
  if (number.bits[M]) flag = 1;
  if (number.bits[L]) flag = 1;
  return flag;
}

void init_dec(s21_decimal *number) {
  number->bits[L] = 0;
  number->bits[M] = 0;
  number->bits[H] = 0;
  number->bits[S] = 0;
  number->value_type = s21_NORMAL_VALUE;
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

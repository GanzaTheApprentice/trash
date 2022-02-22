#include <stdio.h>
#include <stdlib.h>

long long my_pow(int x, int y);
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

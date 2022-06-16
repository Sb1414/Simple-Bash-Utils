#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <regex.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Grep {
    int i;
    int v;
    int c;
    int n;
    int l;
    int h;
    int s;
    int o;
};

void s21_init(struct Grep *flag);
void s21_select(struct Grep grep_set, regex_t *regex_arr, int len, int argc, char **argv, int curr_pos);
regex_t *s21_merger(regex_t *reg1, regex_t *reg2, int e_n, int f_n);
regex_t *s21_parse_2(int argc, char *argv[], int *e_n, int *err, struct Grep  grep_set);
regex_t *s21_parse_f_flag(int argc, char *argv[], int *f_n, int *err, struct Grep  grep_set);
regex_t *s21_part_4(int *curr_pos, int argc,
    char *argv[], regex_t *regex_arr, int *len, struct Grep  grep_set);
regex_t *s21_f_flag(char **file_names, int len, int *f_n, int *error, struct Grep  grep_set);
void s21_parser(int n, char *argv[], int *err, struct Grep * set);
char *s21_string_filter(char *str);

#endif  // SRC_GREP_S21_GREP_H_

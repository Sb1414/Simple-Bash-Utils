#include "s21_grep.h"

int main(int argc, char **argv) {
    struct Grep grep_option;
    s21_init(&grep_option);
    int err = (argc < 3);
    int e_n = 0, f_n = 0;
    int pos = 1;
    s21_parser(argc, argv, &err, &grep_option);
    regex_t *regex_arr = s21_parse_2(argc, argv, &e_n, &err, grep_option);
    regex_t *regex_from_file = s21_parse_f_flag(argc, argv, &f_n, &err, grep_option);
    int len = e_n + f_n;
    regex_arr = s21_merger(regex_arr, regex_from_file, e_n, f_n);
    if (!e_n && !f_n) {
        regex_arr = s21_part_4(&pos, argc, argv, regex_arr, &len, grep_option);
    }
    if (!err) {
        s21_select(grep_option, regex_arr, len, argc, argv, pos);
    }
    if (regex_arr != NULL) {
        free(regex_arr);
    }
    return 0;
}

void s21_init(struct Grep *flag) {
    flag->i = 0;
    flag->v = 0;
    flag->n = 0;
    flag->c = 0;
    flag->l = 0;
    flag->s = 0;
    flag->o = 0;
    flag->h = 0;
}

void s21_parser(int n, char *argv[], int *err, struct Grep * opt) {
    for (int i = 1; i < n && !(*err); i++) {
        if (*(argv[i]) == '-' && strcmp(argv[i], "-e") && strcmp(argv[i], "-f")) {
            if (strlen(argv[i]) == 1) {
                break;
            }
            for (size_t k = 1; k < strlen(argv[i]) && i != -1; k++) {
                switch (*(argv[i] + k)) {
                    case 'i':
                        opt->i = 1;
                        break;
                    case 'v':
                        opt->v = 1;
                        break;
                    case 'c':
                        opt->c = 1;
                        break;
                    case 'l':
                        opt->l = 1;
                        break;
                    case 'n':
                        opt->n = 1;
                        break;
                    case 'h':
                        opt->h = 1;
                        break;
                    case 's':
                        opt->s = 1;
                        break;
                    case 'o':
                        opt->o = 1;
                        break;
                    default:
                        printf("s21_grep: illegal set -- %s\n", (argv[i] + 1));
                        printf("usage: ./s21_grep [-eivclnhsfo] [file ...]");
                        *err = 1;
                        break;
                }
            }
        }
    }
}

regex_t *s21_part_4(int *curr_pos, int argc,
    char *argv[], regex_t *regex_arr, int *len, struct Grep  grep_option) {
    regex_t *ret_reg = regex_arr;
    while (*curr_pos < argc - 1) {
        if (argv[*curr_pos][0] != '-') {
            char *tmp_str = malloc((strlen(argv[*curr_pos]) + 1) * sizeof(char));
            strcpy(tmp_str, argv[*curr_pos]);
            tmp_str = s21_string_filter(tmp_str);
            (*len)++;
            if (*len == 1) {
                ret_reg = malloc((*len) * sizeof(regex_t));
            } else {
                ret_reg = realloc(ret_reg, *len * sizeof(regex_t));
            }
            if (grep_option.i) {
                regcomp(ret_reg + (*len - 1), tmp_str, REG_ICASE);
            } else {
                regcomp(ret_reg + (*len - 1), tmp_str, 0);
            }
            free(tmp_str);
            *curr_pos = *curr_pos + 1;
            break;
        }
        *curr_pos = *curr_pos + 1;
    }
    return ret_reg;
}

void s21_select(struct Grep grep_option, regex_t *regex_arr, int len, int argc, char **argv, int curr_pos) {
    int mul_files = 0;
    for (int i = curr_pos; i < argc; i++) {
        if (strchr(argv[i], '-') != NULL) {
            if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "-e")) {
                i++;
            }
            continue;
        }
        mul_files++;
    }
    for (int i = curr_pos; i < argc; i++) {
        if (strchr(argv[i], '-') != NULL) {
            if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "-e")) {
                i++;
            }
            continue;
        }
        FILE *f = fopen(argv[i], "r");
        if (f != NULL) {
            size_t nmatch = 2;
            regmatch_t pmatch[2];
            char c;
            int line_len = 0;
            int line_num = 0;
            int count_m = 0;
            char *line = malloc(1);
            int on = 1;
            int matched = 0;
            while (on) {
                int scanning = fscanf(f, "%c", &c);
                if (c == '\n') {
                    c = '\0';
                } else if (scanning == -1) {
                    on = 0;
                    c = '\0';
                }
                line_len++;
                char *tmp = realloc(line, line_len * sizeof(char));
                if (tmp != NULL) {
                    line = tmp;
                    *(line + line_len - 1) = c;
                }
                if ((on && c == '\0') || (!on && *line != '\0')) {
                    line_num++;
                    matched = 0;
                    for (int k = 0; k < len; k++) {
                        int ret = regexec(regex_arr + k, line, nmatch, pmatch, 0);
                        if (!ret) {
                            matched = 1;
                            break;
                        }
                    }
                    if (grep_option.v) {
                        matched = matched ? 0 : 1;
                    }
                    if (matched) {
                        count_m++;
                        if (grep_option.l) {
                            break;
                        }
                        if (!grep_option.c) {
                            if (mul_files > 1 && !grep_option.h) {
                                printf("%s:", argv[i]);
                            }
                            if (grep_option.n) {
                                printf("%d:", line_num);
                            }
                            if (grep_option.o && !grep_option.v) {
                                int appender = pmatch[0].rm_eo;
                                printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
                                    &line[pmatch[0].rm_so]);
                                for (int k = 0; k < len; k++) {
                                    while (!regexec(regex_arr + k, line + appender,
                                        nmatch, pmatch, REG_NOTBOL) && pmatch[0].rm_eo != pmatch[0].rm_so) {
                                        printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
                                            &line[appender + pmatch[0].rm_so]);
                                        appender += pmatch[0].rm_eo;
                                    }
                                }
                            } else {
                                printf("%s\n", line);
                            }
                        }
                    }
                    free(line);
                    line_len = 0;
                    line = malloc(0);
                }
            }
            if (grep_option.c) {
                if (mul_files > 1 && !grep_option.h) {
                    printf("%s:", argv[i]);
                }
                printf("%d\n", count_m);
            }
            if (grep_option.l && matched) {
                printf("%s\n", argv[i]);
            }
            free(line);
        } else {
            printf("s21_grep:No such file or directory:%s\n", argv[i]);
        }
    }
    for (int k = 0; k < len; k++) {
        regfree(regex_arr + k);
    }
}

regex_t *s21_merger(regex_t *reg1, regex_t *reg2, int e_n, int f_n) {
    if (reg1 == NULL) {
        reg1 = malloc(0);
    }
    reg1 = realloc(reg1, (e_n + f_n) * sizeof(regex_t));
    for (int i = 0; i < f_n; i++) {
        reg1[e_n + i] = reg2[i];
    }
    if (reg2 != NULL) {
        free(reg2);
    }
    return reg1;
}

char *s21_string_filter(char *str) {
    char *ret = NULL;
    if (str != NULL) {
        int n = strlen(str);
        if (*str == '\'' && *(str + n - 1) == '\'' && n > 1) {
            ret = malloc(n - 1);
            strncpy(ret, str + 1, n - 2);
            free(str);
        } else if (*str == '\"' && *(str + n - 1) == '\"' && n > 1) {
            ret = malloc(n - 1);
            strncpy(ret, str + 1, n - 2);
            free(str);
        } else {
            ret = str;
        }
    }
    return ret;
}

regex_t *s21_parse_2(int argc, char *argv[], int *e_n, int *err, struct Grep  grep_option) {
    int count = 0;
    regex_t *regex_arr = NULL;
    int i = 1;
    while (i < argc && !(*err)) {
        if (!strcmp(argv[i], "-e")) {
            count++;
            i++;
            if (regex_arr == NULL) {
                regex_arr = malloc(0);
            }
            regex_t *tmp = realloc(regex_arr, count * sizeof(regex_t));
            if (tmp != NULL) {
                regex_arr = tmp;
            }
            char *tmp_str = malloc((strlen(argv[i]) + 1) * sizeof(char));
            strcpy(tmp_str, argv[i]);
            tmp_str = s21_string_filter(tmp_str);
            if (grep_option.i) {
                regcomp(regex_arr + (count - 1), tmp_str, REG_ICASE);
            } else {
                regcomp(regex_arr + (count - 1), tmp_str, 0);
            }
            free(tmp_str);
        }
        i++;
    }
    *e_n = count;
    return regex_arr;
}

regex_t *s21_parse_f_flag(int argc, char *argv[], int *f_n, int *err, struct Grep  grep_option) {
    int i = 1;
    int count = 0;
    char **file = NULL;
    while (i < argc && !(*err)) {
        if (!strcmp(argv[i], "-f") && i != argc - 1) {
            i++;
            count++;
            if (file == NULL) {
                file = malloc(count * sizeof(char*));
            } else {
                char **tmp = realloc(file, count * sizeof(char*));
                if (tmp != NULL) {
                    file = tmp;
                }
            }
            *(file + count - 1) = malloc((strlen(argv[i]) + 1) * sizeof(char));
            strcpy(*(file + count - 1), argv[i]);
        }
        i++;
    }
    int file_len = count;
    regex_t *ret = s21_f_flag(file, file_len, f_n, err, grep_option);
    for (int i = 0; i < count; i++) {
        free(*(file + i));
    }
    if (file != NULL) {
        free(file);
    }
    return ret;
}

regex_t *s21_f_flag(char **file, int len, int *f_n, int *error, struct Grep  grep_option) {
    regex_t *regex_arr = NULL;
    for (int i = 0; i < len && !(*error); i++) {
        FILE *f = fopen(file[i], "r");
        if (f == NULL && grep_option.s) {
            *error = 1;
            printf("No such file or directory:%s\n", file[i]);
        } else {
            char c;
            int line_len = 0;
            char *line = malloc(1);
            int on = 1;
            while (on) {
                c = fgetc(f);
                if (c == '\n') {
                    c = '\0';
                } else if (c == EOF) {
                    on = 0;
                    c = '\0';
                }
                line_len++;
                char *tmp = realloc(line, line_len * sizeof(char));
                if (tmp != NULL) {
                    line = tmp;
                    *(line + line_len - 1) = c;
                }
                if (c == '\0') {
                    line = s21_string_filter(line);
                    (*f_n)++;
                    if (regex_arr == NULL) {
                        regex_arr = malloc(0);
                    }
                    regex_t *tmp = realloc(regex_arr, (*f_n) * sizeof(regex_t));
                    if (tmp != NULL) {
                        regex_arr = tmp;
                    }
                    if (grep_option.i) {
                        regcomp(regex_arr + *f_n - 1, line, REG_ICASE);
                    } else {
                        regcomp(regex_arr + (*f_n) - 1, line, 0);
                    }
                    free(line);
                    line_len = 0;
                    line = malloc(0);
                }
            }
            free(line);
        }
        fclose(f);
    }
    return regex_arr;
}

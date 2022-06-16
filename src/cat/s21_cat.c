#include "s21_cat.h"

int main(int argc, char **argv) {
    int error = 0;
    struct Flags flags;
    Init(&flags);
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (ParserFlags(&flags, argv[i])) {
                error = 1;
                break;
            }
        }
    }
    if (!error) {
        int order = 1, shift = 1;
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] != '-')
                order = ProcessFile(&flags, argv[i], order, &shift);
        }
    }
    return 0;
}

void Init(struct Flags *flag) {
    flag->e = 0;
    flag->E = 0;
    flag->v = 0;
    flag->n = 0;
    flag->t = 0;
    flag->T = 0;
    flag->s = 0;
    flag->b = 0;
}

void FlagN(struct Flags *flags, int EmptyLine, int shift, int *order) {
    if (EmptyLine == 0 && flags->n == 1) {
        if (shift) {
            printf("%6d\t", *order);
            *order += 1;
        }
    }
}

void FlagE(struct Flags *flags, int *EmptyLine, int *shift, int *NumberEmptyLine) {
    if (flags->e || flags->E)
        printf("$");
    if (*EmptyLine == 0)
        *NumberEmptyLine += 1;
    printf("\n");
    *EmptyLine = 0;
    *shift = 1;
}

void FlagB(struct Flags *flags, int EmptyLine, int shift, int *order) {
    if (EmptyLine == 0 && flags->b == 1) {
        if (shift) {
            printf("%6d\t", *order);
            *order += 1;
        }
    }
}

void FlagTV(struct Flags *flags, char teamp) {
    if (teamp == '\t') {
        if (flags->t || flags->T) {
            printf("^I");
        } else {
            printf("%c", teamp);
        }
    } else if (((teamp < 32 && teamp >= 0) || teamp == 127) && flags->v) {
        printf("^%c", teamp + 64);
    } else {
        printf("%c", teamp);
    }
}

void PostProcessing(struct Flags *flags, char teamp, int *shift, int countFlags) {
    if (teamp != '\n') {
       *shift = 0;
        if ((flags->b || flags->n) && countFlags == 1) {
            printf("\n");
            *shift = 1;
        }
    } else {
        *shift = 1;
    }
}

int ProcessFile(struct Flags *flags, char *file, int order, int *shift) {
    if (flags->b)
        flags->n = 0;
    if (flags->t || flags->e)
        flags->v = 1;
    char teamp;
    FILE *fp;
    fp = fopen(file, "r");
    if (fp == NULL) {
        printf("cat: can't open '%s': No such file or directory\n", file);
    } else {
        int EmptyLine = 0, NumberEmptyLine = 0, countFlags = CountFlags(flags);
        while (fscanf(fp, "%c", &teamp) != -1) {
            // s
            if (teamp == '\n' && (EmptyLine == 0 && NumberEmptyLine != 0 && flags->s == 1))
                continue;
            FlagN(flags, EmptyLine, *shift, &order);
            if (teamp == '\n') {
            FlagE(flags, &EmptyLine, shift, &NumberEmptyLine);
            } else {
                NumberEmptyLine = 0;
                FlagB(flags, EmptyLine, *shift, &order);
                FlagTV(flags, teamp);
                EmptyLine++;
            }
        }
        PostProcessing(flags, teamp, shift, countFlags);
        fclose(fp);
    }
    return order;
}

int CountFlags(struct Flags *flags) {
    int count = 0;
    count = flags->e + flags->E + flags->v + flags->n + flags->t + flags->T + flags->s + flags->b;
    return count;
}

int IndicateFlag(struct Flags *flags, char c) {
    int error = 0;
    switch (c) {
        case 'e':
            flags->e = 1;
            break;
        case 'E':
            flags->E = 1;
            break;
        case 'n':
            flags->n = 1;
            break;
        case 't':
            flags->t = 1;
            break;
        case 'T':
            flags->T = 1;
            break;
        case 's':
            flags->s = 1;
            break;
        case 'b':
            flags->b = 1;
            break;
        case 'v':
            flags->v = 1;
            break;
        default:
            printf("cat: invalid option -- '%c'\n", c);
            printf("Try 'cat --help' for more information.\n");
            error = 1;
    }
    return error;
}

int ParserFlags(struct Flags *flags, char *str) {
    int error = 0;
    str++;
    while (*str != '\0') {
        if (IndicateFlag(flags, *str)) {
            error = 1;
            break;
        }
        str++;
    }
    return error;
}

#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Flags{
    int e;
    int E;
    int v;
    int n;
    int t;
    int T;
    int s;
    int b;
};

void Init(struct Flags *flag);
int ParserFlags(struct Flags *flags, char *str);
int ProcessFile(struct Flags *flags, char *file, int order, int *shift);
int CountFlags(struct Flags *flags);
void FlagN(struct Flags *flags, int EmptyLine, int shift, int *order);
void FlagE(struct Flags *flags, int *EmptyLine, int *shift, int *NumberEmptyLine);
void FlagB(struct Flags *flags, int EmptyLine, int shift, int *order);
void FlagTV(struct Flags *flags, char teamp);
void PostProcessing(struct Flags *flags, char teamp, int *shift, int countFlags);

#endif  // SRC_CAT_S21_CAT_H_

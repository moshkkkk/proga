#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "laba12.h"

#define N 10

int main() {
    srand(time(NULL));
    struct Student group[N];

    generate(group, N);
    printf("До сортировки:");
    print(group, N);

    sort(group, N);
    printf("\nПосле сортировки:");
    print(group, N);

    return 0;
}
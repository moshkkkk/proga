#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <math.h>
#include <stdint.h>
#include <cmocka.h>
#include <string.h>
#include <stdio.h>

int my_sqrt(int a) {
    if (a < 0) {
        return -1;
    }
    return (int)sqrt(a);
}

int sub(int a, int b) {
    return a - b;
}

int mul(int a, int b) {
    return a * b;
}

int divv(int a, int b) {
    if (b == 0) return 0;
    return a / b;
}

int max(int a, int b) {
    if (a > b) return a;
    return b;
}

int my_pow(int a, int b) {
    if (b < 0) {
        return -1;
    }
    return pow(a, b);
}

int min(int a, int b) {
    if (a < b) return a;
    return b;
}

int mod(int a, int b) {
    if (b == 0) {
        return -1;
    }
    return a % b;
}
static void test_mod(void **state){
    (void)state;
    assert_int_equal(mod(10,2),0);
    assert_int_equal(mod(10,0),-1);
}
static void test_sqrt(void **state){
    (void)state;
    assert_int_equal(my_sqrt(16),4);
    
}
static void test_pow(void **state){
    (void)state;
    assert_int_equal(my_pow(4,2),16);
}

int main(void) {
    printf("\n");
    printf("========================================\n");
    printf("   UNIT TESTING С ФРЕЙМВОРКОМ CMocka    \n");
    printf("========================================\n\n");

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_sqrt),
        cmocka_unit_test(test_pow),
        // cmocka_unit_test(test_mul),
        // cmocka_unit_test(test_div),
        // cmocka_unit_test(test_max),
        // cmocka_unit_test(test_min),
    };

    int result = cmocka_run_group_tests(tests, NULL, NULL);
    
    printf("\n");
    printf("========================================\n");
    if (result == 0) {
        printf("  РЕЗУЛЬТАТ: ВСЕ ТЕСТЫ ПРОЙДЕНЫ!\n");
    } else {
        printf("  РЕЗУЛЬТАТ: ЕСТЬ ОШИБКИ!\n");
    }
    printf("========================================\n");
    
    return result;
}
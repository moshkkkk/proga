#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void* sort_thread(void* arg) {
    int val = *(int*)arg;
    // чем больше число, тем дольше спит поток
    // умножаем на 10000 мкс чтобы было видно результат, но не ждать вечность
    usleep(val * 10000);
    printf("%d ", val);
    fflush(stdout); // сразу выводим в консоль
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Запуск: ./1-6 5 2 8 1 9\n");
        return 1;
    }

    int n = argc - 1;
    if (n > 50) {
        printf("Слишком много чисел! Максимум 50.\n");
        return 1;
    }

    pthread_t th[50];
    int nums[50];

    // Записываем числа из аргументов командной строки
    for (int i = 0; i < n; i++) {
        nums[i] = atoi(argv[i + 1]);
    }

    printf("Сортируем: ");
    for (int i = 0; i < n; i++) printf("%d ", nums[i]);
    printf("\nРезультат: ");

    // Запускаем по потоку на число
    for (int i = 0; i < n; i++) {
        pthread_create(&th[i], NULL, sort_thread, &nums[i]);
    }

    // Ждем пока все отработают
    for (int i = 0; i < n; i++) {
        pthread_join(th[i], NULL);
    }
    printf("\n");
    return 0;
}

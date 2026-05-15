#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Функция очистки, вызовется при отмене
void cleanup_func(void* arg) {
    int id = *(int*)arg;
    printf(">> Поток %d выполняет очистку перед выходом!\n", id);
}

void* worker(void* arg) {
    int id = *(int*)arg;
    pthread_cleanup_push(cleanup_func, arg); // регистрируем обработчик

    for (int i = 1; i <= 10; i++) {
        printf("Поток %d: шаг %d\n", id, i);
        sleep(1);
    }

    // 0 = вызвать только при отмене/исключении. При нормальном выходе не вызовет.
    pthread_cleanup_pop(0);
    return NULL;
}

int main() {
    pthread_t th[4];
    int ids[4] = {1, 2, 3, 4};

    for (int i = 0; i < 4; i++) {
        pthread_create(&th[i], NULL, worker, &ids[i]);
    }

    sleep(2);
    printf("main отменяет потоки...\n");
    for (int i = 0; i < 4; i++) {
        pthread_cancel(th[i]);
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(th[i], NULL);
    }
    return 0;
}

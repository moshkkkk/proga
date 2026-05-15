#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* worker(void* arg) {
    int id = *(int*)arg;
    for (int i = 1; i <= 10; i++) {
        printf("Поток %d: итерация %d\n", id, i);
        sleep(1); // сон между выводами
    }
    return NULL;
}

int main() {
    pthread_t th[4];
    int ids[4] = {1, 2, 3, 4};

    for (int i = 0; i < 4; i++) {
        pthread_create(&th[i], NULL, worker, &ids[i]);
    }

    printf("main ждет 2 секунды...\n");
    sleep(2);

    printf("main отменяет все потоки!\n");
    for (int i = 0; i < 4; i++) {
        pthread_cancel(th[i]);
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(th[i], NULL);
    }
    printf("Все потоки остановлены.\n");
    return 0;
}

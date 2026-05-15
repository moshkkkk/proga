#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Функция для дочернего потока
void* child_func(void* arg) {
    for (int i = 1; i <= 5; i++) {
        printf("[Дочерний] строка %d\n", i);
        usleep(100000); // небольшая пауза чтобы вывод не слипался
    }
    return NULL;
}

int main() {
    pthread_t th;
    pthread_create(&th, NULL, child_func, NULL);

    // Родитель тоже выводит 5 строк
    for (int i = 1; i <= 5; i++) {
        printf("[Родитель] строка %d\n", i);
        usleep(150000);
    }

    pthread_join(th, NULL); // ждем поток чтобы программа не закрылась сразу
    return 0;
}

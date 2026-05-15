#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* child_func(void* arg) {
    for (int i = 1; i <= 5; i++) {
        printf("[Дочерний] строка %d\n", i);
        usleep(100000);
    }
    return NULL;
}

int main() {
    pthread_t th;
    pthread_create(&th, NULL, child_func, NULL);

    // Ждем пока дочерний закончит
    pthread_join(th, NULL);

    printf("\nДочерний закончил. Теперь родитель:\n");
    for (int i = 1; i <= 5; i++) {
        printf("[Родитель] строка %d\n", i);
        usleep(100000);
    }
    return 0;
}

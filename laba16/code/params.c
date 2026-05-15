#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Массивы строк для каждого потока 
char* lines[4][3] = {
    {"Alpha-1", "Alpha-2", "Alpha-3"},
    {"Beta-1", "Beta-2", "Beta-3"},
    {"Gamma-1", "Gamma-2", "Gamma-3"},
    {"Delta-1", "Delta-2", "Delta-3"}
};

void* worker(void* arg) {
    int id = *(int*)arg; // получаем номер потока
    printf("Поток номер %d выводит:\n", id);
    for (int i = 0; i < 3; i++) {
        printf("  -> %s\n", lines[id-1][i]);
        usleep(50000);
    }
    return NULL;
}

int main() {
    pthread_t th[4];
    int ids[4] = {1, 2, 3, 4};

    // Создаем 4 потока, передаем каждому свой id
    for (int i = 0; i < 4; i++) {
        pthread_create(&th[i], NULL, worker, &ids[i]);
    }

    // Ждем все потоки
    for (int i = 0; i < 4; i++) {
        pthread_join(th[i], NULL);
    }
    return 0;
}

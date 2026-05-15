# Отчёт по практическому заданию №16: Знакомство с POSIX потоками

**Студент:** Грехова Мария
**Группа:** ИКС-532  

## Цель работы
Изучить базовые механизмы работы с потоками в POSIX (pthread): создание, синхронизацию, передачу параметров, принудительное завершение и нестандартный алгоритм сортировки.

---

## 🧵 Задание 1. Создание потока
> **Условие:** Создать поток с помощью `pthread_create()`. Родитель и дочерний поток выводят по 5 строк.

### Первый код
```
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* child_func(void* arg) {
    for (int i = 1; i <= 5; i++) {
        printf("[Дочерний] строка %d\n", i);
        usleep(100000); // пауза 0.1 сек
    }
    return NULL;
}

int main() {
    pthread_t th;
    pthread_create(&th, NULL, child_func, NULL);

    for (int i = 1; i <= 5; i++) {
        printf("[Родитель] строка %d\n", i);
        usleep(150000); // пауза 0.15 сек
    }

```
## 🧵 Задание 2. Ожидание потока
> **Условие:** Модифицировать упр.1 так, что родительский поток выводит текст 
после завершения дочернего потока. Подсказка: `pthread_join()`

### Второй код
```
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
```

## 🧵 Задание 3. Параметры потока
> **Условие:** Модифицировать упр.1 так, что родительский поток выводит текст 
после завершения дочернего потока. Подсказка: `pthread_join()`
```
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
```

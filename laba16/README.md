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

## 🧵 Задание 4. Завершение нити без ожидания
> **Условие:** Добавить сон с помощью `sleep()` в функцию потоков между выводами 
строк. Спустя две секунды после создания дочерних потоков 
основной поток должен прервать работу всех дочерних потоков с 
помощью `pthread_cancel()`

```
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
```

## 🧵 Задание 5. Обработать завершение потока
> **Условие:** Модифицировать упр. 4 так, чтобы дочерний поток перед завершение 
распечатывал сообщение об этом. Использовать
`pthread_cleanup_push()`
```
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
```

## 🧵 Задание 6. Реализовать простой Sleepsort
> **Условие:** Реализовать прикольный алгоритм сортировки Sleepsort с 
асимптотикой O(N) (по времени).
```
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
```

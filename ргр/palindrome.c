#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // для добавлений значений true false
#include <locale.h>  // Для поддержки русского языка в консоли
#include <wchar.h>   // Для работы с русскими буквами (wchar_t)
#include <wctype.h>  // Для функций iswalpha (проверка букв) и towlower (регистр)

// Переменные для настроек программы (Требование на "5")
char log_filename[256] = "app.log"; // объявление массива из 256 одиночных символов, где хранится имя файла логов
int min_len = 3; // создание переменной целого типа (int) с именем min_len и начальным значением 3

// 1. СИСТЕМА ЛОГИРОВАНИЯ (Требование на "5")
void log_msg(const char* level, const char* text) {
    FILE* f = fopen(log_filename, "a");
    if (!f) return;
    fprintf(f, "[%s] %s\n", level, text);
    fclose(f);
}

// 2. ПАРСЕР КОНФИГУРАЦИИ KEY=VALUE (Требование на "5")
void load_config() {
    FILE* cfg = fopen("config.ini", "r");
    if (!cfg) {
        log_msg("WARNING", "Файл config.ini не найден. Используем настройки по умолчанию.");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), cfg)) {
        char* eq = strchr(line, '=');
        if (!eq) continue;
        
        *eq = '\0';
        char* key = line;
        char* value = eq + 1;
        value[strcspn(value, "\n\r")] = '\0'; // Удаляем перенос строки

        if (strcmp(key, "LOG_FILE") == 0) {
            strcpy(log_filename, value);
        } else if (strcmp(key, "MIN_LEN") == 0) {
            min_len = atoi(value);
        }
    }
    fclose(cfg);
    log_msg("INFO", "Конфигурация успешно загружена.");
}

// 3. ПРЕДВАРИТЕЛЬНАЯ ОБРАБОТКА (Требование на "Отлично" - удаление пробелов и знаков)
// Используем ДИНАМИЧЕСКУЮ ПАМЯТЬ (malloc)
wchar_t* clean_text(const wchar_t* src) {
    size_t len = wcslen(src);
    wchar_t* dst = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
    if (!dst) {
        log_msg("ERROR", "Не удалось выделить память для очистки строки.");
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; src[i] != L'\0'; i++) {
        if (iswalpha(src[i])) { // Если это буква (русская или английская)
            dst[j++] = towlower(src[i]); // Переводим в нижний регистр
        }
    }
    dst[j] = L'\0'; // Закрываем строку
    return dst;
}

// 4. АЛГОРИТМ МАНАКЕРА ДЛЯ ПОИСКА ПОДПАЛИНДРОМОВ (Требование на "Отлично")
void find_palindromes_manacher(const wchar_t* cleaned, const wchar_t* original) {
    int n = (int)wcslen(cleaned);
    if (n < min_len) return;

    // Шаг 1: Создаем строку с разделителями '#' (например, "шалаш" -> "#ш#а#л#а#ш#")
    int t_len = 2 * n + 1;
    wchar_t* t_str = (wchar_t*)malloc((t_len + 1) * sizeof(wchar_t));
    if (!t_str) return;
    
    int idx = 0;
    for (int i = 0; i < n; i++) {
        t_str[idx++] = L'#';
        t_str[idx++] = cleaned[i];
    }
    t_str[idx++] = L'#';
    t_str[idx] = L'\0';

    // Шаг 2: Выделяем ДИНАМИЧЕСКУЮ ПАМЯТЬ под массив радиусов палиндромов
    int* P = (int*)calloc(t_len, sizeof(int));
    if (!P) {
        free(t_str);
        return;
    }
    
    int center = 0, right = 0;

    // Шаг 3: Главный цикл алгоритма Манакера
    for (int i = 0; i < t_len; i++) {
        int mirror = 2 * center - i;
        if (i < right) {
            P[i] = (right - i < P[mirror]) ? right - i : P[mirror];
        }

        // Расширяем палиндром в обе стороны от центра i
        while (i - 1 - P[i] >= 0 && i + 1 + P[i] < t_len && t_str[i - 1 - P[i]] == t_str[i + 1 + P[i]]) {
            P[i]++;
        }

        // Если вышли за правую границу, сдвигаем центр
        if (i + P[i] > right) {
            center = i;
            right = i + P[i];
        }

        // Шаг 4: Если нашли палиндром нужной длины — выводим его
        if (P[i] >= min_len) {
            int start_idx = (i - P[i]) / 2;
            
            wprintf(L"В предложении: \"%ls\"\n", original);
            wprintf(L"-> Найдена подстрока-палиндром: \"");
            for (int j = 0; j < P[i]; j++) {
                putwchar(cleaned[start_idx + j]);
            }
            wprintf(L"\"\n\n");
        }
    }

    // Обязательно освобождаем память
    free(t_str);
    free(P);
}

// 5. ПРОСТЫЕ UNIT-ТЕСТЫ БЕЗ СЛОЖНЫХ БИБЛИОТЕК (Требование на "5")
void run_unit_tests() {
    // Тест 1: Проверка очистки текста
    wchar_t* res = clean_text(L"Мама, мыла раму!");
    if (res != NULL && wcscmp(res, L"мамамылараму") == 0) {
        log_msg("INFO", "UNIT-TEST 1 (Очистка текста): ПРОЙДЕН");
    } else {
        log_msg("ERROR", "UNIT-TEST 1 (Очистка текста): ПРОВАЛЕН");
    }
    free(res);
}

int main(int argc, char* argv[]) {
    // Настройка локали, чтобы программа понимала русский язык компьютера
    setlocale(LC_ALL, "");

    load_config(); // Загрузка конфига
    log_msg("INFO", "Программа успешно запущена.");
    
    run_unit_tests(); // Запуск встроенных тестов

    // ВАЛИДАЦИЯ ВХОДНЫХ ДАННЫХ (Требование на "4" - проверка аргументов)
    if (argc < 2) {
        log_msg("ERROR", "Ошибка: Пользователь не указал имя файла.");
        fprintf(stderr, "Ошибка: Не указано имя входного файла!\n");
        fprintf(stderr, "Использование: %s <имя_файла.txt>\n", argv[0]);
        return 1;
    }

    // ВАЛИДАЦИЯ ВХОДНЫХ ДАННЫХ (Требование на "4" - проверка открытия файла)
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        log_msg("ERROR", "Ошибка: Указанный файл не существует или недоступен.");
        fprintf(stderr, "Ошибка: Не удалось открыть файл \"%s\". Проверьте путь.\n", argv[1]);
        return 1;
    }

    char line[1024];
    wchar_t w_line[1024];
    int line_count = 0;

    wprintf(L"--- Результаты поиска подпалиндромов ---\n\n");

    // Читаем файл построчно (каждая строка — отдельное предложение/текст)
    while (fgets(line, sizeof(line), file)) {
        line_count++;
        line[strcspn(line, "\n\r")] = '\0'; // Удаляем символ переноса строки
        if (strlen(line) == 0) continue;

        // Превращаем обычную строку в широкую строку (wchar_t) для работы с русским языком
        mbstowcs(w_line, line, 1024);

        // Очищаем строку от знаков и пробелов (выделяется динамическая память)
        wchar_t* cleaned = clean_text(w_line);
        if (cleaned) {
            // Запускаем алгоритм Манакера
            find_palindromes_manacher(cleaned, w_line);
            free(cleaned); // Освобождаем динамическую память строки
        }
    }
    fclose(file);

    // ВАЛИДАЦИЯ ВХОДНЫХ ДАННЫХ (Требование на "4" - проверка на пустой файл)
    if (line_count == 0) {
        log_msg("WARNING", "Предупреждение: Анализируемый файл оказался пуст.");
        wprintf(L"Предупреждение: Файл пуст.\n");
    }

    log_msg("INFO", "Программа успешно завершила работу.");
    return 0;
}

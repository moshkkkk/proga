#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Employee {
    char name[50];    // имя сотрудника (строка)
    int id;           // табельный номер (целое число)
    char level[20];   // уровень доступа (строка)
};

void printTable(struct Employee emp[], int count) {
    printf("\n=============================================\n");
    printf("| %-20s | %-8s | %-10s |\n", "Имя", "ID", "Уровень");
    printf("=============================================\n");
    
    for (int i = 0; i < count; i++) {
        printf("| %-20s | %-8d | %-10s |\n", 
               emp[i].name, emp[i].id, emp[i].level);
    }
    printf("=============================================\n");
}

// Функция сохранения в бинарный файл
void saveToBinary(struct Employee emp[], int count) {
    FILE *file = fopen("database.dat", "wb"); // указатель на файл
    
    if (file == NULL) {
        printf("Ошибка: не могу создать файл database.dat\n");
        return;
    }
    
    int written = fwrite(emp, sizeof(struct Employee), count, file);
    
    if (written != count) {
        printf("Ошибка: не удалось записать все данные\n");
    } else {
        printf("Записано %d структур, каждая размером %d байт\n", 
               count, (int)sizeof(struct Employee));
    }
    
    fclose(file);  //сбрасывает и закрывает файл
}

// Функция поиска сотрудника по ID
void searchByID() {
    FILE *file = fopen("database.dat", "rb"); //чтение
    
    if (file == NULL) {
        printf("Ошибка: файл database.dat не найден\n");
        return;
    }
    
    fseek(file, 0, SEEK_END); // перемещает указатель 
    long fileSize = ftell(file);  
    rewind(file);  //перемещает в начало файла               
    
    int count = fileSize / sizeof(struct Employee);
    
    if (count == 0) {
        printf("Файл пуст\n");
        fclose(file);
        return;
    }
    
    // Выделяем память под массив сотрудников
    struct Employee *emp = (struct Employee*)malloc(fileSize);
    if (emp == NULL) {
        printf("Ошибка: не хватает памяти\n");
        fclose(file);
        return;
    }

    fread(emp, sizeof(struct Employee), count, file);
    fclose(file);

    int searchId;
    printf("\nВведите ID сотрудника для поиска: ");
    scanf("%d", &searchId);
 
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (emp[i].id == searchId) {
            printf("\nСотрудник найден!\n");
            printf("   Имя: %s\n", emp[i].name);
            printf("   ID: %d\n", emp[i].id);
            printf("   Уровень: %s\n", emp[i].level);
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("\nСотрудник с ID %d не найден\n", searchId);
    }
    
    free(emp);  // освобождаем память
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Использование: %s staff.csv\n", argv[0]);
        return 1;
    }
    
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Ошибка: не могу открыть файл %s\n", argv[1]);
        return 1;
    }
    
    // Массив для хранения сотрудников (
    struct Employee employees[100];
    int count = 0;           
    char line[200];          
    
    while (fgets(line, sizeof(line), file) != NULL && count < 100) {
        line[strcspn(line, "\n")] = '\0';
      
        char *token = strtok(line, ";");
        if (token == NULL) continue;
        strcpy(employees[count].name, token);  // копируем имя
        
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        employees[count].id = atoi(token);     // преобразуем строку в число
        
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strcpy(employees[count].level, token); // копируем уровень
        
        count++;
    }
    
    fclose(file);  // закрываем файл
    
    if (count == 0) {
        printf("Файл пуст или имеет неверный формат\n");
        return 1;
    }
    
    printTable(employees, count);

    saveToBinary(employees, count);

    searchByID();
    
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "laba12.h"

void generate(struct Student arr[], int n) {
    char* surnames[] = {"Милюков", "Куприенко", "Рожников", "Тимонин", "Науменко"};
    char* names[] = {"Святослав", "Иван", "Костя", "Михаил", "Семен"};
    char* groups[] = {"ИКС-532", "ИКС-531", "ИП-516", "ИП-514", "ИИ-551"};

    for (int i = 0; i < n; i++){
        strcpy(arr[i].surname, surnames[rand()%5]);
        strcpy(arr[i].name, names[rand() % 5]);
        strcpy(arr[i].group, groups[rand() % 5]);
        arr[i].pole = rand()%101;
    }
}

void print(const struct Student arr[], int n) {
    printf("\n----------------------------------------\n");
    printf("|Фамилия     |Имя     |Группа    |Ср.балл \n");
    printf("------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("|%-10s    |%-9s     |%-8s    |%3d     \n",
                arr[i].surname, arr[i].name, arr[i].group, arr[i].pole);
    }
    printf("------------------------------------------\n");
}

void sort(struct Student arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j].pole > arr[j+1].pole) {
                struct  Student temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}
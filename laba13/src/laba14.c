#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct room {
    char name[50];
    int level;
    int number;
    int resolution;
};
// двусвязный список
typedef struct node {
    struct room data;
    struct node* next; //указывает на следующий узел
    struct node* prev; //указывает на предыдущий уровень
} node_t;
// создание списка
node_t* create_node(struct room r) {
        node_t* new_node = (node_t*)malloc(sizeof(node_t));
        if (new_node == NULL) {
            printf("Ошибка: не удалось выделить память\n");
            exit(1);
        }
        new_node->data = r;
        new_node->next = NULL;
        new_node->prev = NULL;
        return new_node;
}
// перенос в начало списка
void add_to_end(node_t** head, struct room r) {
    node_t* new_node = create_node(r); //создает новый узел с данными r
    new_node->next = *head; //новый узел указывает на текущие начало списка
    new_node->prev = NULL; //новый узел становится первым, у него нет предыдущего
    if (*head != NULL) { 
        (*head)->prev = new_node;
    }
    *head = new_node;
}
// рандомчик комнат
struct room generate_random_room() {
    struct room r;
    const char* names[] = {
        "Кухня", "Ванна", "Гостиная", "Спальня", "Туалет", "Коридор", "Прачка", "Комнота кота"
    };
    snprintf(r.name, sizeof(r.name), "%s_%d", names[rand() % 8], rand() % 100);
    r.level = rand () % 10 + 1;
    r.number = rand() % 1000;
    r.resolution = rand() % 50 + 10;
    
    return r;
}
// функция вывода списка
void print_list(node_t* head) {
    if (head == NULL) {
        printf("Список пуст\n");
        return;
    }
    node_t* temp = head;
    int position = 1;
    printf("\n");
    printf("=====================================\n");
    printf("        СПИСОК ВСЕХ КОМНАТ          \n");
    printf("=====================================\n");

    while (temp != NULL) { //пока не достигнут конец списка
        printf("%2d. %-15s | Lvl: %2d | Num: %3d | Size: %2d\n",
            position,
            temp->data.name,
            temp->data.level,
            temp->data.number,
            temp->data.resolution);
        temp = temp->next; //переход к селдующиму узлу и увеличение счетчика позиций 
        position++;    
    }
    printf("=====================================\n");
    printf("Всего комнат: %d\n", position - 1);
    printf("\n");
}

void print_current(node_t* current, int pos) {
    printf("\n");
    printf("╔════════════════════════════════╗\n");
    printf("║   ТЕКУЩАЯ ПОЗИЦИЯ: %2d          ║\n", pos);
    printf("╠════════════════════════════════╣\n");
    printf("║ Название: %-18s   ║\n", current->data.name);
    printf("║ Уровень:  %-2d                   ║\n", current->data.level);
    printf("║ Номер:    %-3d                  ║\n", current->data.number);
    printf("║ Размер:   %-3d                  ║\n", current->data.resolution);
    printf("╚════════════════════════════════╝\n");
}
//НАВИГАЦИЯ
node_t* navigate(node_t* current, node_t* head, char key, int* pos) { 
    if (key == 'd' || key == 'D' || key == '6') {
        if (current->next != NULL) {
            current = current->next;
            (*pos)++;
        } else {
            printf("\n Конец списка, дальше некуда\n");
            printf("Нажмите 'S' или '2', чтобы вернуться в начало\n");
        }
    }
    else if (key == 'a' || key == 'A' || key == '4' || key == 'w' || key == 'W' || key == '8'){
        if (current->prev != NULL) {
            current = current->prev;
            (*pos)--;
        } else {
            printf("\n Начало списка\n");
            printf("Нажми 'D'  или '6', чтобы пойти вперед\n");
        }
    }
    else if (key == 's' || key == 'S' || key == '2'){
        current = head;
        *pos = 1;
        printf("\n Возврат в начало списка\n");
    }
    return current;
}

void free_list(node_t* head) { //функция освбождения памяти, занятой списком
    node_t* temp;
    while (head != NULL) {
        temp = head; // сохраняем текущий узел
        head = head->next;//перемещаем голову на следующий
        free(temp);//освобождаем сохранненый узел
    }
}

int main() {
    srand(time(NULL));
    node_t* head = NULL;
    node_t* S = head;
    node_t* current = NULL;
    int n;
    int position = 1;
    printf("Введите количество комнат в списке: ");
    scanf("%d", &n);
    if (n <= 0) {
        printf("Ошибочка, количество должно быть больше 0\n");
        return 1;
    }
    for (int i = 0; i < n; i++) {
        add_to_end(&head, generate_random_room());
    }
    print_list(head);
    if (head == NULL) {
        printf("Список пуст\n");
        return 0;
    }
    current = head;
    position = 1;

    printf("\n");
    printf("╔════════════════════════════════════╗\n");
    printf("║        УПРАВЛЕНИЕ НАВИГАЦИЕЙ       ║\n");
    printf("╠════════════════════════════════════╣\n");
    printf("║  D или 6  →  Вправо / Вниз         ║\n");
    printf("║  A или 4  →  Влево / Вверх         ║\n");
    printf("║  W или 8  →  Вверх (как A/4)       ║\n");
    printf("║  S или 2  →  В начало (позиция S)  ║\n");
    printf("║  Q или X  →  Выход из программы    ║\n");
    printf("╚════════════════════════════════════╝\n");

    print_current(current, position);
    while (1) {
        printf("\nНажмите клавишу для навигации: ");
        char key;
        scanf(" %c", &key);
        printf("%c\n", key);
        if (key == 'q' || key == 'Q' || key == 'x' || key == 'X') {
            printf("\nПока-пока");
            break;
        }
        current = navigate(current, head, key, &position);
        print_current(current, position);
        printf("Подсказка: A/4 - назад, D/6 - вперед, S/2 - в начало, Q - выход\n");
    }
    printf("\nОчистка памяти\n");
    free_list(head);
    printf("Готово, память свободна\n");
    return 0;
}
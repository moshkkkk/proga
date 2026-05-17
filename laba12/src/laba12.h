#ifndef STUDENT_H
#define STUDENT_H

struct Student
{
    char surname[20];
    char name[20];
    char group[10];
    int pole;
};

void generate(struct Student arr[], int n);
void print(const struct Student arr[], int n);
void sort(struct Student arr[], int n);

#endif
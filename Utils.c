//
// Created by razergom on 11/12/2018.
//

#include "Utils.h"

void swap (int *x, int *y) {
    /*
     * Обмен значений элементов
     * по указателям x и y
     */
    int t = *x;
    *x = *y;
    *y = t;
}

int in_range(int number, int n) {
    /*
     * Возвращает 1, если число number находится
     * в пределах от 0 до n - 1 включительно
     */
    return number >= 0 && number < n * n;
}
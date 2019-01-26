//
// Created by razergom on 11/12/2018.
//

#include "IDA.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define FOUND -2
#define INF INT_MAX

int search_for_depth(Grid *current, int treshold, int n) {
    /*
     * Рекурсивный поиск решения игры пятнашки (упорядоченной сетки
     * grid в структуре current размера n * n) по алгоритму IDA*,
     * начиная с узла current, g - количество ходов, сделанных для
     * получения current. В случае нахождения решения выводит как
     * цепь сеток, приведших к результату, так и последовательность
     * действий для достижения решения. Возвращает значение,
     * соответствующее выигрышу (например, -2, для прекращения
     * выполнения функции), либо первое попавшийся на пути
     * значение f при поиске, которое превысило значение threshold.
     */
    if (current->f > treshold) {
        return current->f;
    }
    if (!calculate_manhattan_distance(current->grid, n)) {
        printf("\n");
        int count = 0;
        print_path(current, n, &count);
        print_sequence(current);
        printf("\nCount: %d\n", count - 1);
        return FOUND;
    }
    int min = INT_MAX;
    int invMove = get_inverse_last_move(current);
    for (int i = 0; i < 4; i++) {
        if (safe(current, n, i) && i != invMove) {
            Grid *child = create_child(current, n, i);
            int tmp = search_for_depth(child, treshold, n);
            free(child);
            if (tmp == FOUND) {
                return FOUND;
            }
            if (tmp < min) {
                min = tmp;
            }
        }
    }
    return min;
}

void solveIDAStar(grid_t gr, int n) {
    /*
     * Является «контроллером» алгоритма IDA*, управляет вызовом
     * функции  search_for_depth(). Подготавливает данные,
     * необходимые для самого поиска: преобразовывает сетку gr
     * размера n * n в структуру типа Grid, рассчитывает начальный
     * предел глубины спуска на ветвях, равный эвристике начального
     * узла, полученного из сетки gr. Обновляет при каждой итерации
     * этот предел
     */
    Grid *start = create_grid(gr, n);
    int treshold = calculate_manhattan_distance(start->grid, n);
    while (1) {
        int tmp = search_for_depth(start, treshold, n);
        if (tmp == FOUND) {
            return;
        }
        if (tmp == INF) {
            return;
        }
        treshold = tmp;
    }
}
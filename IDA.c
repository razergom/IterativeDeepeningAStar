//
// Created by razergom on 11/12/2018.
//

#include "IDA.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define FOUND -2
#define INF INT_MAX

int search_for_depth(Grid *current, int g, int treshold, int n) {
    /*
     * Ищет в глубину treshold нужную сетку...
     * description is in progress...
     */
    if (current->f > treshold) {
        return current->f;
    }
    if (!calculate_manhattan_distance(&(current->grid), n)) {
        printf("\n");
        int count = 0;
        print_path(current, n, &count);
        print_sequence(current, n);
        printf("\nCount: %d\n", count);
        return FOUND;
    }
    int min = INT_MAX;
    int invMove = get_inverse_last_move(current);
    for (int i = 0; i < 4; i++) {
        if (safe(current, n, i) && i != invMove) {
            Grid *child = create_child(current, n, i);
            int tmp = search_for_depth(child, child->g, treshold, n);
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

void solveIDAStar(grid_t *gr, int n) {
    /*
     * Решает данную сетку игры N-Puzzle
     * размера n * n по алгоритму IDA*
     * эвристического поиска
     * кратчайшего пути. Выводит
     * последовательность сеток, которая
     * которая привела к результату.
     * (Последовательность преобразований сетки
     * смещением нулевого элемента на одну позицию
     * в сторону каждый ход, которая привела
     * к упорядочиванию сетки)
     */
    Grid *start = create_grid(gr, n);
    int treshold = calculate_manhattan_distance(&(start->grid), n);
    while (1) {
        int tmp = search_for_depth(start, 0, treshold, n);
        if (tmp == FOUND) {
            return;
        }
        if (tmp == INF) {
            return;
        }
        treshold = tmp;
    }
}
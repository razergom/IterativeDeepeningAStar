//
// Created by razergom on 11/12/2018.
//

#include "Puzzle.h"
#include <stdio.h>
#include <stdlib.h>
#include "Utils.h"

#define REPEATING_NUMBER 1
#define NOT_ENOUGH_ELEMS 2

int num;

enum Directions {
    LEFT,
    TOP,
    RIGHT,
    BOTTOM,
};


void gridcpy(grid_t g1, grid_t g2, int n) {
    /*
     * Копирует сетку g2 в g1, обе сетки
     * размера n * n
     */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            g1[i][j] = g2[i][j];
        }
    }
}

int safe(Grid *puzzle, int n, int move) {
    /*
     * Возвращает 1, если ход move в
     * сетке gr размера n * n безопасный
     * (не выходит за границы сетки),
     * иначе - 0
     */
    switch (move) {
        case LEFT:
            return (puzzle->zero.j - 1) >= 0;
        case TOP:
            return (puzzle->zero.i - 1) >= 0;
        case RIGHT:
            return (puzzle->zero.j + 1) < n;
        case BOTTOM:
            return (puzzle->zero.i + 1) < n;
        default:
            return -1;
    }
}

void output_grid(grid_t gr, int n) {
    /*
     * Вывод сетки gr размера n * n
     */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!gr[i][j]) {
                printf("-- ");
                continue;
            }
            printf("%02d ", gr[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int calculate_manhattan_distance(grid_t gr, int n) {
    /*
     * Возвращает дистанциюю Мэнхэттэна для
     * сетки gr размера n * n. (Сумма расстояний
     * каждой ячейки (каждого элемента) сетки до своей позиции)
     */
    int res = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (gr[i][j]) {
                int targetx = (gr[i][j] - 1) / n;
                int targety = (gr[i][j] - 1) % n;
                int dx = i - targetx;
                int dy = j - targety;
                res += abs(dx) + abs(dy);
            }
        }
    }
    return res;
}


Coordinate search_zero(grid_t inGrid, int n) {
    /*
     * Возвращает структуру типа Coordinate,
     * содержащий координаты нулевого элемента
     * в сетке inGrid размера n * n.
     * (Предполагается, что нулевой элемент в матрице
     * есть)
     */
    Coordinate zero;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if(!inGrid[i][j]) {
                zero.i = i;
                zero.j = j;
                return zero;
            }
        }
    }
}

Grid *create_grid(grid_t inGrid, int n) {
    /*
     * Возвращает указатель на структуру типа
     * Grid, расположенной в динамической памяти.
     * Копирует сетку inGrid размера n * n в соответсвующее
     * поле структуры Grid. В поле zero записываются
     * координаты нулевого элемента в сетке inGrid.
     * В поле parent записывается NULL, т.к к эта сетка
     * является начальной "позицией". В поля g и h записывает
     * 0 и эвристическое расстояние до конечной позиции
     * соответственно
     */
    Grid *retgrid = (Grid*)malloc(sizeof(Grid));
    gridcpy(retgrid->grid, inGrid, n);
    retgrid->parent = NULL;

    retgrid->zero = search_zero(inGrid, n);
    retgrid->g = 0;
    retgrid->f = calculate_manhattan_distance(retgrid->grid, n);

    retgrid->lastMove = -1;

    return retgrid;
}

Grid *create_child(Grid *inGrid, int n, int move) {
    /*
     * Возвращает указатель на структуру типа Grid, расположенной в
     * динамической памяти. Копирует сетку размера n * n структуры inGrid
     * в соответствующее поле структуры Grid. Меняет позициями нулевой элемент
     * сетки в соответствии с move (0 - с элементом, находящимся слева от
     * нулевого, 1 - сверху, 2 - справа, 3 - снизу). В поле parent записывается
     * указатель на сетку "родителя" - inGrid, из которой была получена "дочерняя"
     * сетка. Для "дочерней" сетки считаются значения g и f
     * (кол-во ходов для получения этой сетки и кол-во ходов вместе с эвристическим
     * значением соответственно) и записываются в соответствующие позиции возвращаемой
     * структуры.
     */
    Grid *retgrid = (Grid*)malloc(sizeof(Grid));
    gridcpy(retgrid->grid, inGrid->grid, n);
    retgrid->parent = inGrid;

    retgrid->zero.i = inGrid->zero.i;
    retgrid->zero.j = inGrid->zero.j;

    switch (move) {
        case LEFT:
            retgrid->zero.j--;
            break;
        case TOP:
            retgrid->zero.i--;
            break;
        case RIGHT:
            retgrid->zero.j++;
            break;
        default:
            retgrid->zero.i++;
            break;
    }

    int i1 = retgrid->zero.i;
    int j1 = retgrid->zero.j;
    int i2 = inGrid->zero.i;
    int j2 = inGrid->zero.j;

    swap(&(retgrid->grid[i1][j1]), &(retgrid->grid[i2][j2]));

    retgrid->g = inGrid->g + 1;
    retgrid->f = retgrid->g + calculate_manhattan_distance(retgrid->grid, n);

    retgrid->lastMove = move;

    return retgrid;
}

void print_path(Grid *inGrid, int n, int *count) {
    /*
     * Выводит последовательность сеток размера n * n, которая
     * привела к сетке inGrid того же размера, на которую указывает
     * указатель inGrid, следуя
     * по родительским указателям
     */
    if (inGrid == NULL) {
        return;
    }
    print_path(inGrid->parent, n, count);
    output_grid(inGrid->grid, n);
    num++;
    *count = num;
}

void print_sequence(Grid *inGrid) {
    /*
     * Выводит последовательность сеток размера n * n, которая
     * привела к сетке того же размера, на которую указывает
     * указатель inGrid того же размера, следуя
     * по родительским указателям
     */
    if (inGrid == NULL) {
        return;
    }
    char s[SIZE] = "LTRB";
    print_sequence(inGrid->parent);
    if (inGrid->lastMove != -1) {
        printf("%c ", s[inGrid->lastMove]);
    }
}

void get_grid_from_txt_file(char *f_name, grid_t gr, int n) {
    /*
     * Считывает из текстового файла f_name целые числа,
     * заполняет сетку gr размера n * n считанными из файла f_name
     * данными. Если считать не получилось, либо данные представлены
     * неверно - глобальной переменной ошибки присваивается
     * соответствующая ошибка.
     */
    int numbers_set[16] = {0}; // чтобы вводимые числа не повторялись

    FILE *f = fopen(f_name, "r");
    if (f == NULL) {
        fclose(f);
        error = 1;
        perror("Cause");
    }

    int i = 0;
    int j = 0;
    int buff = 0;
    int count = 0;
    while (!feof(f)) {
        if (fscanf(f, "%d", &buff) == 1) {
            if (!numbers_set[buff] && in_range(buff, n)) {
                numbers_set[buff]++;
                count++;
                gr[i][j] = buff;
                j++;
                if (j % n == 0) {
                    i++;
                    j = 0;
                }
            } else {
                error = REPEATING_NUMBER;
                fclose(f);
                return;
            }
        }
    }

    if (count != n * n) {
        error = NOT_ENOUGH_ELEMS;
    }
    fclose(f);
}

int get_inverse_last_move(Grid *current) {
    /*
     * Вовзращает ход, обратный ходу lastMove,
     * хранящийся в структуре current (например,
     * lastMove = 1, это значит “вверх”, следовательно,
     * обратный будет “вниз”, то есть программа вернет 3)
     * lastMove для самого первого, начального узла равен -1
     */
    if (current->lastMove != -1) {
        if (current->lastMove < 2 && current->lastMove != -1) {
            return current->lastMove + 2;
        } else {
            return current->lastMove - 2;
        }
    }
    return 4;
}

void to_flat(grid_t mattr, int *linearMattr, int n) {
    /*
     * Записывает матрицу mattr размера n * n в
     * массив linearMattr размера n * n.
     */
    int index = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            linearMattr[index] = mattr[i][j];
            index++;
        }
    }
}

int solvable(grid_t mattr, int n) {
    /*
     * Возращает 1, если данное расположение элементов в матрице mattr размера n * n
     * можно привести к упорядоченному виду по возрастанию в направлениях “слева-направо”
     * и “сверху-вниз”, перемещая каждый раз нулевой элемент в одну из соседних позиций.
     * Иначе - 0.
     */
    int sum = 0;
    int zeroi = 0;
    int linearMattr[SIZE * SIZE];
    to_flat(mattr, linearMattr, n);
    for (int i = 0; i < n * n; i++) {
        if (linearMattr[i]) {
            int tmpsum = 0;
            for (int j = i + 1; j < n * n; j++) {
                if (linearMattr[j] < linearMattr[i] && linearMattr[j]) {
                    tmpsum++;
                }
            }
            sum += tmpsum;
        } else {
            zeroi = i / n;
        }
    }
    sum += zeroi + 1;
    return sum % 2 == 0;
}
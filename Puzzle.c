//
// Created by razergom on 11/12/2018.
//

#include "Puzzle.h"
#include <stdio.h>
#include <stdlib.h>
#include "Utils.h"

int num;

enum Directions {
    LEFT,
    TOP,
    RIGHT,
    BOTTOM,
};

int equal(grid_t *g1, grid_t *g2, int n) {
    /*
     * Возвращает 1, если сетки g1 и g2 размера
     * n * n равны, иначе - 0
     */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if ((*g1)[i][j] != (*g2)[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

void gridcpy(grid_t *g1, grid_t *g2, int n) {
    /*
     * Копирует сетку по указателю g2 в g1, обе сетки
     * размера n * n
     */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            (*g1)[i][j] = (*g2)[i][j];
        }
    }
}

int safe(Grid *puzzle, int n, int move) {
    /*
     * Возвращает 1, если ход move в
     * сетке размера n * n по указателю gr безопасный
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

void output_grid(grid_t *gr, int n) {
    /*
     * Вывод сетки размера n * n по
     * указателю gr
     */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!(*gr)[i][j]) {
                printf("-- ");
                continue;
            }
            printf("%02d ", (*gr)[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int calculate_manhattan_distance(grid_t *gr, int n) {
    /*
     * Возвращает дистанциюю Мэнхэттэна для
     * сетки размера n * n по указателю gr. (Сумма расстояний
     * каждой ячейки (каждого элемента) сетки до своей позиции)
     */
    int res = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if ((*gr)[i][j]) {
                int targetx = ((*gr)[i][j] - 1) / n;
                int targety = ((*gr)[i][j] - 1) % n;
                int dx = i - targetx;
                int dy = j - targety;
                res += abs(dx) + abs(dy);
            }
        }
    }
    return res;
}

/*int heuristic(grid_t *gr, int n) {
    int h1 = calculate_manhattan_distance(gr, n);
    int h2 = calculate_linear_conflict(gr, n);
    return h1 + h2;
}*/

Coordinate search_zero(grid_t *inGrid, int n) {
    /*
     * Возвращает структуру типа Coordinate,
     * содержащий координаты нулевого элемента
     * в сетке размера n * n по указателю inGrid.
     * (Предполагается, что нулевой элемент в матрице
     * есть)
     */
    Coordinate zero;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if(!(*inGrid)[i][j]) {
                zero.i = i;
                zero.j = j;
                return zero;
            }
        }
    }
}

Grid *create_grid(grid_t *inGrid, int n) {
    /*
     * Возвращает указатель на структуру типа
     * Grid, расположенной в динамической памяти.
     * Копирует сетку размера n * n, расположенной по
     * указателю inGrid в соответсвующее поле структуры Grid.
     * В поле zero записываются координаты нулевого
     * элемента в сетке inGrid. В поле parent записывается
     * NULL, т.к к эта сетка является начальной "позицией".
     * В поля g и h записывает 0 и эвристическое расстояние
     * до конечной позиции соответственно
     */
    Grid *retgrid = (Grid*)malloc(sizeof(Grid));
    gridcpy(&(retgrid->grid), inGrid, n);
    retgrid->parent = NULL;

    retgrid->zero = search_zero(inGrid, n);
    retgrid->g = 0;
    retgrid->f = calculate_manhattan_distance(&(retgrid->grid), n);

    retgrid->lastMove = -1;

    return retgrid;
}

Grid *create_child(Grid *inGrid, int n, int move) {
    /*
     * Возвращает указатель на структуру типа
     * Grid, расположенной в динамической памяти.
     * Копирует сетку размера n * n, расположенной по
     * указателю inGrid в соответсвующее поле структуры Grid.
     * Меняет позициями нулевой элемент сетки в соответствии с
     * move (0 - с элементом, находящимся слева от нулевого,
     * 1 - сверху, 2 - справа, 3 - снизу). Предполагается,
     * что до вызова этой функции вызывается функция safe()
     * В поле parent записывается указатель на сетку
     * "родителя" - inGrid, из которой была получена
     * "дочерняя" сетка. Для "дочерней" сетки
     * считаются значения g и f (кол-во ходов до получения
     * этой сетки и кол-во ходов вместе с эвристическим
     * значением соответственно) и записываются в
     * соответствующие позиции
     */
    Grid *retgrid = (Grid*)malloc(sizeof(Grid));
    gridcpy(&(retgrid->grid), &(inGrid->grid), n);
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
    retgrid->f = retgrid->g + calculate_manhattan_distance(&(retgrid->grid), n);

    retgrid->lastMove = move;

    return retgrid;
}

void print_path(Grid *inGrid, int n, int *count) {
    /*
     * Выводит последовательность сеток размера n * n, которая
     * привела к сетке того же размера, на которую указывает
     * указатель inGrid того же размера, следуя
     * по родительским указателям
     */
    if (inGrid == NULL) {
        return;
    }
    print_path(inGrid->parent, n, count);
    output_grid(&(inGrid->grid), n);
    num++;
    *count = num;
}

void print_sequence(Grid *inGrid, int n) {
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
    print_sequence(inGrid->parent, n);
    if (inGrid->lastMove != -1) {
        printf("%c ", s[inGrid->lastMove]);
    }
}

void get_grid_from_txt_file(char *f_name, grid_t *gr, int n) {
    /*
     * Считывает из текстового файла f_name целые числа,
     * заполняет сетку размера n * n по указателю gr считанными
     * из файла f_name данными. Если считать не получилось, либо
     * данные представлены неверно - глобальной переменной ошибки
     * присваивается 1
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
    while (!feof(f)) {
        if (fscanf(f, "%d", &buff) == 1) {
            if (!numbers_set[buff] && in_range(buff, n)) {
                numbers_set[buff]++;
                (*gr)[i][j] = buff;
                j++;
                if (j % n == 0) {
                    i++;
                    j = 0;
                }
            } else {
                error = 1;
                fclose(f);
                return;
            }
        } else {
            error = 1;
            fclose(f);
            return;
        }
    }

    fclose(f);
}

int get_inverse_last_move(Grid *current) {
    /*
     * Вовзращает ход, хранящий в поле
     * lastMove структуры current, обратный
     * ходу lastMove
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

void to_flat(grid_t *mattr, int *linearMattr, int n) {
    int index = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            linearMattr[index] = (*mattr)[i][j];
            index++;
        }
    }
}

int solvable(grid_t *mattr, int n) {
    /*
     * Возвращает 1, если данный(ую) паззл (матрицу) (данную
     * расстановку пятнашек) размера n * m можно собрать
     * (решение есть), иначе - 0.
     * zeroindex - индекс строки, в которой находится
     * нуль
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
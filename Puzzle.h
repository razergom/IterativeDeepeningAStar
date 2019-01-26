//
// Created by razergom on 11/12/2018.
//

#ifndef ITERATIVEDEEPINGASTAR_PUZZLE_H
#define ITERATIVEDEEPINGASTAR_PUZZLE_H

#define SIZE 4

int error;

typedef int grid_t[SIZE][SIZE];

typedef struct coordinate {
    int i;
    int j;
} Coordinate;

typedef struct grid {
    grid_t grid;
    int g, f;
    Coordinate zero;
    int lastMove;
    struct grid *parent;
} Grid;

void gridcpy(grid_t g1, grid_t g2, int n);
void output_grid(grid_t g, int n);
int calculate_manhattan_distance(grid_t gr, int n);
Grid *create_grid(grid_t gr, int n);
Grid *create_child(Grid *inGrid, int n, int move);
int safe(Grid *puzzle, int n, int move);
void print_path(Grid *inGrid, int n, int *count);
void print_sequence(Grid *inGrid);
void get_grid_from_txt_file(char *f_name, grid_t gr, int n);
int get_inverse_last_move(Grid *current);
int solvable(grid_t mattr, int n);

#endif //ITERATIVEDEEPINGASTAR_PUZZLE_H

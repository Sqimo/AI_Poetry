#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE *in;
FILE *out;

void free_sudoku(int *sudoku) {
    free(sudoku);
}

int *init_sudoku(void) {
    int *trial = malloc(sizeof(*trial) * 81);
    return trial;}

void print_sudoku(int *sudoku) {
    int i, j, k;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++)
            fprintf(out, "%d", sudoku[i * 9 + j]);
        fprintf(out, "\n");}}

int can_be_placed(int *sudoku, int pos, int elem) {
    int str = pos / 9;
    int col = pos % 9;
    int i;
    for (i = 0; i < 9; i++) {
        if (sudoku[9 * str + i] == elem) {
            return 0;}}
    for (i = 0; i < 9; i++) {
        if (sudoku[9 * i + col] == elem) {
            return 0;}}
    col = col / 3;
    col *= 3;
    str = str / 3;
    str *= 3;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (sudoku[9 * (str + i) + col + j] == elem) {
                return 0;}}}
    return 1;}

int num_of_elems(int *sudoku, int pos) {
    int i, n = 0;
    for (i = 1; i <= 9; i++) {
        if (can_be_placed(sudoku, pos, i))
            n++;
    }
    return n;
}

int can_be_solved(int *sudoku) {
    int i, j, elem = -1;
    for (i = 0; i < 81; i++) {
        if (sudoku[i] == 0) {
            if (elem == -1)
                elem = i;
            else if (num_of_elems(sudoku, i) < num_of_elems(sudoku, elem))
                elem = i;}}
    i = elem;
    if (i == -1) {
        print_sudoku(sudoku);
        return 1;}
    for (j = 1; j <= 9; j++) {
        if (can_be_placed(sudoku, i, j)) {
            sudoku[i] = j;
            if (can_be_solved(sudoku)) {
                sudoku[i] = 0;
                return 1;}}}
    sudoku[i] = 0;
    return 0;}

void scan_sudoku(int *sudoku) {
    int i;
    char a;
    fscanf(in, "%c", &a);
    while (a != 'G')
        fscanf(in, "%c", &a);
    for (i = 0; i < 8; i++)
        fscanf(in, "%c", &a);
    for (i = 0; i < 81; i++) {
        fscanf(in, "%c", &a);
        if (a - '0' < 0 || '9' - a < 0) {
            i--;
            continue;}
        sudoku[i] = a - '0';}}

int main(void) {
    in = fopen("sudoku.txt", "r");
    out = fopen("sudoku.out", "w");
    int *sudoku = init_sudoku();
    int i;
    for (i = 1; i < 1001; i++) {
        scan_sudoku(sudoku);
        fprintf(out, "Grid %04d\n", i);
        printf("%d", can_be_solved(sudoku));
    }
    free_sudoku(sudoku);
}

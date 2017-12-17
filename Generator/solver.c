#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE *in;
FILE *out;

int *init_sudoku(void) {
    int *trial = malloc(sizeof(*trial) * 81);
    return trial;}

void print_sudoku(int *sudoku) {
    int i, j, k;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++)
            fprintf(out, "%d ", sudoku[i * 9 + j]);
        fprintf(out, "\n");}}

int can_be_placed(int *sudoku, int pos, int elem) {
    int str = pos / 9;
    int col = pos % 9;
    int i;
    for (i = 0; i < 9; i++) {
        if (sudoku[9 * str + i] == elem) {
            //printf("point 1");
            return 0;}}
    for (i = 0; i < 9; i++) {
        if (sudoku[9 * i + col] == elem) {
            //printf("point 2");
            return 0;}}
    col = col / 3;
    col *= 3;
    str = str / 3;
    str *= 3;
    //printf("(%d %d)\n", str, col);
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (sudoku[9 * (str + i) + col + j] == elem) {
                //printf("point 3");
                return 0;}}}
    return 1;}

int can_be_solved(int *sudoku, int start_pos) {
    int i, j;
    for (i = start_pos; i < 81; i++) {
        if (sudoku[i] == 0)
            break;}
    if (i == 81) {
        print_sudoku(sudoku);
        //printf("1");
        return 1;}
    for (j = 1; j <= 9; j++) {
        if (can_be_placed(sudoku, i, j)) {
            sudoku[i] = j;
            if (can_be_solved(sudoku,i)) {
                //printf("2");
                sudoku[i] = 0;
                return 1;}}}
    sudoku[i] = 0;
    //fprintf(out, "%d\n", i);
    //print_sudoku(sudoku);
    //printf("3");
    return 0;}

int number_of_solutions(int *sudoku, int pos) {
    int i, n = 0;
    for (i = 1; i <= 9; i++) {
        sudoku[pos] = i;
        if (can_be_solved(sudoku, 0))
            n += 1;
    }
    return n;
}

void init_grid(int *sudoku) {
    int i, j, k;
    for (i = 0; i < 9; i++){
        for (j = 0; j < 9; j++) {
            if (i < 3)
                sudoku[9 * i + j] = (j + 3 * i) % 9 + 1;
            else if (i < 6)
                sudoku[9 * i + j] = (j + 3 * i + 1) % 9 + 1;
            else
                sudoku[9 * i + j] = (j + 3 * i + 2) % 9 + 1;}}}

void transposing(int *sudoku) {
    int i, j, trial;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (j > i) {
                trial = sudoku[9 * i + j];
                sudoku[9 * i + j] = sudoku[9 * j + i];
                sudoku[9 * j + i] = trial;}}}}

void swap_strings(int *sudoku) {
    int trial, i;
    int block = rand() % 3;
    int s1 = rand() % 3;
    int s2 = rand() % 2;
    if (s2 < s1) {
        for (i = 0; i < 9; i++) {
            trial = sudoku[block * 27 + s1 * 9 + i];
            sudoku[block * 27 + s1 * 9 + i] = sudoku[block * 27 + s2 * 9 + i];
            sudoku[block * 27 + s2 * 9 + i] = trial;}}
    else {
        for (i = 0; i < 9; i++) {
            trial = sudoku[block * 27 + s1 * 9 + i];
            sudoku[block * 27 + s1 * 9 + i] = sudoku[block * 27 + (s2 + 1) * 9 + i];
            sudoku[block * 27 + (s2 + 1) * 9 + i] = trial;}}}

void swap_collumns(int *sudoku) {
    int trial, i;
    int block = rand() % 3;
    int c1 = rand() % 3;
    int c2 = rand() % 2;
    if (c2 < c1) {
        for (i = 0; i < 9; i++) {
            trial = sudoku[block * 3 + i * 9 + c1];
            sudoku[block * 3 + i * 9 + c1] = sudoku[block * 3 + i * 9 + c2];
            sudoku[block * 3 + i * 9 + c2] = trial;}}
    else {
        for (i = 0; i < 9; i++) {
            trial = sudoku[block * 3 + i * 9 + c1];
            sudoku[block * 3 + i * 9 + c1] = sudoku[block * 3 + i * 9 + c2 + 1];
            sudoku[block * 3 + i * 9 + c2 + 1] = trial;}}}

void swap_hor_blocks(int *sudoku) {
    int trial, i, j;
    int block1 = rand() % 3;
    int block2 = rand() % 2;
    if (block2 < block1) {
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 9; j++) {
                trial = sudoku[block1 * 27 + 9 * i + j];
                sudoku[block1 * 27 + 9 * i + j] = sudoku[block2 * 27 + 9 * i + j];
                sudoku[block2 * 27 + 9 * i + j] = trial;}}}
    else {
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 9; j++) {
                trial = sudoku[block1 * 27 + 9 * i + j];
                sudoku[block1 * 27 + 9 * i + j] = sudoku[(block2 + 1) * 27 + 9 * i + j];
                sudoku[(block2 + 1) * 27 + 9 * i + j] = trial;}}}}

void swap_vert_blocks(int *sudoku) {
    int trial, i, j;
    int block1 = rand() % 3;
    int block2 = rand() % 2;
    if (block2 < block1) {
        for (i = 0; i < 9; i++) {
            for (j = 0; j < 3; j++) {
                trial = sudoku[block1 * 3 + 9 * i + j];
                sudoku[block1 * 3 + 9 * i + j] = sudoku[block2 * 3 + 9 * i + j];
                sudoku[block2 * 3 + 9 * i + j] = trial;}}}
    else {
        for (i = 0; i < 9; i++) {
            for (j = 0; j < 3; j++) {
                trial = sudoku[block1 * 3 + 9 * i + j];
                sudoku[block1 * 3 + 9 * i + j] = sudoku[(block2 + 1) * 3 + 9 * i + j];
                sudoku[(block2 + 1) * 3 + 9 * i + j] = trial;}}}}

void random_grid(int *sudoku) {
    int i, a;
    for (i = 0; i < 60; i++) {
        a = rand() % 5;
        if (a == 0)
            transposing(sudoku);
        if (a == 1)
            swap_strings(sudoku);
        if (a == 2)
            swap_collumns(sudoku);
        if (a == 3)
            swap_hor_blocks(sudoku);
        else
            swap_hor_blocks(sudoku);}}

void scan_sudoku(int *sudoku) {
    int i;
    char a;
    for (i = 0; i < 81; i++) {
        fscanf(in, "%c", &a);
        if (a - '0' < 0 || '9' - a < 0) {
            i--;
            continue;}
        sudoku[i] = a - '0';}}

void free_sudoku(int *sudoku) {
    free(sudoku);
}

int main(void) {
    srand(time(NULL) * 1234567890);
    int a = rand();
    in = fopen("sudoku.in", "r");
    out = fopen("sudoku.out", "w");
    int *sudoku = init_sudoku();
    scan_sudoku(sudoku);
    int i;
    for (i = 0; i < 50; i++) {
        printf("%d", can_be_solved(sudoku, 0));
        fprintf(out, "\n");
    }
    //init_grid(sudoku);
    //print_sudoku(sudoku);
    //fprintf(out, "\n");
    //random_grid(sudoku);
    //print_sudoku(sudoku);
    free_sudoku(sudoku);
    return 0;
}

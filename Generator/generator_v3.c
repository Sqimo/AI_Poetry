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
    int i, j;
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
                elem = i;
        }
    }
    i = elem;
    if (i == -1) {
        return 1;}
    for (j = 1; j <= 9; j++) {
        if (can_be_placed(sudoku, i, j)) {
            sudoku[i] = j;
            if (can_be_solved(sudoku)) {
                sudoku[i] = 0;
                return 1;}}}
    sudoku[i] = 0;
    return 0;}

int number_of_solutions(int *sudoku, int pos) {
    int i, n = 0;
    for (i = 1; i <= 9; i++) {
        if (can_be_placed(sudoku, pos, i)) {
            sudoku[pos] = i;
            if (can_be_solved(sudoku))
                n += 1;}}
    return n;}

void init_grid(int *sudoku) {
    int i, j;
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

void normalize(int *sudoku) {
    int i;
    for (i = 0; i < 81; i++) {
        if (sudoku[i] < 0)
            sudoku[i] = -sudoku[i];}
}

int *copy_sudoku(int *sudoku) {
    int *trial = malloc(sizeof(int) * 81);
    int i;
    for (i = 0; i < 81; i++)
        trial[i] = sudoku[i];
    return trial;}

int delete_num(int *sudoku, int n){
    int i = 0;
    int j = n;
    while (j >= 0) {
        if (sudoku[i] <= 0)
            i++;
        else {
            if (j == 0)
                break;
            j--;
            i++;}}
    int *new_sudoku = copy_sudoku(sudoku);
    normalize(new_sudoku);
    new_sudoku[i] = 0;
    if(number_of_solutions(new_sudoku, i) == 1) {
        free_sudoku(new_sudoku);
        sudoku[i] = 0;
        return 1;}
    free_sudoku(new_sudoku);
    sudoku[i] = -sudoku[i];
    return 0;}

void generate(int *sudoku, int difficulty) {
    int a, b, left = 81;
    if (difficulty == 0)
        a = 46;
    else if (difficulty == 1)
        a = 51;
    else if (difficulty == 2)
        a = 56;
    else
        a = 81;
    while (left > 0 && a > 0) {
        b = rand() % left;
        if (delete_num(sudoku, b))
            a--;
        left--;}}

void scan_sudoku(int *sudoku) {
    int i;
    char a;
    for (i = 0; i < 81; i++) {
        fscanf(in, "%c", &a);
        if (a - '0' < 0 || '9' - a < 0) {
            i--;
            continue;}
        sudoku[i] = a - '0';}}

int main(void) {
    srand(time(NULL) * 1234567890);
    rand();
    out = fopen("sudoku.txt", "w");
    int *sudoku = init_sudoku();
    int i;
    int difficulty;
    for (i = 1; i < 1001; i++) {
        fprintf(out, "Grid %04d\n", i);
        printf("%d\n", i);
        init_grid(sudoku);
        random_grid(sudoku);
        difficulty = rand() % 4;
        generate(sudoku, difficulty);
        normalize(sudoku);
        print_sudoku(sudoku);}
    free_sudoku(sudoku);
    fclose(out);
    return 0;}


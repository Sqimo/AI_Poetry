#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

FILE *in, *out;

void set(int grid[9][9][10]) {
    int i, j, k;
    for(i = 0; i < 9; i++)
        for(j = 0; j < 9; j++) {
            grid[i][j][0] = 0;
            for(k = 1; k < 10; k++)
                grid[i][j][k] = 1;
        }
}

void write(int grid[9][9][10]) {
    int i, j;
    for(i = 0; i < 9; i++) {
        for(j = 0; j < 9; j++)
            //fprintf(out, "%d ", grid[i][j][0]);

            if (grid[i][j][0] != 0)
                fprintf(out, "%d ", grid[i][j][0]);
            else {
                int l;
                fprintf(out, "(");
                fprintf(out, "%d; ", grid[i][j][0]);
                for(l = 1; l < 10; l++)
                    if (grid[i][j][l] != 0)
                        fprintf(out, "%d,", l);
                fprintf(out, ") ");
            }

        fprintf(out, "\n");
    }
    fprintf(out, "\n");
}

//проверка решенности
int solved(int grid[9][9][10]) {
    int x, y, i, j;
    for(i = 0; i < 9; i++)
        for(j = 0; j < 9; j++)
            if(grid[i][j][0] == 0)
                return 0;

    //нет ли повторений
    for(x = 0; x < 9; x++)
        for(y = 0; y < 9; y++) {
            for(j = 0; j < 9; j++)
                if(j != y)
                    if(grid[x][y][0] == grid[x][j][0])
                        return 0;
            for(i = 0; i < 9; i++)
                if(i != x)
                    if(grid[x][y][0] == grid[i][y][0])
                        return 0;
            for (i = (x / 3) * 3; i < (x / 3) * 3 + 3; i++)
                for (j = (y / 3) * 3; j < (y / 3) * 3 + 3; j++)
                    if(i != x || j != y)
                        if(grid[x][y][0] == grid[i][j][0])
                            return 0;
        }

    return 1;
}


//готовность и обновление клетки
int cell(int x, int y, int grid[9][9][10]) {
    int k, s = 0, n = 0;
    //s - количество оставшихся цифр, n - отдельный кандидат
    for(k = 1; k < 10; k++)
        if (grid[x][y][k] == 1) {
            s += 1;
            n = k;
        }

    if (s == 1) {
        grid[x][y][n] = 0;
        grid[x][y][0] = n;
        return 1;
    }
    else
        return 0;
}

void update(int x, int y, int n, int grid[9][9][10]) {
    int i, j;

    //горизонталь
    for(j = 0; j < 9; j++)
        if (grid[x][j][n] == 1) {
            grid[x][j][n] = 0;
            if (cell(x, j, grid) == 1)
                update(x, j, grid[x][j][0], grid);
        }

    //вертикаль
    for(i = 0; i < 9; i++)
        if (grid[i][y][n] == 1) {
            grid[i][y][n] = 0;
            if (cell(i, y, grid) == 1)
                update(i, y, grid[i][y][0], grid);
        }

    //блок
    for (i = (x / 3) * 3; i < (x / 3) * 3 + 3; i++)
        for (j = (y / 3) * 3; j < (y / 3) * 3 + 3; j++)
            if (grid[i][j][n] == 1) {
                grid[i][j][n] = 0;
                if (cell(i, j, grid) == 1)
                    update(i, j, grid[i][j][0], grid);
            }
}


int try_to_solve(int*** arr)
{
    int i, j, k, grind[9][9][10];

    set(grind);
    for(i = 0; i < 9; i++)
        for(j = 0; j < 9; j++) {
            for(k = 0; k < 10; k++) {
                grind[i][j][k] = arr[i][j][k];
            }
            if (grind[i][j][0] != 0) {
                for(k = 1; k < 10; k++)
                    grind[i][j][k] = 0;
                update(i, j, grind[i][j][0], grind);
            }
        }



    if (solved(grind) == 1)
    {
        return 1;
    }

    else
        return 0;
}


int ***init_grid(void) {
    int ***arr = malloc(sizeof(*arr) * 9);
    int i, j, k;
    for (i = 0; i < 9; i++) {
        arr[i] = malloc(sizeof(**arr) * 9);
        for (j = 0; j < 9; j++)
            arr[i][j] = malloc(sizeof(***arr) * 10);
    }
    for (i = 0; i < 9; i++){
        for (j = 0; j < 9; j++) {
            if (i < 3) {
                arr[i][j][0] = (j + 3 * i) % 9 + 1;
                for (k = 1; k < 10; k++)
                    arr[i][j][k] = 1;
            }
            else if (i < 6) {
                arr[i][j][0] = (j + 3 * i + 1) % 9 + 1;
                for (k = 1; k < 10; k++)
                    arr[i][j][k] = 1;
            }
            else {
                arr[i][j][0] = (j + 3 * i + 2) % 9 + 1;
                for (k = 1; k < 10; k++)
                    arr[i][j][k] = 1;
            }
        }
    }
    return arr;
}

void transposing(int ***arr) {
    int i, j;
    int *str;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (j > i) {
                str = arr[i][j];
                arr[i][j] = arr[j][i];
                arr[j][i] = str;
            }
        }
    }
}

void swap_strings(int ***arr) {
    int **str;
    int block = rand() % 3;
    int s1 = rand() % 3;
    int s2 = rand() % 2;
    if (s2 < s1) {
        str = arr[block * 3 + s1];
        arr[block * 3 + s1] = arr[block * 3 + s2];
        arr[block * 3 + s2] = str;
    }
    else {
        str = arr[block * 3 + s1];
        arr[block * 3 + s1] = arr[block * 3 + s2 + 1];
        arr[block * 3 + s2 + 1] = str;
    }
}

void swap_collumns(int ***arr) {
    int i;
    int *col;
    int block = rand() % 3;
    int c1 = rand() % 3;
    int c2 = rand() % 2;
    if (c2 < c1) {
        for (i = 0; i < 9; i++) {
            col = arr[i][block * 3 + c1];
            arr[i][block * 3 + c1] = arr[i][block * 3 + c2];
            arr[i][block * 3 + c2] = col;
        }
    }
    else {
        for (i = 0; i < 9; i++) {
            col = arr[i][block * 3 + c1];
            arr[i][block * 3 + c1] = arr[i][block * 3 + c2 + 1];
            arr[i][block * 3 + c2 + 1] = col;
        }
    }
}

void swap_hor_blocks(int ***arr) {
    int i;
    int **col;
    int block1 = rand() % 3;
    int block2 = rand() % 2;
    if (block2 < block1) {
        for (i = 0; i < 3; i++) {
            col = arr[block1 * 3 + i];
            arr[block1 * 3 + i] = arr[block2 * 3 + i];
            arr[block2 * 3 + i] = col;
        }
    }
    else {
        for (i = 0; i < 3; i++) {
            col = arr[block1 * 3 + i];
            arr[block1 * 3 + i] = arr[block2 * 3 + i + 3];
            arr[block2 * 3 + i + 3] = col;
        }
    }
}

void swap_vert_blocks(int ***arr) {
    int i, j;
    int *col;
    int block1 = rand() % 3;
    int block2 = rand() % 2;
    if (block2 < block1) {
        for (i = 0; i < 9; i++) {
            for (j = 0; j < 3; j++) {
                col = arr[i][block1 * 3 + j];
                arr[i][block1 * 3 + j] = arr[i][block2 * 3 + j];
                arr[i][block2 * 3 + j] = col;
            }
        }
    }
    else {
        for (i = 0; i < 9; i++) {
            for (j = 0; j < 3; j++) {
                col = arr[i][block1 * 3 + j];
                arr[i][block1 * 3 + j] = arr[i][block2 * 3 + j + 3];
                arr[i][block2 * 3 + j + 3] = col;
            }
        }
    }
}

void random_grid(int ***arr) {
    int i, a;
    for (i = 0; i < 60; i++) {
        a = rand() % 5;
        if (a == 0)
            transposing(arr);
        else if (a == 1)
            swap_strings(arr);
        else if (a == 2)
            swap_collumns(arr);
        else if (a == 3)
            swap_hor_blocks(arr);
        else
            swap_vert_blocks(arr);
    }
}

void print_grid(int ***arr) {
    int i, j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            fprintf(in, "%d", arr[i][j][0]);
        }
        fprintf(in, "\n");
    }
}

void clear_grid(int ***arr) {
    int i, j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++)
            free(arr[i][j]);
    }
    for (i = 0; i < 9; i++)
        free(arr[i]);
    free(arr);
}

int ***copy_grid(int ***arr_copy) {
    int ***arr = malloc(sizeof(*arr) * 9);
    int i, j, k;
    for (i = 0; i < 9; i++) {
        arr[i] = malloc(sizeof(**arr) * 9);
        for (j = 0; j < 9; j++)
            arr[i][j] = malloc(sizeof(***arr) * 10);
    }
    for (i = 0; i < 9; i++){
        for (j = 0; j < 9; j++) {
            arr[i][j][0] = arr_copy[i][j][0];
            for (k = 1; k < 10; k++)
                arr[i][j][k] = 1;
        }
    }
    return arr;
}

void normalize(int ***arr) {
    int i, j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (arr[i][j][0] < 0) {
                arr[i][j][0] *= -1;
            }
        }
    }
}

int delete_num(int ***arr, int n) {
    int i, j;
    i = n;
    int ***new_arr;
    j = 0;
    while (i >= 0) {
        if (arr[j / 9][j % 9][0] <= 0)
            j++;
        else {
            if (i == 0)
                break;
            else {
                i--;
                j++;
            }
        }
    }
    new_arr = copy_grid(arr);
    new_arr[j / 9][j % 9][0] = 0;
    normalize(new_arr);
    if (try_to_solve(new_arr) == 1) {
        clear_grid(new_arr);
        arr[j / 9][j % 9][0] = 0;
        return 1;
    }
    clear_grid(new_arr);
    arr[j / 9][j % 9][0] *= -1;
    return 0;
}

int delete_elems(int ***arr, int dif) {
    int a, b;
    int left = 81;
    if (dif == 0)
        a = 46;
    else if (dif == 1)
        a = 51;
    else
        a = 56;
    while (left > 0 && a > 0) {
        b = rand() % left;
        if (delete_num(arr, b))
            a--;
        left--;
    }
}



int main() {
    int grid[9][9][10];

    //первый элемент 0, если цифра не определена, иначе записывается сама цифра
    //k-й равен 1, если еще есть возможность цифру k в клетку, иначе 0

    in  = fopen("grids.txt", "rb+");
    out = fopen("grids_solved.txt", "w");

    int i, j, k;

    srand(time(NULL) * 1234567890);
    int ***arr;
    int dif;
    for (i = 0; i < 100; i ++) {
        dif = rand() % 3;
        arr = init_grid();
        random_grid(arr);
        delete_elems(arr, dif);
        normalize(arr);
        fprintf(in, "Grid %03d\n", i+1);
        print_grid(arr);
        clear_grid(arr);
    }

    fseek(in, 0L, SEEK_SET);
    char line[1024];
    while((fgets(line, 1024, in)) != NULL) {

        //начальная позиция
        set(grid);

        //чтение
        for(i = 0; i < 9; i++) {
            fgets(line, 1024, in);
            for(j = 0; j < 9; j++)
                if (line[j] - '0' != 0) {
                    grid[i][j][0] = line[j] - '0';
                    for(k = 1; k < 10; k++)
                        grid[i][j][k] = 0;
                    update(i, j, grid[i][j][0], grid);
            }
        }


        if(solved(grid) == 1)
            write(grid);
        else {
            fprintf(out, "UNSOLVED:\n");
            write(grid);
        }

    }

    fclose(in);
    fclose(out);

    return 0;
}


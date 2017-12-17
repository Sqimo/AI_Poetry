#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

FILE *in, *out;

//инициализация поля
void set(int grid[9][9][10]) {
    int i, j, k;
    for(i = 0; i < 9; i++)
        for(j = 0; j < 9; j++) {
            grid[i][j][0] = 0;
            for(k = 1; k < 10; k++)
                grid[i][j][k] = 1;
        }
}

//вывод поля
void write(int grid[9][9][10]) {
    int i, j;
    for(i = 0; i < 9; i++) {
        for(j = 0; j < 9; j++)
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
    //проверка заполненности
    for(i = 0; i < 9; i++)
        for(j = 0; j < 9; j++)
            if(grid[i][j][0] == 0)
                return 0;

    //проверка корректности
    for(x = 0; x < 9; x++)
        for(y = 0; y < 9; y++) {
            //строки
            for(j = 0; j < 9; j++)
                if(j != y)
                    if(grid[x][y][0] == grid[x][j][0])
                        return 2;
            //столбцы
            for(i = 0; i < 9; i++)
                if(i != x)
                    if(grid[x][y][0] == grid[i][y][0])
                        return 2;
            //блоки
            for (i = (x / 3) * 3; i < (x / 3) * 3 + 3; i++)
                for (j = (y / 3) * 3; j < (y / 3) * 3 + 3; j++)
                    if(i != x || j != y)
                        if(grid[x][y][0] == grid[i][j][0])
                            return 2;
        }

    return 1;
}


//доопределение ячейки, если остается только одно возможное число
int set_cell(int x, int y, int grid[9][9][10]) {
    int k, s = 0, n = 0;
    //s - количество, n - кандидат
    for(k = 1; k < 10; k++)
        if (grid[x][y][k] == 1) {
            if (s == 1)
                return 0;
            s += 1;
            n = k;
        }

    grid[x][y][n] = 0;
    grid[x][y][0] = n;
    return 1;
}

//обновление поля при записи в клетку (x, y) числа n
void update(int x, int y, int n, int grid[9][9][10]) {
    int i, j;

    //строки
    for(j = 0; j < 9; j++)
        if (grid[x][j][n] == 1) {
            grid[x][j][n] = 0;
            if (set_cell(x, j, grid) == 1)
                update(x, j, grid[x][j][0], grid);
        }

    //столбцы
    for(i = 0; i < 9; i++)
        if (grid[i][y][n] == 1) {
            grid[i][y][n] = 0;
            if (set_cell(i, y, grid) == 1)
                update(i, y, grid[i][y][0], grid);
        }

    //блоки
    for (i = (x / 3) * 3; i < (x / 3) * 3 + 3; i++)
        for (j = (y / 3) * 3; j < (y / 3) * 3 + 3; j++)
            if (grid[i][j][n] == 1) {
                grid[i][j][n] = 0;
                if (set_cell(i, j, grid) == 1)
                    update(i, j, grid[i][j][0], grid);
            }
}


guess(int grid[9][9][10]) {
    int i, j, k, l;
    int low = 10, nx = 0, ny = 0, s;
    int grid_copy[9][9][10];

    //находим клетку с наименьшим количеством вариантов
    for(i = 0; i < 9; i++)
        for(j = 0; j < 9; j++)
            if (grid[i][j][0] == 0) {
                s = 0;
                for (k = 1; k < 10; k++)
                    if (grid[i][j][k] != 0)
                        s += 1;
                if (s < low) {
                    low = s;
                    nx = i;
                    ny = j;
                }
            }

    //пробуем подставлять разные значения в найденную клетку
    for(k = 1; k < 10; k++)
        if (grid[nx][ny][k] != 0) {
            memcpy(grid_copy, grid, sizeof(grid_copy));
            grid_copy[nx][ny][0] = k;
            for(l = 1; l < 10; l++)
                grid_copy[nx][ny][l] = 0;
            update(nx, ny, k, grid_copy);

            //продолжаем угадывать
            if (solved(grid_copy) == 0)
                guess(grid_copy);

            //угадали
            if (solved(grid_copy) == 1) {
                memcpy(grid, grid_copy, sizeof(grid_copy));
                break;
            }
        }

}

int main() {
    float start_time =  clock();

    //0-й элемент равен записанной в ячейке цифре (по дефлоту 0)
    //в i-й (i > 0) ячейке 1, если i возможно поставить, иначе 0 (по дефлоту 1)
    int grid[9][9][10];

    in  = fopen("grids.txt", "r");
    out = fopen("grids_solved.txt", "w");

    int i, j, k;

    char line[1024];
    while((fgets(line, 1024, in)) != NULL) {

        set(grid);

        //чтение и обновление поля
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

        //попытка угадать
        if(solved(grid) != 1)
            guess(grid);

        //последння проверка
        if(solved(grid) != 1)
            fprintf(out, "UNSOLVED:\n");

        write(grid);

    }

    float end_time =  clock();
    fprintf(out, "time: %.3f s\n", (end_time - start_time)/1000.0);

    fclose(in);
    fclose(out);

    return 0;
}


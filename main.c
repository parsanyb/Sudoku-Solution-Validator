#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9


int sudoku[SIZE][SIZE] = {
    {5, 3, 4, 6, 7, 8, 9, 1, 2},
    {6, 7, 2, 1, 9, 5, 3, 4, 8},
    {1, 9, 8, 3, 4, 2, 5, 6, 7},
    {8, 5, 9, 7, 6, 1, 4, 2, 3},
    {4, 2, 6, 8, 5, 3, 7, 9, 1},
    {7, 1, 3, 9, 2, 4, 8, 5, 6},
    {9, 6, 1, 5, 3, 7, 2, 8, 4},
    {2, 8, 7, 4, 1, 9, 6, 3, 5},
    {3, 4, 5, 2, 8, 6, 1, 7, 9}
};


int valid[11] = {0};


typedef struct {
    int row;
    int col;
    int index;
} parameters;


void *check_rows(void *arg) {
    for (int i = 0; i < SIZE; i++) {
        int nums[SIZE + 1] = {0};
        for (int j = 0; j < SIZE; j++) {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || nums[num] == 1) {
                pthread_exit(NULL);
            } else {
                nums[num] = 1;
            }
        }
    }
    valid[0] = 1;
    pthread_exit(NULL);
}


void *check_columns(void *arg) {
    for (int j = 0; j < SIZE; j++) {
        int nums[SIZE + 1] = {0};
        for (int i = 0; i < SIZE; i++) {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || nums[num] == 1) {
                pthread_exit(NULL);
            } else {
                nums[num] = 1;
            }
        }
    }
    valid[1] = 1;
    pthread_exit(NULL);
}


void *check_subgrid(void *param) {
    parameters *data = (parameters *)param;
    int row = data->row;
    int col = data->col;
    int nums[SIZE + 1] = {0};
    for (int i = row; i < row + 3; i++) {
        for (int j = col; j < col + 3; j++) {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || nums[num] == 1) {
                free(data);
                pthread_exit(NULL);
            } else {
                nums[num] = 1;
            }
        }
    }
    valid[data->index] = 1;
    free(data);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[11];

    // ایجاد نخ برای سطرها
    pthread_create(&threads[0], NULL, check_rows, NULL);

    // ایجاد نخ برای ستون‌ها
    pthread_create(&threads[1], NULL, check_columns, NULL);

   
    int index = 2;
    for (int i = 0; i < SIZE; i += 3) {
        for (int j = 0; j < SIZE; j += 3) {
            parameters *data = (parameters *) malloc(sizeof(parameters));
            data->row = i;
            data->col = j;
            data->index = index;
            pthread_create(&threads[index], NULL, check_subgrid, data);
            index++;
        }
    }


    for (int i = 0; i < 11; i++) {
        pthread_join(threads[i], NULL);
    }


    int isValid = 1;
    for (int i = 0; i < 11; i++) {
        if (valid[i] == 0) {
            isValid = 0;
            break;
        }
    }

    if (isValid) {
        printf("Sudoku solution is valid.\n");
    } else {
        printf("Sudoku solution is invalid.\n");
    }

    return 0;
}

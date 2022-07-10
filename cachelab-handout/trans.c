/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

void trans(int M, int N, int A[N][M], int B[M][N]);

char transpose_61_67_desc[] = "Transpose 61 67";

void transpose_61_67(int M, int N, int A[N][M], int B[M][N]) {
    for (int i = 0; i < N; i += 16) {
        for (int j = 0; j < M; j += 16) {
            for (int k = i; k < i + 16 && k < N; k++) {
                for (int l = j; l < j + 16 && l < M; l++) {
                    B[l][k] = A[k][l];
                }
            }
        }
    }
}

char transpose_32_32_desc[] = "Transpose 32 32";

void transpose_32_32(int M, int N, int A[N][M], int B[M][N]) {
    int v1, v2, v3, v4, v5, v6, v7, v8;
    for (int i = 0; i < N; i += 8) {
        for (int j = 0; j < M; j += 8) {
            for (int k = i; k < i + 8 && k < N; k++) {
                v1 = A[k][j];
                v2 = A[k][j + 1];
                v3 = A[k][j + 2];
                v4 = A[k][j + 3];
                v5 = A[k][j + 4];
                v6 = A[k][j + 5];
                v7 = A[k][j + 6];
                v8 = A[k][j + 7];
                B[j][k] = v1;
                B[j + 1][k] = v2;
                B[j + 2][k] = v3;
                B[j + 3][k] = v4;
                B[j + 4][k] = v5;
                B[j + 5][k] = v6;
                B[j + 6][k] = v7;
                B[j + 7][k] = v8;
            }
        }
    }
}

char transpose_64_64_desc[] = "Transpose 64 64";
void transpose_64_64(int M, int N, int A[N][M], int B[M][N]) {
    int i, j, x, y;
    int x1, x2, x3, x4, x5, x6, x7, x8;
    for (i = 0; i < N; i += 8)
        for (j = 0; j < M; j += 8) {
            for (x = i; x < i + 4; ++x) {
                x1 = A[x][j];
                x2 = A[x][j + 1];
                x3 = A[x][j + 2];
                x4 = A[x][j + 3];
                x5 = A[x][j + 4];
                x6 = A[x][j + 5];
                x7 = A[x][j + 6];
                x8 = A[x][j + 7];

                B[j][x] = x1;
                B[j + 1][x] = x2;
                B[j + 2][x] = x3;
                B[j + 3][x] = x4;
                B[j][x + 4] = x5;
                B[j + 1][x + 4] = x6;
                B[j + 2][x + 4] = x7;
                B[j + 3][x + 4] = x8;
            }
            for (y = j; y < j + 4; ++y) {
                x1 = A[i + 4][y];
                x2 = A[i + 5][y];
                x3 = A[i + 6][y];
                x4 = A[i + 7][y];
                x5 = B[y][i + 4];
                x6 = B[y][i + 5];
                x7 = B[y][i + 6];
                x8 = B[y][i + 7];

                B[y][i + 4] = x1;
                B[y][i + 5] = x2;
                B[y][i + 6] = x3;
                B[y][i + 7] = x4;
                B[y + 4][i] = x5;
                B[y + 4][i + 1] = x6;
                B[y + 4][i + 2] = x7;
                B[y + 4][i + 3] = x8;
            }
            for (x = i + 4; x < i + 8; ++x) {
                x1 = A[x][j + 4];
                x2 = A[x][j + 5];
                x3 = A[x][j + 6];
                x4 = A[x][j + 7];
                B[j + 4][x] = x1;
                B[j + 5][x] = x2;
                B[j + 6][x] = x3;
                B[j + 7][x] = x4;
            }
        }
}

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";

void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {
    // our code
    if (M == 32 && N == 32) {
        transpose_32_32(M, N, A, B);
    } else if (M == 61 && N == 67) {
        transpose_61_67(M, N, A, B);
    } else if (M == 64 && N == 64) {
        transpose_64_64(M, N, A, B);
    } else {
        trans(M, N, A, B);
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";

void trans(int M, int N, int A[N][M], int B[M][N]) {
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions() {
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
//    registerTransFunction(trans, trans_desc);
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N]) {
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}


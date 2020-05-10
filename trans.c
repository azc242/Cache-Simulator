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
#include "lab3.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);
void transpose_32(int M, int N, int A[N][M], int B[M][N]);
void transpose_64t(int M, int N, int A[N][M], int B[M][N]);
// void transpose_64(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    if(M == 32 && N == 32){
        transpose_32(M, N, A, B);
        return;
    }

    if(M == 64 && N == 64){
        transpose_64t(M, N, A, B);
        return;
    }

}

void transpose_32(int M, int N, int A[N][M], int B[M][N]){

    int diagonalElement;
    int diagonalIndex;
    
    for(int col = 0; col < M; col += 8){
        for(int row = 0; row < N; row += 8){

            for(int rBlock = row; rBlock < row + 8; ++rBlock){
                for(int cBlock = col; cBlock < col + 8; ++cBlock){
                    if(rBlock != cBlock){
                        B[cBlock][rBlock] = A[rBlock][cBlock];
                    }
                    else{
                        diagonalElement = A[rBlock][cBlock];
                        diagonalIndex = rBlock;
                    }
                }
                if(col == row){
                    B[diagonalIndex][diagonalIndex] = diagonalElement;
                }
            }

        }
    }
}

// /* Uses row-major order and the locality principles to access contiguous array indeces */
// void transpose_64(int M, int N, int A[N][M], int B[M][N]){

//     // temp variables to set and refresh after each block iteration
//     int temp, temp1, temp2, temp3, temp4, temp5, temp6, temp7

//     for(int col = 0; col < M; col += 8){
//         for(int row = 0; row < N; row += 8){
//             /* int p acts as a placeholder, and the 8 temps are for accessing
//             contiguous array elements to take advantage of spatial and temporal locality in
//             accessing elements near each other at the same time */
//             for(int p = row; p < row + 4; ++p){

//             }
//             for(int p = col; p < col + 8; ++p){

//             }

//         }
//     }
// }


void transpose_64t(int M, int N, int A[N][M], int B[M][N]){

    int diagonalElement;
    int diagonalIndex;
    
    for(int col = 0; col < M; col += 4){
        for(int row = 0; row < N; row += 4){

            for(int rBlock = row; rBlock < row + 4; ++rBlock){
                for(int cBlock = col; cBlock < col + 4; ++cBlock){
                    if(rBlock != cBlock){
                        B[cBlock][rBlock] = A[rBlock][cBlock];
                    }
                    else{
                        diagonalElement = A[rBlock][cBlock];
                        diagonalIndex = rBlock;
                    }
                }
                if(col == row){
                    B[diagonalIndex][diagonalIndex] = diagonalElement;
                }
            }

        }
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
void trans(int M, int N, int A[N][M], int B[M][N])
{
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
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

    // registerTransFunction(transpose_32_32, transpose_32_32_desc);

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
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


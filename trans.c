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
void transpose_64(int M, int N, int A[N][M], int B[M][N]);

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
        transpose_64_64(M, N, A, B);
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


void transpose_64(int M, int N, int A[N][M], int B[M][N]){
    int blockSize = 8;
	int gap = 4;
	int row, col;	

	for (col = 0; col < M; col += blockSize){ // 0-63
		for (row = 0; row < N; row += blockSize){ // 0-63
			
			int nextX = col;
			int nextY = row;
            
            nextY += blockSize; // increments by 8
            /* once our y var is equal to the matrix edge increment x var
            and go to a new row and reset the Y var */
            if (nextY >= N){ 
                nextX += blockSize;
                nextY -= N;
            }
            
            while (col == nextY){
				nextY += blockSize;
				if (nextY >= N){
					nextX += blockSize;
					nextY -= N;
				}
			}

            // will create the next block 
			int nextX_2 = nextX;
			int nextY_2 = nextY;

            nextY_2 += blockSize;
            if (nextY_2 >= N){
                nextX_2 += blockSize;
                nextY_2 -= N;				
            }

			while (col == nextY_2){
				nextY_2 += blockSize;
				if (nextY_2 >= N){
					nextX_2 += blockSize;
					nextY_2 -= N;				
				}
			}

			if (nextX >= M){
                // moves 8x8 block from A to B, with no miss
				for (int i = 0; i < blockSize; i++){
					for (int j = 0; j < blockSize; j++){
						B[col + j][row + i] = A[row + i][col + j];	
                    }
                }		
			} 
            else {
                // moves the upper 4x8 block between matrices
				for (int i = 0; i < gap; i++){
					for (int j = 0; j < blockSize; j++){
						B[nextX + i][nextY + j] = A[row + i][col + j];	
                    }
                }		
				
                // moves lower 4x8 block between matrices
				for (int i = 0; i < gap; i++){
					for (int j = 0; j < blockSize; j++){
						B[nextX_2 + i][nextY_2 + j] = A[row + gap + i][col + j];	
                    }
                }	

                // transposes the lead 4x4 to the top of the 4x8 destination
				for (int i = 0; i < gap; i++){
					for (int j = 0; j < gap; j++){
						B[col + j][row + i] = B[nextX + i][nextY + j];
						B[col + j][row + gap + i] = B[nextX_2 + i][nextY_2 + j];	
					}
                }
				// transposes trailing 4x4 to the bottom of the remaining 4x8 of the total 8x8 block
				for (int i = 0; i < gap; i++){
					for (int j = 0; j < gap; j++){
						B[col + gap + j][row + i] = B[nextX + i][nextY + gap + j];
						B[col + gap + j][row + gap + i] = B[nextX_2 + i][nextY_2 + gap + j];	
					}			
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


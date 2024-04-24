#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MATRIX_SIZE 3
#define NUM_THREADS 3

int matrixA[MATRIX_SIZE][MATRIX_SIZE];
int matrixB[MATRIX_SIZE][MATRIX_SIZE];
int resultMatrix[MATRIX_SIZE][MATRIX_SIZE];
int calculationCount = 0; // Global variable to keep track of calculation count
pthread_mutex_t countMutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for protecting calculationCo>


typedef struct {
    int row;
    int col;
} thread_data_t;

void *multiply(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int localCount = 0; // Local count for each thread

    for (int i = 0; i < MATRIX_SIZE; i++) {
        resultMatrix[data->row][data->col] += matrixA[data->row][i] * matrixB[i][data->col];
        localCount++;
    }

    // Lock mutex before updating global calculationCount
    pthread_mutex_lock(&countMutex);
    calculationCount += localCount; // Update global count
    pthread_mutex_unlock(&countMutex); // Unlock mutex
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];

    // Initialize matrices A and B
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrixA[i][j] = rand() % 10;
            matrixB[i][j] = rand() % 10;
        }
    }

    // Initialize resultMatrix to zero
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            resultMatrix[i][j] = 0;
        }
    }
    // Create threads to compute the result matrix
    int thread_count = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            thread_data[thread_count].row = i;
            thread_data[thread_count].col = j;
            pthread_create(&threads[thread_count], NULL, multiply, (void *)&thread_data[thread_count]);
            thread_count++;

            if (thread_count >= NUM_THREADS)
                thread_count = 0;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    calculationCount = NUM_THREADS * MATRIX_SIZE * MATRIX_SIZE;


    // Print the result matrix
    printf("Result Matrix:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", resultMatrix[i][j]);
        }
        printf("\n");
    }
   printf("Number of completed calculations: %d\n", calculationCount);
    
    return 0;
}

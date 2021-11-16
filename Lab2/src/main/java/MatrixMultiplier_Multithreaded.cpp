#include <stdlib.h>
#include <jni.h>
#include "MatrixMultiplier.h"
#include <pthread.h>
#include <time.h>

struct DataStruct
{
    int n;
    int row;
    double **x;
    double **y;
    double **res;

    DataStruct(int n, int row, double **x, double **y, double **res)
    {
        this->n = n;
        this->row = row;
        this->x = x;
        this->y = y;
        this->res = res;
    }
};

int matA[MAX][MAX];
int matB[MAX][MAX];
int matC[MAX][MAX];
int step_i = 0;
int MAX = 200;

JNIEXPORT void JNICALL
Java_MatrixMultiplier_matmultithreaded(jint n, jobjectArray matA, jobjectArray matB, jobjectArray matC)
{
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            matA[i][j] = rand() % 10;
            matB[i][j] = rand() % 10;
        }
    }

    // Displaying matA
    cout << endl
         << "Matrix A" << endl;
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
            cout << matA[i][j] << " ";
        cout << endl;
    }

    // Displaying matB
    cout << endl
         << "Matrix B" << endl;
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
            cout << matB[i][j] << " ";
        cout << endl;
    }

    // declaring four threads
    pthread_t threads[MAX_THREAD];

    // Creating four threads, each evaluating its own part
    for (int i = 0; i < MAX_THREAD; i++)
    {
        int *p;
        pthread_create(&threads[i], NULL, multi, (void *)(p));
    }

    // joining and waiting for all threads to complete
    for (int i = 0; i < MAX_THREAD; i++)
        pthread_join(threads[i], NULL);

    // Displaying the result matrix
    cout << endl
         << "Multiplication of A and B" << endl;
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
            cout << matC[i][j] << " ";
        cout << endl;
    }
    return 0;
}

void *multi(void *arg)
{
    int i = step_i++; //i denotes row number of resultant matC

    for (int j = 0; j < MAX; j++)
        for (int k = 0; k < MAX; k++)
            matC[i][j] += matA[i][k] * matB[k][j];
}

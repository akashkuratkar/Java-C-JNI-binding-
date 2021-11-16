#include "HelloWorldJNI.h"
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <bits/stdc++.h>
#include <jni.h>
#include <stdlib.h>

/* jobjectArray JNICALL Java_HelloWorldJNI_sayHello(JNIEnv *env, jobject thisObject)
{
    //std::string hello = "Hello from C++ !!";
    //std::cout << hello << std::endl;

    size_t i, j, k;
    int n = 20;
    int *res = new int[n][n];
    int x[n][n];
    int y[n][n];
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            for (k = 0; k < n; k++)
            {
                res[i][j] += x[i][k] * y[k][j];
            }
        }
    }
    //std::string hello = "Hello from C++ !!" + std::to_string(res[0][0]);
    //std::cout << hello << std::endl;
    return res;
}
JNIEXPORT jobjectArray Java_HelloWorldJNI_matmulti(JNIEnv *env, jobject thisObject, jint n, jobjectArray *A, jobjectArray *B)
{

    jobjectArray retC;
    int i, j, k;
    double sum;
    int localAm;
    localAm = n;
    //jobjectArray localArrayCopyB = env->NewObjectArray(n, int, A); //(jobjectArray)env->GetObjectArrayElement(B, n);
    //jobjectArray localArrayCopyA = (jobjectArray)env->GetObjectArrayElement(A, n);
    // jobjectArray localArrayCopyC = (jobjectArray)env->GetObjectArrayElement(A, n);
    double localArrayCopyB[localAm][localAm];
    double localArrayCopyA[localAm][localAm];
    double localArrayCopyC[localAm][localAm];

    for (i = 0; i < localAm; i++)
    {
        jintArray oneDimA =
            (jintArray)env->GetObjectArrayElement(A, i);

        jint *elementA = env->GetIntArrayElements(oneDimA, 0);
        for (j = 0; j < localAm; j++)
        {

            localArrayCopyA[i][j] = elementA[j];
        }

        for (i = 0; i < localAm; i++)
        {
            jintArray oneDimB =
                (jintArray)env->GetObjectArrayElement(B, i);

            jint *elementB = env->GetIntArrayElements(oneDimB, 0);
            for (j = 0; j < localAm; j++)
            {

                localArrayCopyB[i][j] = elementB[j];
            }
        }
        for (i = 0; i < localAm; i++)
        {
            for (j = 0; j < localAm; j++)
            {
                for (k = 0; k < localAm; k++)
                {
                    sum += localArrayCopyB[i][k] * localArrayCopyA[k][j];
                }
                localArrayCopyc[i][j] = sum;
                // std::cout << localArrayCopyC[i][j] << std::endl;
            }
        }
        // env->SetObjectArrayRegion(retC, 0, n, localArrayCopyC);
        return localArrayCopyC;
    }

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

    void multi(void *args)
    {
        size_t i, j, k;
        struct DataStruct *data = (struct DataStruct *)args;
        i = data->row;

        for (j = 0; j < data->n; j++)
            for (k = 0; k < data->n; k++)
                data->res[i][j] += data->x[i][k] * data->y[k][j];
    }

    /*extern "C" void matmultithreaded(int n, double **x, double **y, double **res)
{
    size_t i, j, k;

    pthread_t threads[n];
    struct DataStruct *data[n];
    for (i = 0; i < n; i++)
    {
        data[i] = new DataStruct(n, i, x, y, res);
        pthread_create(&threads[i], NULL, multi, (void *)data[i]);
    }
    // joining and waiting for all threads to complete
    for (i = 0; i < n; i++)
        pthread_join(threads[i], NULL);
}*/
extern "C"

    JNIEXPORT jobjectArray JNICALL
    Java_HelloWorldJNI_matmulti(JNIEnv *env, jobject obj, jobjectArray a, jobjectArray b)
{

    jint size = (*env)->GetArrayLength(env, a);

    // READ MATRIX B (JVM object) TO BUFFER (native C array)
    int k;
    jintArray *b_sub = malloc(size * sizeof(jintArray *)); // Subarray objects of B,
                                                           // stored for cleaning up.
    jint **buf_b = malloc(size * sizeof(jint *));          // Buffer for matrix B.
    for (k = 0; k < size; k++)
    {
        b_sub[k] = (*env)->GetObjectArrayElement(env, b, k);         // Fetch subarray of B.
        buf_b[k] = (*env)->GetIntArrayElements(env, b_sub[k], NULL); // Read subarray to buffer.
    }

    // CREATE RESULT ARRAY
    jclass intArrCls = (*env)->FindClass(env, "[I"); // Find int[] class.
    if (intArrCls == NULL)
        return NULL; /* exception thrown */
    jobjectArray result = (*env)->NewObjectArray(env, size, intArrCls, NULL);
    // Create array of int[].
    if (result == NULL)
        return NULL;                        /* out of memory error thrown */
    (*env)->DeleteLocalRef(env, intArrCls); // Delete reference to int[] class.

    // MULTIPLY MATRICES
    int i, j;
    jint *buf_res_row = (jint *)malloc(size * sizeof(jint)); // Buffer for final matrix.

    for (i = 0; i < size; i++)
    {
        // READ I-TH ROW FROM A TO BUFFER
        jintArray a_i = (*env)->GetObjectArrayElement(env, a, i);
        jint *buf_a_i = (*env)->GetIntArrayElements(env, a_i, NULL);

        // CALCULATE I-TH ROW OF RESULT
        for (j = 0; j < size; j++)
        {
            buf_res_row[j] = 0;
            for (k = 0; k < size; k++)
            {
                buf_res_row[j] += buf_a_i[k] * buf_b[k][j];
            }
        }

        (*env)->ReleaseIntArrayElements(env, a_i, buf_a_i, 0);
        (*env)->DeleteLocalRef(env, a_i);

        // COPY I-TH ROW TO RESULT OBJECT
        jintArray res_row = (*env)->NewIntArray(env, size);
        if (res_row == NULL)
            return NULL;                                               /* out of memory error thrown */
        (*env)->SetIntArrayRegion(env, res_row, 0, size, buf_res_row); // Firstly copy to new
        (*env)->SetObjectArrayElement(env, result, i, res_row);        // array object, and
        (*env)->DeleteLocalRef(env, res_row);                          // then to matrix.
    }

    // CLEAN UP
    free(buf_res_row);
    for (k = 0; k < size; k++)
    {
        (*env)->ReleaseIntArrayElements(env, b_sub[k], buf_b[k], 0);
        (*env)->DeleteLocalRef(env, b_sub[k]);
    }
    free(buf_b);
    free(b_sub);

    return result;
}
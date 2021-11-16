#include <stdlib.h>
#include <jni.h>
#include "MatrixMultiplier.h"
#include <pthread.h>
#include <time.h>
#import <stdio.h>

static JavaVM *jvm;
struct arg_struct
{
    jint size;
    jint i;
    jobjectArray matrix1;
    jobjectArray matrix2;
    jobjectArray matrix3;
    jint **result;
    jint **buf_b;
    JNIEnv env;

} * args;

void *print_the_arguments(void *arguments)
{
    struct arg_struct *args = arguments;
    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, (void *)&env, NULL);
    jint *buf_a_i = (jint *)malloc(args->size * sizeof(jint));
    jint *buf_res_row = (jint *)malloc(args->size * sizeof(jint));
    jintArray a_i = (*env)->GetObjectArrayElement(env, args->matrix1, args->i);
    (*env)->GetIntArrayRegion(env, a_i, 0, args->size, buf_a_i);
    (*env)->DeleteLocalRef(env, a_i);

    // CALCULATE I-TH ROW OF RESULT
    int j, k;
    for (j = 0; j < args->size; j++)
    {
        buf_res_row[j] = 0;
        for (k = 0; k < args->size; k++)
        {
            buf_res_row[j] += buf_a_i[k] * args->buf_b[k][j];
        }
    }

    // COPY I-TH ROW TO RESULT OBJECT
    jintArray res_row = (*env)->NewIntArray(env, args->size);
    if (res_row == NULL)
        return NULL;                                                     /* out of memory error thrown */
    (*env)->SetIntArrayRegion(env, res_row, 0, args->size, buf_res_row); // firstly copy to new
    (*env)->SetObjectArrayElement(env, args->matrix3, args->i, res_row); // array object, and
    (*env)->DeleteLocalRef(env, res_row);
    free(buf_res_row);
    free(buf_a_i);

    return NULL;
}

JNIEXPORT void JNICALL Java_MatrixMultiplier_matmultithreaded(JNIEnv *env, jobject object, jint n, jobjectArray a, jobjectArray b, jobjectArray c)
{
    pthread_t some_thread;
    args = malloc(sizeof(struct arg_struct) * 1);

    args->size = n;

    int k;
    jint **buf_b = malloc(args->size * sizeof(jint *));
    for (k = 0; k < args->size; k++)
    { // copy b[k] to buffer
        buf_b[k] = (jint *)malloc(args->size * sizeof(jint));
        jintArray b_k = (*env)->GetObjectArrayElement(env, b, k);     // fetch b[k] aka b_k
        (*env)->GetIntArrayRegion(env, b_k, 0, args->size, buf_b[k]); // copy b_k to buffer
        (*env)->DeleteLocalRef(env, b_k);                             // delete references b_k
    }

    args->buf_b = buf_b;
    //pthread_t threads[args->size];
    pthread_t *threads = malloc(args->size * sizeof(pthread_t));
    // args->matrix1 = malloc(args->size * sizeof(jint));
    //args->matrix2 = malloc(args->size * sizeof(jint));
    //args->matrix3 = malloc(args->size * sizeof(jint));
    args->matrix1 = a;
    args->matrix2 = b;
    args->matrix3 = c;
    size_t i;
    for (i = 0; i < args->size; i++)
    {
        args = malloc(sizeof(struct arg_struct) * 1);

        args->i = i;
        pthread_create(&threads[i], NULL, &print_the_arguments, args);
    }

    for (i = 0; i < args->size; i++)
        pthread_join(threads[i], NULL);
    pthread_exit(NULL);
}
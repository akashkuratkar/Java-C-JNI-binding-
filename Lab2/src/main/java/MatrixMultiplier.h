/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class MatrixMultiplier */

#ifndef _Included_MatrixMultiplier
#define _Included_MatrixMultiplier
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     MatrixMultiplier
 * Method:    multiply
 * Signature: ([[I[[I)[[I
 */
JNIEXPORT jobjectArray JNICALL Java_MatrixMultiplier_multiply
  (JNIEnv *, jobject, jobjectArray, jobjectArray);

/*
 * Class:     MatrixMultiplier
 * Method:    matmultithreaded
 * Signature: (I[[I[[I[[I)V
 */
JNIEXPORT void JNICALL Java_MatrixMultiplier_matmultithreaded
  (JNIEnv *, jobject, jint, jobjectArray, jobjectArray, jobjectArray);

#ifdef __cplusplus
}
#endif
#endif

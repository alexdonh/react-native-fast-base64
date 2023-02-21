#include <jni.h>
#include "react-native-fast-base64.h"

extern "C"
JNIEXPORT jint JNICALL
Java_com_fastbase64_FastBase64Module_nativeMultiply(JNIEnv *env, jclass type, jdouble a, jdouble b) {
    return fastbase64::multiply(a, b);
}

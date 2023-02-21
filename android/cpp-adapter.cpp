#include <jni.h>
#include "react-native-fast-base64.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_fastbase64_FastBase64Module_nativeInstall(JNIEnv *env, jobject thiz, jlong jsiPtr) {
    auto runtime = reinterpret_cast<facebook::jsi::Runtime *>(jsiPtr);
    if (runtime) {
        fastbase64::install(*runtime);
    }
}

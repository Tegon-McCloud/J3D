#include "j3d_Window.h"

#include "JNIUtils.h"
#include "Window.h"
#include "Keyboard.h"
#include "Mouse.h"

#include <iostream>
#include <cassert>

JNIEXPORT jlong JNICALL Java_j3d_Window_init(JNIEnv* env, jobject jthis, jint width, jint height, jstring title) {
    JNI_TRY(
        jboolean isCopy;
        const char* utf_title = env->GetStringUTFChars(title, &isCopy);

        Window* pWindow = new Window(width, height, std::string(utf_title));

        env->ReleaseStringUTFChars(title, utf_title);
        
        return reinterpret_cast<jlong>(pWindow);
    )
}

JNIEXPORT jlong JNICALL Java_j3d_Window_getMouseHandle(JNIEnv* env, jobject jthis) {
    JNI_TRY(
        Window* pWindow = getNative<Window>(env, jthis);
        return reinterpret_cast<jlong>(pWindow->getMouse());
    )
}
JNIEXPORT jlong JNICALL Java_j3d_Window_getKeyboardHandle(JNIEnv* env, jobject jthis) {
    JNI_TRY(
        Window* pWindow = getNative<Window>(env, jthis);
        return reinterpret_cast<jlong>(pWindow->getKeyboard());
    )
}
JNIEXPORT jlong JNICALL Java_j3d_Window_getGraphicsHandle(JNIEnv* env, jobject jthis) {
    JNI_TRY(
        Window* pWindow = getNative<Window>(env, jthis);
        return reinterpret_cast<jlong>(pWindow->getGraphics());
    )
}

JNIEXPORT void JNICALL Java_j3d_Window_close(JNIEnv* env, jobject jthis) {
    JNI_TRY_VOID(
        delete getNative<Window>(env, jthis);
    )
}

JNIEXPORT jboolean JNICALL Java_j3d_Window_poll(JNIEnv* env, jclass) {
    JNI_TRY(
        return Window::poll().has_value();
    )
}

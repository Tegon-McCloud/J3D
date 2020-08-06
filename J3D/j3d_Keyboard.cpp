#include "j3d_Keyboard.h"

#include "JNIUtils.h"
#include "Keyboard.h"

JNIEXPORT jboolean JNICALL Java_j3d_Keyboard_isKeyDown(JNIEnv* env, jobject jthis, jbyte key) {
    JNI_TRY(
        Keyboard* pKeyboard = getNative<Keyboard>(env, jthis);
        return pKeyboard->isKeyDown(key);
    )
}

JNIEXPORT jobject JNICALL Java_j3d_Keyboard_nextEvent(JNIEnv* env, jobject jthis) {
    JNI_TRY(
        Keyboard* pKeyboard = getNative<Keyboard>(env, jthis);
        auto keyEvent = pKeyboard->readKey();

        if (keyEvent.has_value()) {
            jclass cls = getClass(env, "j3d/KeyEvent");
            jmethodID constructor = getMethod(env, cls, "<init>", "(BZ)V");
            jobject jkeyEvent = env->NewObject(cls, constructor, keyEvent->getKey(), keyEvent->isPress());
            if (!jkeyEvent) {
                jniThrowByName(env, "java/lang/Error", "Failed to instantiate class.");
            }
            return jkeyEvent;
        } else {
            return nullptr;
        }
    )
}

JNIEXPORT void JNICALL Java_j3d_Keyboard_flushEvents(JNIEnv* env, jobject jthis) {
    JNI_TRY_VOID(
        Keyboard* pKeyboard = getNative<Keyboard>(env, jthis);
        pKeyboard->flush();
    ) 
}
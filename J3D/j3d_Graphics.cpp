#include "j3d_Graphics.h"

#include "JNIUtils.h"
#include "Graphics.h"

JNIEXPORT jlong JNICALL Java_j3d_Graphics_getSceneHandle(JNIEnv* env, jobject jthis) {
    Graphics* pGraphics = getNative<Graphics>(env, jthis);
    return reinterpret_cast<jlong>(pGraphics->getScene());

}

JNIEXPORT void JNICALL Java_j3d_Graphics_render(JNIEnv* env, jobject jthis) {
    Graphics* pGraphics = getNative<Graphics>(env, jthis);
    pGraphics->render();
}

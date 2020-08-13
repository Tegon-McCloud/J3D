#include "j3d_Graphics.h"

#include "JNIUtils.h"
#include "Graphics.h"

JNIEXPORT void JNICALL Java_j3d_Graphics_render(JNIEnv* env, jobject jthis) {
    Graphics* pGraphics = getNative<Graphics>(env, jthis);
    pGraphics->render();
}

JNIEXPORT void JNICALL Java_j3d_Graphics_setScene(JNIEnv* env, jobject jthis, jobject jscene) {
    Graphics* pGraphics = getNative<Graphics>(env, jthis);
    Scene* pScene = getNative<Scene>(env, jscene);
    pGraphics->setScene(pScene);
}
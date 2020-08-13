#include "j3d_Scene.h"
#include "JNIUtils.h"
#include "Graphics.h"
#include "Scene.h"

JNIEXPORT jlong JNICALL Java_j3d_Scene_init(JNIEnv* env, jobject jthis, jobject jgfx, jstring jfile) {
	Graphics* pGfx = getNative<Graphics>(env, jgfx);

	jboolean isCopy;
	const char* utfFile = env->GetStringUTFChars(jfile, &isCopy);
	std::string file(utfFile);
	env->ReleaseStringUTFChars(jfile, utfFile);

	return reinterpret_cast<jlong>(new Scene(*pGfx, file));
}

JNIEXPORT void JNICALL Java_j3d_Scene_close(JNIEnv* env, jobject jthis) {
	Scene* pScene = getNative<Scene>(env, jthis);
	delete pScene;
}
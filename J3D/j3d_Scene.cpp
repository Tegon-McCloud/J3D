#include "j3d_Scene.h"
#include "JNIUtils.h"
#include "Graphics.h"
#include "Scene.h"

#include <filesystem>
#include <iostream>

JNIEXPORT jlong JNICALL Java_j3d_Scene_init(JNIEnv* env, jobject jthis, jobject jgfx, jstring jfile) {
	Graphics* pGfx = getNative<Graphics>(env, jgfx);

	jboolean isCopy;
	const char* utfFile = env->GetStringUTFChars(jfile, &isCopy);
	std::string file(utfFile);
	env->ReleaseStringUTFChars(jfile, utfFile);

	std::filesystem::path path(file);
	if (!std::filesystem::exists(path)) {
		std::cerr << "Error: file does not exist: " << path.c_str() << std::endl;
	}

	return reinterpret_cast<jlong>(new Scene(*pGfx, file));
}

JNIEXPORT void JNICALL Java_j3d_Scene_close(JNIEnv* env, jobject jthis) {
	Scene* pScene = getNative<Scene>(env, jthis);
	delete pScene;
}
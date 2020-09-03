#include "j3d_Scene.h"
#include "JNIUtils.h"
#include "Graphics.h"
#include "Scene.h"

#include <filesystem>
#include <iostream>

JNIEXPORT jlong JNICALL Java_j3d_Scene_init(JNIEnv* env, jobject jthis, jobject jgfx, jstring jfile) {
	JNI_TRY(

		Graphics* pGfx = getNative<Graphics>(env, jgfx);

		jboolean isCopy;
		const char* utfFile = env->GetStringUTFChars(jfile, &isCopy);
		std::string file(utfFile);
		env->ReleaseStringUTFChars(jfile, utfFile);

		std::filesystem::path path(file);
		if (!std::filesystem::exists(path)) {
			std::cerr << "Error: file does not exist: " << path.c_str() << std::endl;
		}
		try {
			return reinterpret_cast<jlong>(new Scene(*pGfx, file));
		} catch (std::runtime_error& e) {
			jniThrowByName(env, "java/io/FileNotFoundException", e.what());
			return NULL;
		}
		
	)

}

JNIEXPORT void JNICALL Java_j3d_Scene_close(JNIEnv* env, jobject jthis) {
	Scene* pScene = getNative<Scene>(env, jthis);
	delete pScene;
}

JNIEXPORT jlong JNICALL Java_j3d_Scene_getRootNodeHandle(JNIEnv* env, jobject jthis) {
	Scene* pScene = getNative<Scene>(env, jthis);
	return reinterpret_cast<jlong>(pScene->getRoot());
}

JNIEXPORT jlong JNICALL Java_j3d_Scene_getNodeHandle__Ljava_lang_String_2(JNIEnv* env, jobject jthis, jstring jname) {
	Scene* pScene = getNative<Scene>(env, jthis);

	jboolean isCopy;
	const char* utfName = env->GetStringUTFChars(jname, &isCopy);
	std::string name(utfName);
	SceneNode* pNode = pScene->getNode(name);
	env->ReleaseStringUTFChars(jname, utfName);
	
	return reinterpret_cast<jlong>(pNode);
}

JNIEXPORT jlong JNICALL Java_j3d_Scene_getNodeHandle__I(JNIEnv* env, jobject jthis, jint jindex) {
	Scene* pScene = getNative<Scene>(env, jthis);
	return reinterpret_cast<jlong>(pScene->getNode(jindex));
}

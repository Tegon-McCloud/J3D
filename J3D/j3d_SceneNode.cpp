#include "j3d_SceneNode.h"

#include "JNIUtils.h"
#include "DXUtils.h"
#include "Scene.h"

using namespace DirectX;

JNIEXPORT void JNICALL Java_j3d_SceneNode_rotateX(JNIEnv* env, jobject jthis, jfloat jangle) {
	SceneNode* pNode = getNative<SceneNode>(env, jthis);
	pNode->transform(XMMatrixRotationX(jangle));
}

JNIEXPORT void JNICALL Java_j3d_SceneNode_rotateY(JNIEnv* env, jobject jthis, jfloat jangle) {
	SceneNode* pNode = getNative<SceneNode>(env, jthis);
	pNode->transform(XMMatrixRotationY(jangle));
}

JNIEXPORT void JNICALL Java_j3d_SceneNode_rotateZ(JNIEnv* env, jobject jthis, jfloat jangle) {
	SceneNode* pNode = getNative<SceneNode>(env, jthis);
	pNode->transform(XMMatrixRotationZ(jangle));
}

JNIEXPORT void JNICALL Java_j3d_SceneNode_translate(JNIEnv* env, jobject jthis, jfloat x, jfloat y, jfloat z) {
	SceneNode* pNode = getNative<SceneNode>(env, jthis);
	pNode->transform(XMMatrixTranslation(x, y, z));
}

JNIEXPORT void JNICALL Java_j3d_SceneNode_scale(JNIEnv* env, jobject jthis, jfloat x, jfloat y, jfloat z) {
	SceneNode* pNode = getNative<SceneNode>(env, jthis);
	pNode->transform(XMMatrixScaling(x, y, z));
}
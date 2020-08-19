#include "j3d_SceneNode.h"

#include "JNIUtils.h"
#include "DXUtils.h"
#include "Scene.h"

using namespace DirectX;

JNIEXPORT void JNICALL Java_j3d_SceneNode_rotateY(JNIEnv* env, jobject jthis, jfloat jangle) {
	SceneNode* pNode = getNative<SceneNode>(env, jthis);
	pNode->transform(XMMatrixRotationY(jangle));
}

JNIEXPORT void JNICALL Java_j3d_SceneNode_rotateZ(JNIEnv* env, jobject jthis, jfloat jangle) {
	SceneNode* pNode = getNative<SceneNode>(env, jthis);
	pNode->transform(XMMatrixRotationZ(jangle));
}
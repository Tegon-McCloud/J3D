/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class j3d_Keyboard */

#ifndef _Included_j3d_Keyboard
#define _Included_j3d_Keyboard
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     j3d_Keyboard
 * Method:    isKeyDown
 * Signature: (B)Z
 */
JNIEXPORT jboolean JNICALL Java_j3d_Keyboard_isKeyDown
  (JNIEnv *, jobject, jbyte);

/*
 * Class:     j3d_Keyboard
 * Method:    nextEvent
 * Signature: ()Lj3d/KeyEvent;
 */
JNIEXPORT jobject JNICALL Java_j3d_Keyboard_nextEvent
  (JNIEnv *, jobject);

/*
 * Class:     j3d_Keyboard
 * Method:    flushEvents
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_j3d_Keyboard_flushEvents
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif

#pragma once

#pragma warning(push, 0)
#include <jni.h>
#pragma warning(pop)

#include <stdexcept>

#define JNI_TRY(code) try { \
	code \
} catch (JNIError&) { \
	return NULL; \
}

#define JNI_TRY_VOID(code) try { \
	code \
} catch (JNIError&) {}

class JNIError : std::runtime_error {
public:
	JNIError(const std::string& msg = "Java exception thrown.") : std::runtime_error(msg) {}
};

inline void jniThrowByName(JNIEnv* env, const std::string& exceptionName, const std::string& msg) {
	jclass exClass;

	exClass = env->FindClass(exceptionName.c_str());
	if (!exClass) {
		jniThrowByName(env, "java/lang/NoClassDefFoundError", "Tried to throw exception " + exceptionName + " but failed to get its class.");
	}

	if (env->ThrowNew(exClass, msg.c_str()) == JNI_OK) {
		throw JNIError();
	} else {
		throw std::runtime_error("Failed to throw java exception");
	}
}

inline jclass getClass(JNIEnv* env, jobject obj) {
	jclass cls = env->GetObjectClass(obj);
	if (!cls) {
		jniThrowByName(env, "java/lang/NoClassDefFoundError", "Failed to get class of object.");
	}

	return cls;
}

inline jclass getClass(JNIEnv* env, const std::string& className) {
	jclass cls = env->FindClass(className.c_str());
	if (!cls) {
		jniThrowByName(env, "java/lang/NoClassDefFoundError", "Failed to get class with name: " + className + ".");
	}

	return cls;
}

inline jmethodID getMethod(JNIEnv* env, jclass cls, const std::string& methodName, const std::string& methodSignature) {
	jmethodID mid = env->GetMethodID(cls, methodName.c_str(), methodSignature.c_str());
	if (!mid) {
		jniThrowByName(env, "java/lang/NoSuchMethodError", "Failed to get method with name: " + methodName + " and signature: " + methodSignature + ".");
	}

	return mid;
}

inline jfieldID getField(JNIEnv* env, jclass cls, const std::string& fieldName, const std::string& fieldSignature) {
	jfieldID fid = env->GetFieldID(cls, fieldName.c_str(), fieldSignature.c_str());
	if (!fid) {
		jniThrowByName(env, "java/lang/NoSuchFieldError", "Failed to get field with name: " + fieldName + " and signature: " + fieldSignature + ".");
	}

	return fid;
}

template<class T>
inline T* getNative(JNIEnv* env, jobject jobj, const std::string& fieldName = "nativeHandle") {

	jclass cls = getClass(env, jobj);
	jfieldID fid = getField(env, cls, fieldName, "J");

	return reinterpret_cast<T*>(env->GetLongField(jobj, fid));
}


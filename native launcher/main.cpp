

#pragma warning(push, 0)
#include <jni.h>
#pragma warning(pop)

#pragma comment(lib, "jvm.lib")

#include <stdexcept>

JavaVM* jvm;
JNIEnv* env;


void createJVM() {

	char optionStrings[1][256] = {
#ifdef _DEBUG
		"-Djava.class.path=..\\x64\\Debug\\J3D.jar"
#else
		"-Djava.class.path=..\\x64\\Release\\J3D.jar"
#endif
	};

	JavaVMOption jvmOptions[1];

	jvmOptions[0].optionString = optionStrings[0];

	JavaVMInitArgs jvmArgs;
	jvmArgs.version = JNI_VERSION_9;
	jvmArgs.nOptions = 1;
	jvmArgs.options = jvmOptions;
	jvmArgs.ignoreUnrecognized = JNI_FALSE;

	jint result = JNI_CreateJavaVM(&jvm, reinterpret_cast<void**>(&env), &jvmArgs);

	if (result == JNI_ERR) {
		throw std::runtime_error("JVM initialization failed");
	}

}

void destroyJVM() {
	jvm->DestroyJavaVM();
}

int main() {
	createJVM();

	jclass cls = env->FindClass("j3d/Window");
	if (cls == nullptr) {
		throw std::runtime_error("Could not find class j3d/Window");
	}

	jmethodID test_mid = env->GetStaticMethodID(cls, "test", "()V");
	if (test_mid == nullptr) {
		throw std::runtime_error("Failed to get method id");
	}

	env->CallStaticVoidMethod(cls, test_mid);

	destroyJVM();
	return 0;

}

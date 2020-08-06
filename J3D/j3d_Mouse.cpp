#include "j3d_Mouse.h"

#include "JNIUtils.h"
#include "Mouse.h"

#include <unordered_map>
#include <string>

JNIEXPORT jobject JNICALL Java_j3d_Mouse_nextEvent(JNIEnv* env, jobject jthis) {

    static const std::unordered_map<Mouse::Event::Type, const std::string> enumToJEnum = {
        { Mouse::Event::Type::LPress, "L_PRESS" },
        { Mouse::Event::Type::LRelease, "L_RELEASE" },
        { Mouse::Event::Type::RPress, "R_PRESS" },
        { Mouse::Event::Type::RRelease, "R_RELEASE" },
        { Mouse::Event::Type::WheelUp, "WHEEL_UP" },
        { Mouse::Event::Type::WheelDown, "WHEEL_DOWN" },
        { Mouse::Event::Type::Move, "MOVE" },
    };

    JNI_TRY(
        Mouse* pMouse = getNative<Mouse>(env, jthis);

        auto mouseEvent = pMouse->nextEvent();

        if (mouseEvent.has_value()) {
            jclass cls = getClass(env, "j3d/MouseEvent");
            jmethodID ctor = getMethod(env, cls, "<init>", "(Lj3d/MouseEventType;IIZZ)V");

            jclass eventTypeCls = getClass(env, "j3d/MouseEventType");
            jfieldID typeFid = env->GetStaticFieldID(eventTypeCls, enumToJEnum.at(mouseEvent->getType()).c_str(), "Lj3d/MouseEventType;");
            if (!typeFid) {
                jniThrowByName(env, "java/lang/NoSuchFieldError", "Failed to get field.");
            }
            jobject type = env->GetStaticObjectField(eventTypeCls, typeFid);

            jobject jmouseEvent = env->NewObject(
                cls,
                ctor,
                type,
                mouseEvent->getX(),
                mouseEvent->getY(),
                mouseEvent->leftIsPressed(), 
                mouseEvent->rightIsPressed()
            );
            if (!jmouseEvent) {
                jniThrowByName(env, "java/lang/Error", "Failed to instantiate class.");
            }

            return jmouseEvent;

        } else {
            return nullptr;
        }
    )
}

JNIEXPORT void JNICALL Java_j3d_Mouse_flushEvents(JNIEnv* env, jobject jthis) {
    JNI_TRY_VOID(
        Mouse* pMouse = getNative<Mouse>(env, jthis);
        pMouse->flush();
    )
}

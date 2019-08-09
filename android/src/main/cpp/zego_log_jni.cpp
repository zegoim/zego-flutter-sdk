#include <jni.h>
#include <string>
#include "../../../../zego_defines.h"

std::string jstring2string(JNIEnv* env, jstring jstr)
{
    jclass strClass = env->FindClass("java/lang/String");
    jstring encoding = env->NewStringUTF("UTF-8");
    jmethodID methodId = env->GetMethodID(strClass, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray byteArr = (jbyteArray)env->CallObjectMethod(jstr, methodId, encoding); // String .getByte("UTF-8");
    jsize arrLength = env->GetArrayLength(byteArr); //获取长度
    jbyte* bytePtr = env->GetByteArrayElements(byteArr, JNI_FALSE); // jbyteArray 转为 jbyte*

    std::string str{(char *)bytePtr, (size_t)arrLength};

    env->ReleaseByteArrayElements(byteArr, bytePtr, 0);  //释放掉
    env->DeleteLocalRef(byteArr);
    env->DeleteLocalRef(encoding);
    env->DeleteLocalRef(strClass);

    return str;
}

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_zego_zegoliveroomplugin_ZegoLogJNI_logNotice(JNIEnv *env, jclass, jstring content) {

    std::string strContent = jstring2string(env, content);
    return ZegoExternalLogWithNotice(strContent.c_str());
}

#ifdef __cplusplus
}
#endif


#ifndef JNIHELPER_H
#define JNIHELPER_H

#include <jni.h>
#include <string>

struct JniMethodInfo
{
    JNIEnv*     env;
    jclass      classID;
    jmethodID   methodID;
};

class JniHelper
{
public:
    static void setJavaVM(JavaVM *javaVM);
    static JavaVM* getJavaVM();
    static JNIEnv* getEnv();

    static void detach(void* env);

    static bool setClassLoaderFrom(jobject activityInstance);
    
    static bool getStaticMethodInfo(JniMethodInfo &methodinfo,
                                    const char *className,
                                    const char *methodName,
                                    const char *paramCode);
                                    
    static bool getMethodInfo(JniMethodInfo &methodinfo,
                              const char *className,
                              const char *methodName,
                              const char *paramCode);

    static bool getStaticMethodInfoEx(JNIEnv* env,
                                      JniMethodInfo &methodinfo,
                                      jclass classID, 
                                      const char *methodName,
                                      const char *paramCode);
                                      
    static bool getMethodInfoEx(JNIEnv* env,
                                JniMethodInfo &methodinfo,
                                jclass classID, 
                                const char *methodName,
                                const char *paramCode);
                              
    static std::string jstring2string(jstring str);
    static jstring char2jstring(const char* str);
    static jstring wchar2jstring(const wchar_t* wstr);

    static jmethodID loadclassMethod_methodID;
    static jobject classloader;

private:
    static JNIEnv* cacheEnv();

    static bool getMethodInfo_DefaultClassLoader(JniMethodInfo &methodinfo,
                                                 const char *className,
                                                 const char *methodName,
                                                 const char *paramCode);

    static JavaVM* _psJavaVM;
};

#endif //JNIHELPER_H

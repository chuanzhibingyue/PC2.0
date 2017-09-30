#include "JniHelper.h"
#include <pthread.h>
#include "ConsolePrint.h"

static pthread_key_t g_key;

jclass _getClassID(const char *className)
{
    if (NULL == className)
    {
        return NULL;
    }

    JNIEnv* env = JniHelper::getEnv();

    jstring _jstrClassName = env->NewStringUTF(className);

    jclass _clazz = (jclass) env->CallObjectMethod(JniHelper::classloader,
                                                   JniHelper::loadclassMethod_methodID,
                                                   _jstrClassName);

    if (NULL == _clazz) 
    {
        LOGD("JNI-Classloader failed to find class of %s", className);
    }

    env->DeleteLocalRef(_jstrClassName);
        
    return _clazz;
}

JavaVM* JniHelper::_psJavaVM = NULL;
jmethodID JniHelper::loadclassMethod_methodID = NULL;
jobject JniHelper::classloader = NULL;

JavaVM* JniHelper::getJavaVM()
{
    return _psJavaVM;
}

void JniHelper::setJavaVM(JavaVM *javaVM) 
{
    pthread_t thisthread = pthread_self();
    LOGD("JNI-JniHelper::setJavaVM(%p), pthread_self() = %ld", javaVM, thisthread);
    _psJavaVM = javaVM;

    pthread_key_create(&g_key, NULL);
}

JNIEnv* JniHelper::cacheEnv() 
{
    JNIEnv* _env = NULL;
    // get jni environment
    jint ret = _psJavaVM->GetEnv((void**)&_env, JNI_VERSION_1_4);
        
    switch (ret)
    {
    case JNI_OK :
        // Success!
        pthread_setspecific(g_key, _env);
        return _env;        
    case JNI_EDETACHED :
        // Thread not attached
        
        // TODO : If calling AttachCurrentThread() on a native thread
        // must call DetachCurrentThread() in future.
        // see: http://developer.android.com/guide/practices/design/jni.html
                
        if (_psJavaVM->AttachCurrentThread(&_env, NULL) < 0)
        {
            LOGD("JNI-Failed to get the environment using AttachCurrentThread()");
            return NULL;
        }
        else
        {
            // Success : Attached and obtained JNIEnv!
            pthread_setspecific(g_key, _env);
            pthread_key_create(&g_key, JniHelper::detach);
            return _env;
        }
    case JNI_EVERSION :
        // Cannot recover from this error
        LOGD("JNI-JNI interface version 1.4 not supported");
    default :
        LOGD("JNI-Failed to get the environment using GetEnv()");
        return NULL;
    }
}

JNIEnv* JniHelper::getEnv()
{
    JNIEnv *_env = (JNIEnv *)pthread_getspecific(g_key);
    if (_env == NULL)
        _env = JniHelper::cacheEnv();
    return _env;
}

void JniHelper::detach(void* env)
{
    if(JNI_OK != _psJavaVM->DetachCurrentThread())
    {
        LOGD("JniHelper->detach DetachCurrentThread failed");
    }
}

bool JniHelper::setClassLoaderFrom(jobject activityinstance)
{
    JniMethodInfo _getclassloaderMethod;
    if (!JniHelper::getMethodInfo_DefaultClassLoader(_getclassloaderMethod,
                                                     "android/content/Context",
                                                     "getClassLoader",
                                                     "()Ljava/lang/ClassLoader;"))
    {
        return false;
    }

    jobject _c = JniHelper::getEnv()->CallObjectMethod(activityinstance,
                                                          _getclassloaderMethod.methodID);
    if (NULL == _c) 
    {
        return false;
    }

    JniMethodInfo _m;
    if (!JniHelper::getMethodInfo_DefaultClassLoader(_m,
                                                     "java/lang/ClassLoader",
                                                     "loadClass",
                                                     "(Ljava/lang/String;)Ljava/lang/Class;")) 
    {
        return false;
    }

    JniHelper::classloader = _c;
    JniHelper::loadclassMethod_methodID = _m.methodID;

    return true;
}

bool JniHelper::getStaticMethodInfo(JniMethodInfo &methodinfo,
                                      const char *className, 
                                      const char *methodName,
                                      const char *paramCode)
{
    if ((NULL == className) ||
        (NULL == methodName) ||
        (NULL == paramCode))
    {
        return false;
    }

    JNIEnv *pEnv = JniHelper::getEnv();
    if (!pEnv)
    {
        LOGD("JNI-Failed to get JNIEnv");
        return false;
    }
   
    jclass classID = _getClassID(className);
    if (! classID)
    {
        LOGD("JNI-Failed to find class %s", className);
        return false;
    }
   
    jmethodID methodID = pEnv->GetStaticMethodID(classID, methodName, paramCode);
    if (! methodID)
    {
        LOGD("JNI-Failed to find static method id of %s", methodName);
        return false;
    }
            
    methodinfo.classID = classID;
    methodinfo.env = pEnv;
    methodinfo.methodID = methodID;
    return true;
}

bool JniHelper::getStaticMethodInfoEx(JNIEnv* env,
                                        JniMethodInfo &methodinfo,
                                        jclass classID, 
                                        const char *methodName,
                                        const char *paramCode)
{
    if ((NULL == env) ||
        (NULL == classID) ||
        (NULL == methodName) ||
        (NULL == paramCode))
    {
        return false;
    }

    jmethodID methodID = env->GetStaticMethodID(classID, methodName, paramCode);
    if (! methodID)
    {
        LOGD("JNI-Failed to find static method id of %s", methodName);
        return false;
    }
            
    methodinfo.classID = classID;
    methodinfo.env = env;
    methodinfo.methodID = methodID;
    return true;
}

bool JniHelper::getMethodInfo_DefaultClassLoader(JniMethodInfo &methodinfo,
                                                   const char *className,
                                                   const char *methodName,
                                                   const char *paramCode) 
{
    if ((NULL == className) ||
        (NULL == methodName) ||
        (NULL == paramCode))
    {
        return false;
    }

    JNIEnv *pEnv = JniHelper::getEnv();
    if (!pEnv) 
    {
        return false;
    }

    jclass classID = pEnv->FindClass(className);
    if (! classID) 
    {
        LOGD("JNI-Failed to find class %s", className);
        return false;
    }

    jmethodID methodID = pEnv->GetMethodID(classID, methodName, paramCode);
    if (! methodID) 
    {
        LOGD("JNI-Failed to find method id of %s", methodName);
        return false;
    }

    methodinfo.classID = classID;
    methodinfo.env = pEnv;
    methodinfo.methodID = methodID;

    return true;
}

bool JniHelper::getMethodInfo(JniMethodInfo &methodinfo,
                                const char *className,
                                const char *methodName,
                                const char *paramCode) 
{
    if ((NULL == className) ||
        (NULL == methodName) ||
        (NULL == paramCode)) 
    {
    	LOGD("JNI-Failed paramer is null");
        return false;
    }

    JNIEnv *pEnv = JniHelper::getEnv();
    if (!pEnv) 
    {
    	LOGD("JNI-Failed getEnv is null");
        return false;
    }

    jclass classID = _getClassID(className);
    if (! classID) 
    {
        LOGD("JNI-Failed to find class %s", className);
        pEnv->ExceptionClear();
        return false;
    }

    jmethodID methodID = pEnv->GetMethodID(classID, methodName, paramCode);
    if (! methodID) 
    {
        LOGD("JNI-Failed to find method id of %s", methodName);
        pEnv->ExceptionClear();
        return false;
    }

    methodinfo.classID = classID;
    methodinfo.env = pEnv;
    methodinfo.methodID = methodID;

    return true;
}

bool JniHelper::getMethodInfoEx(JNIEnv* env,
                                  JniMethodInfo &methodinfo,
                                  jclass classID,
                                  const char *methodName,
                                  const char *paramCode) 
{
    if ((NULL == env) ||
        (NULL == classID) ||
        (NULL == methodName) ||
        (NULL == paramCode)) 
    {
    	LOGD("JNI-Failed paramer is null");
        return false;
    }

    jmethodID methodID = env->GetMethodID(classID, methodName, paramCode);
    if (! methodID) 
    {
        LOGD("JNI-Failed to find method id of %s", methodName);
        env->ExceptionClear();
        return false;
    }

    methodinfo.classID = classID;
    methodinfo.env = env;
    methodinfo.methodID = methodID;

    return true;
}

std::string JniHelper::jstring2string(jstring jstr)
{
    if (jstr == NULL) 
    {
        return "";
    }
        
    JNIEnv *pEnv = JniHelper::getEnv();
    if (!pEnv) 
    {
        return NULL;
    }

    const char* chars = pEnv->GetStringUTFChars(jstr, NULL);
    std::string ret(chars);
    pEnv->ReleaseStringUTFChars(jstr, chars);

    return ret;
}

jstring JniHelper::char2jstring(const char* str)
{  
    if (str == NULL) 
    {
        return NULL;
    }

    JNIEnv *pEnv = JniHelper::getEnv();
    if (!pEnv) 
    {
         return NULL;
    }
    return pEnv->NewStringUTF(str);
}

jstring JniHelper::wchar2jstring(const wchar_t* wstr)
{  
    if (wstr == NULL) 
    {
        return NULL;
    }

    JNIEnv *pEnv = JniHelper::getEnv();
    if (!pEnv) 
    {
         return NULL;
    }
    
    size_t len = wcslen(wstr);
    jchar* str2 = (jchar*)malloc(sizeof(jchar)*(len+1));   
    for (int i = 0; i < len; i++) 
        str2[i] = wstr[i];   
    str2[len] = 0;   
    jstring js = pEnv->NewString(str2, len);   
    free(str2);   
    return js;  
}

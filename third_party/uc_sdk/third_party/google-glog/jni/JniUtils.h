#ifndef JNIUTILS_H
#define JNIUTILS_H

#include <jni.h>
#include <string>
#include "ConsolePrint.h"

#ifdef __cplusplus
extern "C"
{
#endif

static jobject JniUtils_CreateArrayList(JNIEnv* env)
{
    LOGD("JniUtils_CreateArrayList Enter");

    jobject result;
    jclass contentvalues_class = env->FindClass("java/util/ArrayList");
    if(contentvalues_class)
    {
        LOGD("JniUtils_CreateArrayList FindClass(java/util/ArrayList) OK");

        jmethodID contentvalues_constructor_methodid = env->GetMethodID(contentvalues_class, "<init>", "()V");
        if (contentvalues_constructor_methodid)
        {

            LOGD("JniUtils_CreateArrayList GetMethodID OK");
            result = (jobject)env->NewObject(contentvalues_class, contentvalues_constructor_methodid);
            env->DeleteLocalRef(contentvalues_class);
            return result;
        }
        env->DeleteLocalRef(contentvalues_class);
    }
    return NULL;
}

static jobject JniUtils_CreateHashMap(JNIEnv* env)
{
    LOGD("JniUtils_CreateHashMap Enter");
    jobject result;
    jclass contentvalues_class = env->FindClass("java/util/HashMap");
    if(contentvalues_class)
    {
        LOGD("JniUtils_CreateHashMap FindClass(java/util/HashMap) OK");

        jmethodID contentvalues_constructor_methodid = env->GetMethodID(contentvalues_class, "<init>", "()V");
        if (contentvalues_constructor_methodid)
        {
            LOGD("JniUtils_CreateHashMap GetMethodID OK");
            result = (jobject)env->NewObject(contentvalues_class, contentvalues_constructor_methodid);
            env->DeleteLocalRef(contentvalues_class);
            return result;
        }
        env->DeleteLocalRef(contentvalues_class);
    }
    return NULL;
}

static jint JniUtils_AddIntItemToHashMap(JNIEnv* env, jobject jobjContent, const char* szKey, jint jnValue)
{
	LOGD("JniUtils_AddIntItemToHashMap start!");
	
    jclass contentvalues_class = env->GetObjectClass(jobjContent);
    if(contentvalues_class)
    {
        LOGD("JNI->JniUtils_AddIntItemToHashMap GetObjectClass OK");
        jmethodID contentvalues_put_methodid = env->GetMethodID(
            contentvalues_class, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
        
        env->DeleteLocalRef(contentvalues_class);
        if (contentvalues_put_methodid)
        {
            LOGD("JNI->JniUtils_AddIntItemToHashMap GetMethodID OK");
            
            int value = jnValue;
						char szValue[50];
						memset(szValue, 0, 50);
						sprintf(szValue, "%d", value);
            jstring jstrKey = env->NewStringUTF(szKey);
            jstring jstrValue = env->NewStringUTF(szValue);

            env->CallObjectMethod(jobjContent, contentvalues_put_methodid, jstrKey, jstrValue);
            env->DeleteLocalRef(jstrKey);
            env->DeleteLocalRef(jstrValue);
            
            LOGD("JniUtils_AddIntItemToHashMap end!");
            return 0;
        }
    }
    return 1;
}

static jint JniUtils_AddStringItemToHashMap(JNIEnv* env, jobject jobjContent, const char* szKey, const char* szValue)
{
	LOGD("JniUtils_AddStringItemToHashMap, szKey:%s, szValue:%s", szKey, szValue);
	
    jclass contentvalues_class = env->GetObjectClass(jobjContent);
    if(contentvalues_class)
    {
    		LOGD("env->GetObjectClass end");
    		
        jmethodID contentvalues_put_methodid = env->GetMethodID(
            contentvalues_class, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
        env->DeleteLocalRef(contentvalues_class);
        if (contentvalues_put_methodid)
        {
        		LOGD("env->GetMethodID end");
        		
            jstring jstrKey = env->NewStringUTF(szKey);
            jstring jstrValue = env->NewStringUTF(szValue);
            env->CallObjectMethod(jobjContent, contentvalues_put_methodid, jstrKey, jstrValue);
            env->DeleteLocalRef(jstrKey);
            env->DeleteLocalRef(jstrValue);
            
            LOGD("JniUtils_AddStringItemToHashMap end");
            return 0;
        }
    }
    return 1;
}

static jint JniUtils_AddObjectItemToHashMap(JNIEnv* env, jobject jobjContent, const char* szKey, jobject jobUserList)
{
	LOGD("JniUtils_AddObjectItemToHashMap start!");
	
    jclass contentvalues_class = env->GetObjectClass(jobjContent);
    if(contentvalues_class)
    {
        jmethodID contentvalues_put_methodid = env->GetMethodID(
            contentvalues_class, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
        env->DeleteLocalRef(contentvalues_class);
            
        if (contentvalues_put_methodid)
        {
        	LOGD("JniUtils_AddObjectItemToHashMap GetMethodID end");
            jstring jstrKey = env->NewStringUTF(szKey);
            env->CallObjectMethod(jobjContent, contentvalues_put_methodid, jstrKey, jobUserList);
            env->DeleteLocalRef(jstrKey);
            
            LOGD("JniUtils_AddObjectItemToHashMap end");
            return 0;
        }
    }
    return 1;
}

static jint JniUtils_AddBoolItemToHashMap(JNIEnv* env, jobject jobjContent, const char* szKey, jboolean jbValue)
{
	LOGD("JniUtils_AddBoolItemToHashMap start!");
	
    jclass contentvalues_class = env->GetObjectClass(jobjContent);
    if(contentvalues_class)
    {
        jmethodID contentvalues_put_methodid = env->GetMethodID(
            contentvalues_class, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
        env->DeleteLocalRef(contentvalues_class);
        if (contentvalues_put_methodid)
        {
        	LOGD("JniUtils_AddBoolItemToHashMap GetMethodID end");
            jstring jstrKey = env->NewStringUTF(szKey);
            jstring jstrValue;
            if (JNI_TRUE == jbValue)
            {
                jstrValue = env->NewStringUTF("1");
            }
            else
            {
                jstrValue = env->NewStringUTF("0");
            }

            env->CallBooleanMethod(jobjContent, contentvalues_put_methodid, jstrKey, jstrValue);
            env->DeleteLocalRef(jstrKey);
            env->DeleteLocalRef(jstrValue);
            
            LOGD("JniUtils_AddBoolItemToHashMap end");
            return 0;
        }
    }
    return 1;
}

static jint JniUtils_AddByteItemToHashMap(JNIEnv* env, jobject jobjContent, const char* szKey, jbyte jbtValue)
{
	LOGD("JniUtils_AddByteItemToHashMap start!");
	
	  unsigned char value = jbtValue;
	  char szValue[10];
		memset(szValue, 0, 10);
		sprintf(szValue, "%d", value);
		
		LOGD("JniUtils_AddByteItemToHashMap, szKey:%s, szValue:%s", szKey, szValue);
	
    jclass contentvalues_class = env->GetObjectClass(jobjContent);
    if(contentvalues_class)
    {
        jmethodID contentvalues_put_methodid = env->GetMethodID(
            contentvalues_class, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
        env->DeleteLocalRef(contentvalues_class);
        if (contentvalues_put_methodid)
        {
        	LOGD("JniUtils_AddByteItemToHashMap GetMethodID end!");
        	
            jstring jstrKey = env->NewStringUTF(szKey);
            jstring jstrValue = env->NewStringUTF(szValue);

            env->CallObjectMethod(jobjContent, contentvalues_put_methodid, jstrKey, jstrValue);
            env->DeleteLocalRef(jstrKey);
            env->DeleteLocalRef(jstrValue);
            
            LOGD("JniUtils_AddByteItemToHashMap end!");
            return 0;
        }
    }
    return 1;
}

static jint JniUtils_AddUserDataItemToArrayList(JNIEnv* env, jobject jobjContent, jobject jobUserData)
{
	LOGD("JniUtils_AddUserDataItemToArrayList start!");
	
    jclass contentvalues_class = env->GetObjectClass(jobjContent);
    if(contentvalues_class)
    {
        jmethodID contentvalues_put_methodid = env->GetMethodID(
            contentvalues_class, "add", "(Ljava/lang/Object;)Z");
        env->DeleteLocalRef(contentvalues_class);
        if (contentvalues_put_methodid)
        {
        	LOGD("JniUtils_AddUserDataItemToArrayList GetMethodID end!");
        	
            env->CallBooleanMethod(jobjContent, contentvalues_put_methodid, jobUserData);
            
            LOGD("JniUtils_AddUserDataItemToArrayList end!");
            return 0;
        }
    }
    return 1;
}

static jobject JniUtils_CreateNameValuePair(JNIEnv* env, jstring jstrKey, jstring jstrValue)
{
    jobject result;
    jclass namevaluepair_class = env->FindClass("org/apache/http/message/BasicNameValuePair");
    if(namevaluepair_class)
    {
        jmethodID namevaluepair_constructor_methodid = env->GetMethodID(
            namevaluepair_class, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");
        if (namevaluepair_constructor_methodid)
        {
            result = (jobject)env->NewObject(
                namevaluepair_class, namevaluepair_constructor_methodid, jstrKey, jstrValue);
            env->DeleteLocalRef(namevaluepair_class);
            return result;
        }
        env->DeleteLocalRef(namevaluepair_class);
    }
    return NULL;
}


static jint JniUtils_ArrayListAddNameValuePairItem(JNIEnv* env, jobject jobjArrayList, jstring jstrKey, jstring jstrValue)
{
    jclass arraylist_class = env->GetObjectClass(jobjArrayList);
    if(arraylist_class)
    {
        jobject namevaluepair = JniUtils_CreateNameValuePair(env, jstrKey, jstrValue);
        if(namevaluepair)
        {
            jmethodID arraylist_add_methodid = env->GetMethodID(
                arraylist_class, "add", "(Ljava/lang/Object;)Z");
            env->DeleteLocalRef(arraylist_class);
            if (arraylist_add_methodid)
            {
                env->CallBooleanMethod(jobjArrayList, arraylist_add_methodid, namevaluepair);
                env->DeleteLocalRef(namevaluepair);
                return 0;
            }        
        }
    }
    return 1;
}



#ifdef __cplusplus
}
#endif

#endif //NIUTILS_H

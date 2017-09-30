#include <jni.h>
#include "ConsolePrint.h"
#include "JniHelper.h"

//coredump start
#include <sys/time.h> 
#include <sys/resource.h>
//coredump end
#ifdef __cplusplus
extern "C"
{
#endif

    jint JNI_OnLoad(JavaVM *vm, void *reserved)
    {
	 //coredump start
        struct rlimit coredump;
        memset(&coredump, 0, sizeof(struct rlimit));
        coredump.rlim_cur = RLIM_INFINITY;
        coredump.rlim_max = RLIM_INFINITY;
        setrlimit(RLIMIT_CORE, &coredump);
        //coredump end

        JniHelper::setJavaVM(vm);
        JNIEnv* env = JniHelper::getEnv();

        //to do

        return JNI_VERSION_1_4;
    }

    void JNI_OnUnload(JavaVM* vm, void* reserved)
    {

    }

#ifdef __cplusplus
}
#endif

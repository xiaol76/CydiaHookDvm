//
// Created by NetEase on 17/5/23.
//
#include "jni.h"
#include "substrate.h"
#include <android/log.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>

#define TAG "HOOKTEST"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)


MSConfig(MSFilterLibrary, "libdvm.so");

bool (*_dvmLoadNativeCode)(char *pathName, void *classLoader, char **detail);

void (*_dvmUseJNIBridge)(void *method, void *func);

bool fake_dvmLoadNativeCode(char *soPath, void *classLoader, char **detail) {
    LOGD("fake_dvmLoadNativeCode soPath:%s", soPath);
    return _dvmLoadNativeCode(soPath, classLoader, detail);
}

void fake_dvmUseJNIBridge(void* method, void* func)
{
    LOGD("fake_dvmUseJNIBridge method.name: %s\tfunc:%.8X",*(char**)(method+16), func);
    _dvmUseJNIBridge(method,func);
}

//Substrate entry point
MSInitialize {
    LOGD("Substrate initialized.");
    MSImageRef image;
    image = MSGetImageByName("/system/lib/libdvm.so"); // 绝对路径
    if (image != NULL) {
        LOGD("dvm image: 0x%08X", (void *) image);

        void *dvmload = MSFindSymbol(image, "_Z17dvmLoadNativeCodePKcP6ObjectPPc");
        if (dvmload == NULL) {
            LOGD("error find dvmLoadNativeCode.");
        } else {
            MSHookFunction(dvmload, (void *) &fake_dvmLoadNativeCode, (void **) &_dvmLoadNativeCode);
            LOGD("hook dvmLoadNativeCode success.");
        }
        void *jniBridge = MSFindSymbol(image, "_Z15dvmUseJNIBridgeP6MethodPv");
        if (jniBridge == NULL) {
            LOGD("error find dvmUseJNIBridge");
        } else {
            MSHookFunction(jniBridge, (void *) &fake_dvmUseJNIBridge, (void **) &_dvmUseJNIBridge);
            LOGD("hook dvmUseJNIBridge success.");
        }
    } else {
        LOGD("can not find libdvm.");
    }
}

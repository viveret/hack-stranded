LOCAL_PATH := $(call my-dir)
NDK_PROJECT_PATH = $(call my-dir)

NDK_BUILD V=1


#include $(CLEAR_VARS)
#LOCAL_MODULE := Config-Plugin-prebuilt
#LOCAL_SRC_FILES := SE_Root/plugins/libseConfig-android.so
#include $(PREBUILT_SHARED_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := Console-Plugin-prebuilt
#LOCAL_SRC_FILES := SE_Root/plugins/libseConsole-android.so
#include $(PREBUILT_SHARED_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := Filesys-Plugin-prebuilt
#LOCAL_SRC_FILES := SE_Root/plugins/libseFilesys-android.so
#include $(PREBUILT_SHARED_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := Window-Plugin-prebuilt
#LOCAL_SRC_FILES := SE_Root/plugins/libseWindow-android.so
#include $(PREBUILT_SHARED_LIBRARY)









include $(CLEAR_VARS)

LOCAL_MODULE    := SurvivorEngineAndroid

include ../../src/Makefile
LOCAL_SRC_FILES := $(MODULES:%=SE_Root/src/%.cpp)
LOCAL_C_INCLUDES := SE_Root/project/include
LOCAL_C_INCLUDES := SE_Root/project/include/SE
#LOCAL_EXPORT_C_INCLUDE_DIRS := ../project/include
BYPASS_INCLUDE_PATH := SE_Root/project/include

LOCAL_CPPFLAGS  := -ISE_Root/project/include -fexceptions -ldl -O0 -g3 -std=c++11 -shared -rdynamic -fPIC -D_GLES -D_ANDROID -D_POSIX_THREAD -D_TEST -D_NO_BULLET -D_NO_BASS
LOCAL_CFLAGS := -ISE_Root/project/include -std=c++11
LOCAL_CPP_FEATURES += exceptions
LOCAL_SHARED_LIBRARIES := bullet bass
LOCAL_LDLIBS		:= -lz -lGLESv2 -ldl -llog -landroid -lEGL
LOCAL_STATIC_LIBRARIES := android_native_app_glue
#LOCAL_LDLIBS += -fuse-ld=bfd
include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_EXECUTABLE)
$(call import-module,android/native_app_glue)


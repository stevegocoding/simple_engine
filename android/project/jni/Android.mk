LOCAL_PATH := $(call my-dir) 

include $(CLEAR_VARS)

ENGINE_CORE_INC_PATH := $(abspath $(LOCAL_PATH)/../../../engine_main/core/include)
ENGINE_CORE_SRC_PATH := $(abspath $(LOCAL_PATH)/../../../engine_main/core/src)
ENGINE_DEPS_PATH := $(abspath $(LOCAL_PATH)/../../../deps)

LOCAL_MODULE 		:= simple_engine_android
LOCAL_SRC_FILES 	:= $(wildcard $(ENGINE_CORE_SRC_PATH)/*.cpp) app_android_native.cpp log.cpp
LOCAL_C_INCLUDES 	:= $(ENGINE_DEPS_PATH)/boost $(ENGINE_CORE_INC_PATH)
LOCAL_LDLIBS		:= -landroid -llog -lEGL -lGLESv2
LOCAL_CPP_FEATURES 	+= rtti 
LOCAL_CPP_FEATURES  += exceptions
LOCAL_STATIC_LIBRARIES	:= android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
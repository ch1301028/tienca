#LOCAL_PATH := $(call my-dir)
#
#include $(CLEAR_VARS)
#
#$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
#$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
#$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
#
#LOCAL_MODULE := cocos2dcpp_shared
#
#LOCAL_MODULE_FILENAME := libcocos2dcpp
#
#CLASSES_FILES   := $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)
#CLASSES_FILES   := $(CLASSES_FILES:$(LOCAL_PATH)/%=%)
#
#LOCAL_SRC_FILES := hellocpp/main.cpp \
#                   ../../Classes/AbstractConfig.cpp \
#                   ../../Classes/AppDelegate.cpp \
#                   ../../Classes/Bullet.cpp \
#                   ../../Classes/Cannon.cpp \
#                   ../../Classes/ConfigManagement.cpp \
#                   ../../Classes/Fish.cpp \
#                   ../../Classes/FishAniData.cpp \
#                   ../../Classes/FishConfig.cpp \
#                   ../../Classes/NetworkManagement.cpp \
#                   ../../Classes/ResourceManager.cpp \
#                   ../../Classes/GameLayer.cpp \
#                   ../../Classes/ChargingLayer.cpp \
#                   ../../Classes/GunConfig.cpp \
#                   ../../Classes/MenuLayer.cpp \
#                   ../../Classes/RollNum.cpp \
#                   ../../Classes/ScriptConfig.cpp \
#                   ../../Classes/User.cpp
#
#LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
#					$(LOCAL_PATH)/.. \
#					$(LOCAL_PATH)/../../cocos2d/extensions \
#					$(LOCAL_PATH)/../../cocos2d/external \
#					$(LOCAL_PATH)/../../cocos2d/cocos \
#					$(LOCAL_PATH)/../../cocos2d/cocos/editor-support \
#					$(LOCAL_PATH)/../../cocos2d
#
#LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
#LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
#LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
#LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
#LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
#
## LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
## LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
## LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
## LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
## LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
## LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
#
#
#include $(BUILD_SHARED_LIBRARY)
#
#$(call import-module,.)
#$(call import-module,audio/android)
#
## $(call import-module,Box2D)
## $(call import-module,editor-support/cocosbuilder)
## $(call import-module,editor-support/spine)
# $(call import-module,editor-support/cocostudio)
# $(call import-module,network)
# $(call import-module,extensions)

LOCAL_PATH := $(call my-dir)
 
include $(CLEAR_VARS)
 
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
 
LOCAL_MODULE := cocos2dcpp_shared
 
LOCAL_MODULE_FILENAME := libcocos2dcpp
 
MY_FILES_PATH  :=  $(LOCAL_PATH) \
                   $(LOCAL_PATH)/../../Classes
 
MY_FILES_SUFFIX := %.cpp %.c
                        
# Make does not offer a recursive wildcard function, so here's one:
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
     
# Retrieve all source files in each dir recursively.
MY_ALL_FILES := $(foreach src_path,$(MY_FILES_PATH), $(call rwildcard,$(src_path),*.*) )
MY_ALL_FILES := $(MY_ALL_FILES:$(MY_CPP_PATH)/./%=$(MY_CPP_PATH)%)
MY_SRC_LIST  := $(filter $(MY_FILES_SUFFIX),$(MY_ALL_FILES))
MY_SRC_LIST  := $(MY_SRC_LIST:$(LOCAL_PATH)/%=%)
 
# helper function for remove duplicated words in string test
define uniq =
  $(eval seen :=)
  $(foreach _,$1,$(if $(filter $_,${seen}),,$(eval seen += $_)))
  ${seen}
endef
     
# Retrieve all directory recursively.
MY_ALL_DIRS := $(dir $(foreach src_path,$(MY_FILES_PATH), $(call rwildcard,$(src_path),*/) ) )
MY_ALL_DIRS := $(call uniq,$(MY_ALL_DIRS))
 
# All done, just tell NDK build system.
LOCAL_SRC_FILES  := $(MY_SRC_LIST)
LOCAL_C_INCLUDES := $(MY_ALL_DIRS)
LOCAL_STATIC_LIBRARIES := cocos2dx_static
 
include $(BUILD_SHARED_LIBRARY)
 
$(call import-module,.)

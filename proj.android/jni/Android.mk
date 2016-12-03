LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos/audio/include)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/GameManager.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/MainMenuScene.cpp \
                   ../../Classes/SplashScreenScene.cpp \
                   ../../Classes/GameScene.cpp \
                   ../../Classes/SpaceShip.cpp \
                   ../../Classes/FriendlyShipBlast.cpp \
                   ../../Classes/MyJNIHelper.cpp \
                   ../../Classes/NetworkChooseScene.cpp \
                   ../../Classes/ClientConnectScene.cpp \
                   ../../Classes/ClientWaitingScene.cpp \
                   ../../Classes/ServerWaitingScene.cpp \
                   ../../Classes/EnemyShipBlast.cpp \
                   ../../Classes/GameEffects.cpp \
                   ../../Classes/JNIHelpers.cpp \
                   ../../Classes/JNIResults.cpp \
                   ../../Classes/SonarFrameworks.cpp \
                   ../../Classes/MenuLayer.cpp \
                   ../../Classes/AboutScene.cpp \
                   ../../Classes/NameSetScene.cpp \
                   ../../Classes/GamePrepareScene.cpp \
                   ../../Classes/ShipBlast.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END

/*
 Copyright (c) 2014 Mudafar
 GPLv3
 */


#include "Utils.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "NetworkManagement.h"
#include "User.h"
#include "ChargingLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#endif


const char* JniClassName = "org/cocos2dx/cpp/AppActivity";

using namespace CocosDenshion;


void Utils::playMusic(string filename,bool isLoop){
	if(!User::Instance() . getMusic())
	{
		return;
	}
    auto audio = SimpleAudioEngine::getInstance();

    // set the background music and continuously play it.
    audio->playBackgroundMusic(filename.c_str(), isLoop);
} 

void Utils::playEffect(string filename){
    if(!User::Instance() . getMusic())
	{
		return;
	}

	auto audio = SimpleAudioEngine::getInstance();

    // play a sound effect, just once.
    audio->playEffect(filename.c_str(), false);
}

void Utils::updateMusic(bool on){
	auto audio = SimpleAudioEngine::getInstance();

	if(on){
		// resume background music.
		audio->resumeBackgroundMusic();
	}
	else{
		// pause background music.
		audio->pauseBackgroundMusic();


	}

}


void Utils::openUrl(const char * url){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	{
		cocos2d::JniMethodInfo minfo;
		if (cocos2d::JniHelper::getStaticMethodInfo(minfo, JniClassName,
				"openUrl",
				"(Ljava/lang/String;)V"))
		{
				jstring strParam = minfo.env->NewStringUTF(url);
				minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, strParam);
				minfo.env->DeleteLocalRef(strParam);
				minfo.env->DeleteLocalRef(minfo.classID);		
		}		
	}
#elif (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
	//helperOpenURL(url);
#else
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void Utils::openSms(int amount){
	{
		cocos2d::JniMethodInfo minfo;
		if (cocos2d::JniHelper::getStaticMethodInfo(minfo, JniClassName,
				"openSms",
				"(I)V"))
		{
				minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, amount);
				minfo.env->DeleteLocalRef(minfo.classID);		
		}		
	}
}

#elif (CC_TARGET_PLATFORM==CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	
#else
void Utils::openSms(int amount){
}
#endif


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
string Utils::getUniqueId(){

	{
		JniMethodInfo minfo;
        
		if(JniHelper::getStaticMethodInfo(minfo, JniClassName, 
			"getDeviceUniqueId",
			 "()Ljava/lang/String;"))
		{			
			jstring return_value = (jstring) minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
			string ret = JniHelper::jstring2string(return_value);	
			return ret;
		}

	}
}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)


#else
string Utils::getUniqueId(){
    return "ADMIN";
}
#endif

/*
 Copyright (c) 2014 Mudafar
 GPLv3
 */


#ifndef  __UTILS_HELPER_H_
#define  __UTILS_HELPER_H_

#include "cocos2d.h"

 using namespace std;
 using namespace cocos2d;

class Utils
{
public:
    static void playMusic(string,bool);
    static void playEffect(string);
    static void updateMusic(bool on);
    static void openUrl(const char * url);
    static void openSms(int amount);
    static string getUniqueId();
    
};


#endif // __ADMOB_HELPER_H_
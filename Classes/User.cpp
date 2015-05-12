//
//  User.cpp
//  Tiên Cá
//
//  Created by Ho Duy Nhat Linh on 1/18/15.
//
//


#include "User.h"
#include "ChargingLayer.h"


User::User(){
    load();
}
User::~User(){
    
}

void User::load(){
    
    bool change = false;
    if (UserDefault::getInstance()->getIntegerForKey("coin",NULL)){
        m_coin = 1000;
        change = true;
    }
    else
    {
        m_coin = UserDefault::getInstance()->getIntegerForKey("coin");
    }
    
    if (UserDefault::getInstance()->getIntegerForKey("xp",NULL)){
        m_xp = 1;
        change = true;
    }
    else
    {
        m_xp = UserDefault::getInstance()->getIntegerForKey("xp");
    }
    
    if (UserDefault::getInstance()->getIntegerForKey("level",NULL)){
        m_level = 1;
        change = true;
    }
    else
    {
        m_level = UserDefault::getInstance()->getIntegerForKey("level");
    }
    
    if (UserDefault::getInstance()->getBoolForKey("music",true)){
        m_music = true;
        change = true;
    }
    else
    {
        m_music = UserDefault::getInstance()->getBoolForKey("music", true);
    }

    if(m_last_time = UserDefault::getInstance()->getDoubleForKey("last_time",NULL)){
        
        std::time_t t = std::time(0);

        m_last_time = (double)t;
        
        change = true;
    }
    else
    {
        m_last_time = UserDefault::getInstance()->getDoubleForKey("last_time");
    }
    
    if (change)
    {
        this->save();
    }
    
}
void User::setLastTimeUpdate()
{
    std::time_t t = std::time(0);
    m_last_time = (double)t;
}

void User::save(){
    UserDefault::getInstance()->setIntegerForKey("xp", m_xp);
    UserDefault::getInstance()->setIntegerForKey("level", m_level);
    UserDefault::getInstance()->setIntegerForKey("coin", m_coin);
    UserDefault::getInstance()->setDoubleForKey("last_time", m_last_time);
    UserDefault::getInstance()->setBoolForKey("music", m_music);
    UserDefault::getInstance()->flush();
}

void User::parseServerData(const rapidjson::Value&  document){
    if(document.HasMember("server_coin")){
        m_server_coin = document ["server_coin"].GetInt();
    }
    if(document.HasMember("bonus_coin")){
        m_coin += document ["bonus_coin"].GetInt();
    }
    
    save();
}


User& User::Instance(){
    static User _instance;
    return _instance;
}
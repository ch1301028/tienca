//
//  User.h
//  Tiên Cá
//
//  Created by Ho Duy Nhat Linh on 1/18/15.
//
//

#ifndef __Fish___User__
#define __Fish___User__

#include <stdio.h>

#include "GameConfig.h"
#include "cocos2d.h"
#include "ChargingLayer.h"


using namespace std;
using namespace cocos2d;

class User{
    
public:
    
    User();

    virtual ~User();
    
    static User	&Instance();
    
    CC_SYNTHESIZE(int , m_level,	Level);
    
    CC_SYNTHESIZE(int , m_xp,	XP);
    
    CC_SYNTHESIZE(int , m_coin,	Coin);
    
    CC_SYNTHESIZE(int , m_server_coin,	ServerCoin);
    
    CC_SYNTHESIZE(double, m_last_time, LastTime);
    
    CC_SYNTHESIZE(bool, m_music, Music);

    void save();
    
    void load();
    
    void parseServerData(const rapidjson::Value& );
    
    void setLastTimeUpdate();
    
};



#endif /* defined(__Tie_n_Ca___User__) */

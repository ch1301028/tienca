//
//  Cannon.h
//  FishingJoy
//
//  Created by 胡 蓉 on 12-11-30.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef FishingJoy_Cannon_h
#define FishingJoy_Cannon_h

#include "cocos2d.h"
using namespace cocos2d;
class GameLayer;

class Cannon : public cocos2d::Ref
{
public:
    Cannon();
    virtual ~Cannon();
    
    static Cannon *createWithCannonType(int cannonType, GameLayer *pGameLayer, cocos2d::SpriteBatchNode *pBatchNode);
    CC_SYNTHESIZE(int, m_nCannonType, CannonType);
    CC_SYNTHESIZE(cocos2d::Sprite *, m_pSprite, Sprite);
    CC_SYNTHESIZE(float, m_fRotation, Rotation);
    CC_SYNTHESIZE(GameLayer *, m_pGameLayer, GameLayer);
	
    CC_SYNTHESIZE(Point, m_ptDirection, Direction);

	CC_SYNTHESIZE(int, _cost, Cost);
	CC_SYNTHESIZE(int, _power, Power);
    void setRotate(Point pt);
    void rotateToPoint(cocos2d::Point pt);
    void fire();
	void removeSelf();
    void transformToRotation(float );
    
private:
    bool initWithCannonType(int cannonType, GameLayer* pGameLayer, SpriteBatchNode *pBatchNode);
};

#endif

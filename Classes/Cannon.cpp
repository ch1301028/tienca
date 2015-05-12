//
//  Cannon.cpp
//  FishingJoy
//
//  Created by 胡 蓉 on 12-11-30.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Cannon.h"
#include "Bullet.h"
#include "GunConfig.h"

using namespace cocos2d;

Cannon::Cannon(){
    
}

Cannon::~Cannon(){
    
}

Cannon *Cannon::createWithCannonType(int cannonType, GameLayer* pGameLayer, SpriteBatchNode *pBatchNode)
{
    Cannon *pCannon = new Cannon();
    if(pCannon && pCannon->initWithCannonType(cannonType, pGameLayer, pBatchNode))
    {
        pCannon->autorelease();
        return pCannon;
    }
    else
    {
        CC_SAFE_DELETE(pCannon);
        return NULL;
    }
}

bool Cannon::initWithCannonType(int cannonType, GameLayer* pGameLayer, SpriteBatchNode *pBatchNode)
{
	auto gunConfig = GunConfig::getInstance()->getData();

	this->setPower(gunConfig[StringUtils::format("%d",cannonType)].asValueMap()["power"].asInt());
	this->setCost(gunConfig[StringUtils::format("%d", cannonType)].asValueMap()["cost"].asInt());
	
	Size winSize = Director::getInstance()->getWinSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

    this->setCannonType(cannonType);
    this->setGameLayer(pGameLayer);
	CCLOG("net_%d2.png", cannonType);
	m_pSprite = Sprite::createWithSpriteFrameName(StringUtils::format("net_%d2.png",cannonType));
	
	m_pSprite->setPosition(Vec2(winSize.width / 2, m_pSprite->getContentSize().height ));
	m_pSprite->setAnchorPoint(Vec2(0.5f,0.3f));
    pBatchNode->addChild(m_pSprite);
    return true;
}

void Cannon::rotateToPoint(cocos2d::Point ptTo)
{
    Point ptFrom = m_pSprite->getPosition();
    float angle = atan2f(ptTo.y - ptFrom.y, ptTo.x - ptFrom.x) / M_PI * 180.0f;
    this->transformToRotation(90.0f - angle);
    this->setDirection(ptTo);
}
void Cannon::setRotate(Point ptTo){
    Point ptFrom = m_pSprite->getPosition();
    float angle = atan2f(ptTo.y - ptFrom.y, ptTo.x - ptFrom.x) / M_PI * 180.0f;

    m_pSprite->setRotation(90.0f - angle) ;
    this->setDirection(ptTo);
}


void Cannon::transformToRotation(float var)
{
    m_fRotation = var;
    
    float duration = fabsf(m_fRotation - m_pSprite->getRotation()) / 180.0f * 0.2f;
    
    FiniteTimeAction *pAction = CCRotateTo::create(duration, m_fRotation);
    m_pSprite->runAction(pAction);
}

void Cannon::fire()
{
	SpriteFrame *pFrame1 = SpriteFrameCache::getInstance()->spriteFrameByName(StringUtils::format("net_%d2.png",this->getCannonType()));
    SpriteFrame *pFrame2 = SpriteFrameCache::getInstance()->spriteFrameByName(StringUtils::format("net_%d1.png",this->getCannonType()));
	Vector<SpriteFrame *> frame(3);
	frame.pushBack(pFrame1);
	frame.pushBack(pFrame2);
	
	Animation *animation = Animation::createWithSpriteFrames(frame, 0.1f);
    animation->setRestoreOriginalFrame(true);
    FiniteTimeAction *pAction = Animate::create(animation);
    m_pSprite->runAction(pAction);
    
    
	Bullet *pBullet = Bullet::createWithBulletType(this->getCannonType(), m_pGameLayer, m_pGameLayer->getBatchNodeBullet(), m_pGameLayer->getBatchNode3());
    pBullet->shootTo(m_ptDirection);
}
void Cannon::removeSelf()
{
	m_pSprite->removeFromParentAndCleanup(true);
	//this->getGameLayer()->getFishes()->removeObject(this);
}
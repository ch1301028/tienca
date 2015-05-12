//
//  Bullet.cpp
//  FishingJoy
//
//  Created by 胡 蓉 on 12-11-30.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Bullet.h"
#include "GunConfig.h"

using namespace cocos2d;

Bullet *Bullet::createWithBulletType(int bulletType, GameLayer *gameLayer, SpriteBatchNode *pBatchNodeBullet, SpriteBatchNode *pBatchNodeNet)
{
    Bullet *pBullet = new Bullet();
    if(pBullet && pBullet->initWithBulletType(bulletType, gameLayer, pBatchNodeBullet, pBatchNodeNet))
    {
        pBullet->autorelease();
        return pBullet;
    }
    else
    {
        CC_SAFE_DELETE(pBullet);
        return NULL;
    }
}

bool Bullet::initWithBulletType(int bulletType, GameLayer *gameLayer, SpriteBatchNode *pBatchNodeBullet,  SpriteBatchNode *pBatchNodeNet)
{
    m_bCaught = false;
    
    this->setBulletType(bulletType);
	this->setGameLayer(gameLayer); 

	this->setPower(GunConfig::getInstance()->getData()[StringUtils::format("%d", bulletType)].asValueMap()["power"].asInt());

    String *bulletName = String::createWithFormat("bullet_%d.png", m_nBulletType);
	//String *bulletName = String::createWithFormat("bullet_%d.png", m_nBulletType);
    this->setSpriteBullet(CCSprite::createWithSpriteFrameName(bulletName->getCString()));
	Sprite *cannon = this->getGameLayer()->getCannon()->getSprite();
	m_pSpriteBullet->setPosition(cannon->getPosition());
    pBatchNodeBullet->addChild(m_pSpriteBullet,-1);
        
    m_pGameLayer->getBullets()->addObject(this);    
    return true;
}

void Bullet::shootTo(Point targetDirection)
{
    Point ptFrom = m_pSpriteBullet->getPosition();
    Point ptTo = targetDirection;
    float angle = atan2f(ptTo.y - ptFrom.y, ptTo.x - ptFrom.x);
    float rotation = angle / M_PI * 180.0f;
    m_pSpriteBullet->setRotation(90.0f - rotation);
    
    float distance = 1000.0f;
    Point targetPt = Vec2(ptFrom.x + distance * cosf(angle), ptFrom.y + distance * sinf(angle));
    FiniteTimeAction *moveto = MoveTo::create(1.0f, targetPt);
    FiniteTimeAction *callFunc = CallFunc::create(this, callfunc_selector(Bullet::removeSelf));
    FiniteTimeAction *sequence = Sequence::create(moveto, callFunc, NULL);
    m_pSpriteBullet->runAction(sequence);
}
void Bullet::showSmoke(){
	m_bCaught = true;
    m_pSpriteBullet->stopAllActions();
    m_pSpriteBullet->setVisible(false);    

	ParticleSystem *particle = ParticleSystemQuad::create("Explosive.plist");
    particle->setPosition(m_pSpriteBullet->getPosition());
    particle->setPositionType( kCCPositionTypeGrouped);
    particle->setAutoRemoveOnFinish(true);
	particle->setScale(0.3f);
    m_pGameLayer->addChild(particle, 100);
  
}
void Bullet::showNet()
{
    m_bCaught = true;
    m_pSpriteBullet->stopAllActions();
    m_pSpriteBullet->setVisible(false);
    

	ParticleSystem *particle = ParticleSystemQuad::create("ExplodingRing.plist");
    particle->setPosition(m_pSpriteBullet->getPosition());
    particle->setPositionType(kCCPositionTypeGrouped);
    particle->setAutoRemoveOnFinish(true);
    m_pGameLayer->addChild(particle, 100);
  
}


void Bullet::removeSelf()
{
    this->getGameLayer()->getBullets()->removeObject(this);
    m_pSpriteBullet->removeFromParentAndCleanup(true);
 
}


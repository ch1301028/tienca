#include "Fish.h"
#include "GameLayer.h"
#include "FishConfig.h"
#include "FishAniData.h"





using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocostudio;



#define GRID_SIZE 5

Fish::~Fish()
{
    
}





Fish *Fish::createWithFishType(int fishType, GameLayer *gameLayer, SpriteBatchNode *spriteBatchNode)


{
    
    
    Fish *fish = new Fish();
    
    
    if (fish && fish->initWithFishType(fishType, gameLayer, spriteBatchNode))
        
        
    {
        
        
        fish->autorelease();
        
        
        return fish;
        
        
    }
    
    CC_SAFE_DELETE(fish);
}





Size Fish::getFishSize(int fishType){
    
    
    string originalFrameName = StringUtils::format("fish%02d_01.png", fishType);
    
    
    //CCString *originalFrameName = CCString::createWithFormat("creep_%02d.png", fishType);
    
    
    auto sprite = Sprite::createWithSpriteFrameName(originalFrameName);
    
    
    auto size = Size(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2);
    
    
    return size;
    
    
}


bool Fish::initWithFishType(int fishType, GameLayer *gameLayer, SpriteBatchNode *spriteBatchNode)


{
    
    
    Size winSize = Director::getInstance()->getWinSize();
    
    
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    
    
    
    
    // Initialize fish const
    
    
    m_bParticleBubble = false;
    
    
    m_bCaught = false;
    
    
    m_crtPoint = 0;
    
    
    isInit = false;
    
    
    m_deleted = false;
    
    
    // Initialize fish parameter
    
    
    auto fishAniData = FishAniData::getInstance()->getData();
    
    
    this->setFishType(fishType);
    
    
    this->setGameLayer(gameLayer);
    
    
    
    
    
    // Initialize animation string
    
    
    int countAni = fishAniData[StringUtils::format("swing_fish_%02d", fishType)].asInt();
    
    
    _animFrames = *(new Vector<SpriteFrame*>(countAni));
    
    
    for (int i = 1; i <= countAni; i++)
        
        
    {
        
        
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(
                                                                              
                                                                              
                                                                              StringUtils::format("fish%02d_%02d.png", fishType, i));
        
        
        _animFrames.pushBack(spriteFrame);
        
        
    }
    
    // Initialize sprite
    
    
    m_pSprite = Sprite::createWithSpriteFrameName(StringUtils::format("fish%02d_01.png", fishType));
    
    
    m_pSprite->setAnchorPoint(Vec2(0.1f, 0.5f));
    
    
    spriteBatchNode->addChild(m_pSprite);
    
    // Hide Object
    
    
    this->setAlive(false);
    
    return true;
    
    
}


void Fish::initParameter(std::vector<Point> pts){
    
    
    this->setPoints(pts);
    
    
    int fishType = this->getFishType();
    
    
    auto fishConfig = FishConfig::getInstance()->getData();
    
    
    this->setHP(fishConfig[StringUtils::format("%d", fishType)].asValueMap()["hp"].asInt());
    
    
    this->setInstantRate(fishConfig[StringUtils::format("%d", fishType)].asValueMap()["d-rate"].asInt());
    
    
    int max_coin = fishConfig[StringUtils::format("%d", fishType)].asValueMap()["max-coin"].asInt();
    
    
    int min_coin = fishConfig[StringUtils::format("%d", fishType)].asValueMap()["min-coin"].asInt();
    
    this->setCoin(min_coin + (int)((CCRANDOM_0_1()   * (max_coin - min_coin))));
    
    this->setIsInstance((CCRANDOM_0_1() * 1000  < this->getInstantRate()));
    
    this->setHP(this->getIsInstance()? 1 : this->getHP());
    
    this->_hit = false;
    
    
    this->setAlive(true);
    
    
    this->getPath();
    
    
    Point startPts = this->getPoints()[0];
    
    
    m_pSprite->setPosition(startPts);
    
    
    m_previousPoint = startPts;
    
    
    m_nextPoint = this->getPoints()[1];
    
    
    this->setDeleted(false);
    
    
    
    
    
    
    auto _animation = Animation::createWithSpriteFrames(_animFrames, 0.2f);
    
    
    auto _animate = Animate::create(_animation);
    
    
    m_pSprite->runAction(RepeatForever::create(_animate));
    
    
}


void Fish::showCaught()


{
    this->setVisible(false);
    
    m_bCaught = true;
    
    
    m_pSprite->stopAllActions();
    
    
    FiniteTimeAction *callFunc = CallFunc::create(CC_CALLBACK_0(Fish::removeSelf, this));
    
    
    FiniteTimeAction *sequence = Sequence::create(FadeOut::create(1.0f), callFunc, NULL);
    
    
    m_pSprite->runAction(sequence);
    
    
}


void offsetPoint(Point& pt, float offsetX, float offsetY)


{
    pt.x += offsetX;
    
    
    pt.y += offsetY;
    
}


void Fish::setAlive(bool status){

    m_pSprite->stopAllActions();
    
    this->getSprite()->setVisible(status);
    
    m_pSprite->setOpacity(status ? 255 : 0);
    
    this->setVisible(status);
    
    m_bCaught = status;
}


void Fish::getPath()


{
    
    Size fishSize = m_pSprite->getContentSize();
    
    
    Size winSize = Director::getInstance()->getWinSize();
    
    
    Point ptStart, ptEnd;
    
   int min_y, max_y;
    
    
    min_y = 100;
    
    
    max_y = winSize.height;
    
    
    ptStart = this->getPoints()[0];
    
    Point endPts = this->getPoints()[this->getCrtPoint() + 1];
    
    
    m_nextPoint = this->getPoints()[(this->getPoints().size() > this->getCrtPoint() + 2) ? this->getCrtPoint() + 2 : this->getCrtPoint() + 1];
    
    
    ptEnd.x = endPts.x;
    
    
    ptEnd.y = endPts.y;
    
    float angle = atan2f(this->getNextPoint().y - this->getPreviousPoint().y, this->getNextPoint().x - this->getPreviousPoint().x);
    
    float rotation = angle * 180.0f / M_PI;
    
    m_pSprite->setPosition(ptStart);
    
    
    m_pSprite->setRotation(rotation);
    
    
    FiniteTimeAction *releaseFunc = CallFunc::create(CC_CALLBACK_0(Fish::removeSelf, this));
    
    vecBezierConfig cBezier;
    
    for(int i = 0; i < this->getPoints().size() ; i ++ )
    {
        auto pts = this->getPoints()[i];
        
        cBezier.bezierPtsConfigX.push_back(pts.x);
        
        cBezier.bezierPtsConfigY.push_back(pts.y);
    }
    
    auto benzier = ZBezierBy::actionWithDuration(10, cBezier);
    
    m_pSprite->runAction(Sequence::create(benzier, releaseFunc, NULL));
    
    isInit = false;

}





void Fish::removeSelf()


{
    this->setAlive(false);
    
    this->setDeleted(true);
}


void Fish::hit(int power){
    
    
    this->setHP(this->getHP() - power);
    
    
    if(!this->_hit){
        
        
        this->_hit = true;
        
        
    }
    
    //auto tintBy = TintBy::create(0.1f, 0, -255, 0);
    
}



Rect Fish::getBounding(){
    
    
    return this->getSprite()->getBoundingBox();
    
    
}

long *  ZBezierBy::factorials =  NULL;
ZBezierBy* ZBezierBy::actionWithDuration(float t, const vecBezierConfig &vec )
{
    
    ZBezierBy* b = new ZBezierBy;
    b->initWithDuration(t, vec);
    return b;
    
}


unsigned long ZBezierBy :: factorial(long n)
{
    if(factorials == NULL)
    {
        factorials =  new long[100];;
        for(int i=0;i < 100;i++){
            factorials[i] = 0;
        }
    }

    if (n == 0 || n== 1) {
        return 1;
    }
    else {
     if (factorials[n] != 0)
         return factorials[n];
     else
         return factorials[n] = (n *  factorial(n - 1));
     }
    
}
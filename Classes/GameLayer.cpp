#include "GameLayer.h"
#include "SimpleAudioEngine.h"
#include "ChargingLayer.h"
#include "Fish.h"
#include "Cannon.h"
#include "Bullet.h"
#include "RollNum.h"
#include "User.h"
#include "FishConfig.h"
#include "ScriptConfig.h"
#include "GameConfig.h"
#include "BonusLayer.h"
#include "Utils.h"
#include "ContextLayer.h"

using namespace cocos2d;
using namespace CocosDenshion;

#define MAX_SCRIPT_SIZE 3

const vector<int> FishMaximum = {
	36,//0
	36,//1
	36,//2
	36,//3
	36,//4
	36,//5
	36,//6
	9,//7
	9,//8
	9,//9
	2,//10
	2,//11
	2,//12
	2,//11
	2,//14
	2,//15
	2,//16
	2,//17
	0//18
};

const vector<vector<int>> FishInBatchNode = {
	{ 99 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9 },
	{ 99 },
	{ 99 },
	{ 99 },
	{ 15, 16, 17, 18 }
};



std::vector<std::vector<Point>> team = {};

Scene* GameLayer::scene()
{
	Scene *scene = Scene::create();
	GameLayer *layer = GameLayer::create();
	scene->addChild(layer);
	return scene;
}

GameLayer::GameLayer() :
m_pBullets(NULL),
m_pRollNumGroup(NULL),
m_pCannon(NULL),
m_nScore(1000000),
spawnTime(0),
reloadBullet(0),
isFire(false),
m_crtCannon(4),
m_crtScript(1),
m_crtTime(0),
_nextTimeStampAddFish(0),
_timeStampAddFish(0),
m_show_charge(false),
addRewardAni(false)
{

}

GameLayer::~GameLayer()
{
	CC_SAFE_RELEASE(m_pBullets);
	CC_SAFE_RELEASE(m_pCannon);
	CC_SAFE_RELEASE(m_pRollNumGroup);
}

bool GameLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	this->initFrames();
	this->initBackground();
	this->initFishes();
	this->initCannon();
	this->initButtons();
    this->initRewards();
    this->initMusic();

	this->schedule(schedule_selector(GameLayer::updateGame), 0.05f);
	this->setBullets(CCArray::create());

	this->m_pRollNumGroup->setValue(User::Instance().getCoin());

    
	auto listener = EventListenerTouchOneByOne::create();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);

    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    this->updateXP();

    this->initNetwork();
    
    // if (AdmobHelper::isAdShowing)
    //     AdmobHelper::hideAd();
    // else
    //     AdmobHelper::showAd();
    
	return true;
}
void GameLayer::initNetwork()
{
    NetworkManagement::getInstance().initData(CC_CALLBACK_1(GameLayer::onHttpInitComplete,this));
}
void GameLayer::initFrames()
{
    SpriteFrameCache :: getInstance()->addSpriteFramesWithFile("bullet.plist");
	SpriteFrameCache :: getInstance()->addSpriteFramesWithFile("8fire.plist");
	SpriteFrameCache :: getInstance()->addSpriteFramesWithFile("8goldItem.plist");
    SpriteFrameCache :: getInstance()->addSpriteFramesWithFile("FishUI0.plist");

	cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("fpack01.ExportJson");
	cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("fpack05.ExportJson");

}

void GameLayer::initMusic(){
	Utils::playEffect("music-affect-begin.mp3");

	Utils::playMusic("music-background.mp3",true);
}

void GameLayer::initBackground()
{
    Size winSize = Director::getInstance()->getWinSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
	Texture2D *texture;
    int bgRand = (int)(CCRANDOM_0_1() * 100 ) % 7 + 1;
	string bgStr = StringUtils::format("background/bg_%02d.jpg", bgRand);
	Sprite *pBackground = Sprite::create(bgStr);
	pBackground->setAnchorPoint(Vec2(0.5f, 0.5f));
	pBackground->setPosition( Vec2( winSize.width / 2, winSize.height / 2));
    this->addChild(pBackground);

	this->setRollNumGroup(RollNumGroup::createWithGameLayer(this, 6));
	m_pRollNumGroup->setPosition(Vec2(200, 70));
   
    texture = Director :: getInstance() -> getTextureCache() ->addImage("mask.png");
	Sprite *mask = Sprite::createWithTexture(texture);
	mask->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	//this->addChild(mask, 100);
}

void GameLayer::initFishes()
{
	for (int j = 0; j < FishInBatchNode.size(); j++){
		set<int> fishInBatchNode;
		auto t = FishInBatchNode[j];
		for (int i = 0; i < t.size(); i++){
			fishInBatchNode.insert(t[i]);

		}
		fishInBatchNodes.push_back(fishInBatchNode);
	}
	SpriteBatchNode * spriteBatchNode;
	Texture2D *texture;
	for (int i = 0; i < FishInBatchNode.size(); i++)
	{
        texture = Director :: getInstance() -> getTextureCache() -> addImage(StringUtils::format("fpack%02d0.png", i));
		Armature *armature;
		if (texture == NULL){
			spriteBatchNode = NULL;
			armature = NULL;
		}
		else{

			spriteBatchNode = SpriteBatchNode::createWithTexture(texture);
			this->addChild(spriteBatchNode);
		}
		this->m_pBatchNodeFishs.push_back(spriteBatchNode);
	}

	texture = Director :: getInstance() -> getTextureCache() -> addImage("bullet.png");
	this->setBatchNodeBullet(SpriteBatchNode::createWithTexture(texture));
	this->addChild(m_pBatchNodeBullet);
	this->m_pFishes = Array::createWithCapacity(MAX_FISH_COUNT);
    
	for (int i = 1; i < FishMaximum.size(); i++){
		for (int index = 0; index < fishInBatchNodes.size(); index++){
			set<int> fishInBatchNode;
			auto t = fishInBatchNodes[index];
			std::set<int>::iterator it = t.find(i);
			if (it != t.end()){
				for (int j = 0; j < FishMaximum[i]; j++){
					Fish * _fish = Fish::createWithFishType(i, this, this->getBatchNodeFishs()[index]);
					this->getFishes()->addObject(_fish);
				}
			}
		}
	}
    
	this->getFishes()->retain();

    scriptDocument = ScriptConfig::getInstance()->document["script"];
    teamDocument = ScriptConfig::getInstance()->document["team"];
    pathDocument = ScriptConfig::getInstance()->document["path"];
    int randomScript = (int)(CCRANDOM_0_1() * 100 ) % MAX_SCRIPT_SIZE;
  
    this->setCrtScript(randomScript + 1);
    this->setCrtTime(0);
    }

void GameLayer::initCannon()
{
    auto pTexture = Director :: getInstance() -> getTextureCache() -> addImage("8fire.png");
	this->setBatchNodeCannon(SpriteBatchNode::createWithTexture(pTexture));
	this->addChild(m_pBatchNodeCannon, 101);

	pTexture = Director :: getInstance() -> getTextureCache() -> addImage("8goldItem.png");
	this->setBatchNodeCoin(SpriteBatchNode::createWithTexture(pTexture));
	this->addChild(m_pBatchNodeCoin, 102);

	this->setCannon(Cannon::createWithCannonType(m_crtCannon + 1, this, m_pBatchNodeCannon));
	//m_pCannon->setRotation(90.0f);
}

void GameLayer::initButtons()
{
    menu = Menu::create();
	auto leftCannonSprite = Sprite::create("leftCannon.png");
	auto rightCannonSprite = Sprite::create("rightCannon.png");
    auto menuSprite = Sprite::create("menu-icon.png");

     auto frames = Vector<SpriteFrame *>(9);
    for (int i = 1; i <= 9; i++)
    {
        auto spriteFrameCache = (SpriteFrameCache :: getInstance()->getSpriteFrameByName(String::createWithFormat("gold%02d.png", i)->getCString()));
        frames.pushBack(spriteFrameCache);
    }
    auto chargeCoinSprite = Sprite::createWithSpriteFrameName("gold01.png");
    auto animation = Animation::createWithSpriteFrames(frames, 0.1f);
    auto 	animate = Animate::create(animation);
    Action *coinAnimation = RepeatForever::create(animate);
    chargeCoinSprite->runAction(coinAnimation);
    
    
	auto winSize = Director::getInstance()->getWinSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto leftCannon = MenuItemSprite::create(
		leftCannonSprite, leftCannonSprite,
		CC_CALLBACK_1(GameLayer::changeLeftCannon, this)
		);

	leftCannon->setPosition(winSize.width / 2 - leftCannonSprite->getContentSize().width - 50, leftCannonSprite->getContentSize().height  + 30);
    leftCannon->setScale(1.3f);
	auto rightCannon = MenuItemSprite::create(
		rightCannonSprite, rightCannonSprite,
		CC_CALLBACK_1(GameLayer::changeRightCannon, this)
		);
    rightCannon->setScale(1.3f);
	rightCannon->setPosition(winSize.width / 2 + rightCannonSprite->getContentSize().width + 50, rightCannonSprite->getContentSize().height  + 30);
    
    chargeCoinBtn = MenuItemSprite::create(
                                             chargeCoinSprite, chargeCoinSprite,
                                             CC_CALLBACK_1(GameLayer::chargeCoin, this)
                                             );
    chargeCoinBtn->setPosition(Vec2(chargeCoinBtn->getContentSize().width + 20  ,
                                 leftCannonSprite->getContentSize().height  + 30 ));
    
    //chargeCoinBtn->setScale(1.0f);
    
    //Enable Payment Method by default
    chargeCoinBtn->setVisible(false);

    auto menuItem = MenuItemSprite::create(
		menuSprite, menuSprite,
		CC_CALLBACK_1(GameLayer::menuUpdate, this)
		);

	menuItem->setPosition(winSize.width  - menuSprite->getContentSize().width/2 ,winSize.height  -  menuSprite->getContentSize().height * 2  );
	menuItem->setScale(0.5f);
    menu->addChild(chargeCoinBtn);
	menu->addChild(leftCannon);
	menu->addChild(rightCannon);
	menu->addChild(menuItem);

	menu->setPosition(0, 0);
	this->addChild(menu);

	auto t = Sprite::create("border-process.png");
    lbLevel = Label::create();
    lbLevel->setString(StringUtils ::format("%d",User::Instance().getLevel()));
    lbLevel->setTextColor(Color4B(255,215,0,255));
    lbLevel->setHorizontalAlignment(TextHAlignment::CENTER);
    lbLevel->setPosition(t->getContentSize().width/2,t->getContentSize().height/2);
    lbLevel->enableOutline(Color4B::WHITE, 3);
    lbLevel->setSystemFontSize(36);
    t->addChild(lbLevel);
    
	this->processBarXP = ProgressTimer::create(Sprite::create("process.png"));
	processBarXP->setType(ProgressTimerType::RADIAL);
	processBarXP->setAnchorPoint(Point(0, 0));
	processBarXP->setBarChangeRate(Vec2(1, 1));
	t->addChild(processBarXP);
	t->setPosition(0 + t->getContentSize().width / 2, winSize.height - t->getContentSize().width / 2);
	this->addChild(t);


}

void GameLayer::initRewards(){
    Size winSize = Director::getInstance()->getWinSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    auto rewardSprite = Sprite::create("border-reward-process.png");
    auto selectedSprite= Sprite::create("border-reward-process.png");
    this->processBarReward = ProgressTimer::create(Sprite::create("reward-process.png"));
    processBarReward->setType(ProgressTimerType::RADIAL);
    processBarReward->setAnchorPoint(Point(0, 0));
    processBarReward->setBarChangeRate(Vec2(1, 1));
    rewardSprite->addChild(processBarReward);
    
    lbRewardTimer = Label::create();
    lbRewardTimer->setString("##:##:##");
    lbRewardTimer->setAlignment(TextHAlignment::CENTER);
    lbRewardTimer->setPositionY(rewardSprite->getContentSize().height/4);
    lbRewardTimer->setPositionX(rewardSprite->getContentSize().width/2);
    lbRewardTimer->enableOutline (Color4B::WHITE,1);
    rewardSprite->addChild(lbRewardTimer);
    
    rewardButton = MenuItemSprite::create(rewardSprite, selectedSprite,
                                               CC_CALLBACK_1(GameLayer::colectReward, this)
                                               );
    rewardButton->setPosition((winSize.width - rewardSprite->getContentSize().width  - 10) , (winSize.height - rewardSprite->getContentSize().height) / 2);
    rewardButton->setScale(1.3f);
    rewardButton->runAction(RepeatForever::create(Shake::create(2.0f, 5)) );
    menu->addChild(rewardButton);
}

void GameLayer::updateReward(float dt){
    std::time_t nowReward = std::time(0);
    
    double lastTime = User::Instance().getLastTime();
    
    int nextTime = ScriptConfig::getInstance()->document["reward"]["deltaTime"].GetInt();
    
    std::time_t nextReward = (std::time_t) (lastTime + nextTime);
    
    int diffTime = difftime(nextReward , nowReward);
    
    if(diffTime < 0){
        diffTime = 0;
    }
    
    if(diffTime > 0){
        time_t seconds(diffTime); // you have to convert your input_seconds into time_t
        tm *p = gmtime(&seconds); // convert to broken down time
        
        canGetReward = false;
        lbRewardTimer->setString(StringUtils::format("%02d:%02d:%02d",p->tm_hour,p->tm_min,p->tm_sec));
        rewardButton->stopAllActions();
       
        
        if(!addRewardAni){
            rewardButton->runAction(RepeatForever::create(Shake::create(2.0f, 5)) );
            addRewardAni = true;
        }
    }
    else{
        canGetReward = true;
        
        lbRewardTimer->setString("Nhận");
        
        addRewardAni = false;
    }
}

void GameLayer::addFish()
{
	Size winSize = Director::getInstance()->getWinSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

    
    
	//while (1)
	{
        const rapidjson::Value& scriptDocument = this->scriptDocument;
        
        
        if(!scriptDocument.HasMember(StringUtils::format("%d",this->getCrtScript()).c_str())){
            return;
        }
        
        const rapidjson::Value& scriptConfig = scriptDocument[StringUtils::format("%d",this->getCrtScript()).c_str()];
        int crtTime = this->getCrtTime();
        if(!scriptConfig.HasMember(StringUtils::format("%d",crtTime).c_str())){
            
            return;
        }
        
        
        const rapidjson::Value& fishConfigs = scriptConfig[StringUtils::format("%d",crtTime).c_str()];
        
        for (rapidjson::SizeType index = 0; index < fishConfigs.Size(); index++){

            const rapidjson::Value& fishConfig = fishConfigs[index];
            
            const char * teamFishConfig = fishConfig["team"].GetString();
            
            int typeFishConfig = fishConfig["fish"].GetInt();
            
            const char * pathFishConfig = fishConfig["path"].GetString();
            
            const rapidjson::Value& teamConfig = this->teamDocument[teamFishConfig];
            
            const rapidjson::Value& pathConfig = this->pathDocument[pathFishConfig];
            
            for (int index = 0; index < fishInBatchNodes.size(); index++){
                auto t = fishInBatchNodes[index];
                std::set<int>::iterator it = t.find(typeFishConfig);
                
                if (it != t.end())
                {
                    vector<int> _availableFish;
                    auto size = Fish::getFishSize(typeFishConfig);
                    
                    for (int i = 0; i < this->getFishes()->count(); i++)
                    {
                        auto _fish = (Fish *)getFishes()->getObjectAtIndex(i);
                        if (_fish->getFishType() == typeFishConfig && !_fish->getVisible() && !_fish->getDeleted()){
                            _availableFish.push_back(i);
                        }
                        if (_availableFish.size() == teamConfig.Size())
                            break;
                    }
                    
                    if (_availableFish.size() != teamConfig.Size())
                        return;
                    
                    for (rapidjson::SizeType i = 0;i < teamConfig.Size(); i++)
                    {
                        const rapidjson::Value& team = teamConfig[i];
                        
                        Fish* _fish = (Fish *)this->getFishes()->getObjectAtIndex(_availableFish[i]);
                        
                        vector<Point> pts;
                        
                        // Add Fish Path
                        for (rapidjson::SizeType j = 0;j < pathConfig.Size(); j++){
                            const rapidjson::Value& path = pathConfig[j];
                            
                            int xPath = path["x"].GetInt();
                            double xTeam = team["x"].GetDouble() * size.width * 2;
                            double x = xPath + xTeam;
                            
                            double y = path["y"].GetInt() + team["y"].GetDouble() * size.height * 2;
                            pts.push_back(Point(
                                                x,y
                                                )
                                          );
                            
                        }
                        
                        // Init Fish
                        _fish->initParameter(pts);
                        _fish->setIndex(_availableFish[i]);
                        
                        // Add Fish to operation pool
                        m_currentFish.push_back(_availableFish[i]);
                    }
                }
            }
        }
       

	}
}

#define DELTA_ADDFISH 10

void GameLayer::updateFish(float dt)
{
    
    vector< int >::iterator it = m_currentFish.begin();
    
    while(it != m_currentFish.end()) {
        Fish * fish = (Fish *)this->getFishes()->getObjectAtIndex(*it);
        
        if(fish->getDeleted()){
            fish->setDeleted(false);
            fish->setVisible(false);
            it = m_currentFish.erase(it);
        }
        else ++it;
    }
    
    _timeStampAddFish += dt * 20;
    
    if(_timeStampAddFish > _nextTimeStampAddFish){
        int _time = _timeStampAddFish / DELTA_ADDFISH;
        
        // Check current avaible Fish Generation Time
        
         this->setCrtTime(_time);
        
        this->addFish();
        
        _nextTimeStampAddFish = _timeStampAddFish + DELTA_ADDFISH;
        
        if(this->getCrtTime() > 180){
            int randomScript = (int)(CCRANDOM_0_1() * 100) % MAX_SCRIPT_SIZE;
          
            this->setCrtScript(randomScript + 1);
            this->setCrtTime(0);
            _nextTimeStampAddFish = _timeStampAddFish = 0;
        }
    }
}

void GameLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point pt = Director::getInstance()->convertToGL(touch->getLocationInView());
	m_pCannon->setRotate(pt);


}
bool GameLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{

	Point pt = Director::getInstance()->convertToGL(touch->getLocationInView());
	m_pCannon->rotateToPoint(pt);
	isFire = true;


	return true;
}

void GameLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	isFire = false;
}

Rect shrinkRect(Rect rc, float xr, float yr)
{
	float w = rc.size.width * xr;
	float h = rc.size.height * yr;
	Point pt = Vec2(rc.origin.x + rc.size.width * (1.0f - xr) / 2,
		rc.origin.y + rc.size.height * (1.0f - yr) / 2);
	return Rect(pt.x, pt.y, w, h);
}

void GameLayer::updateGame(float dt)
{
    updateFish(dt);
    updateReward(dt);
    updateBullet(dt);
    
}
void GameLayer::fireBullet(float dt){
    if(pause)
        return;
    
     reloadBullet++;

    if (reloadBullet > RELOADTIME)
        reloadBullet = RELOADTIME;
    
    if(!isFire)
        return;

    if(reloadBullet < RELOADTIME)
        return;

    if(User::Instance().getCoin() - m_pCannon->getCost() < 0){
        if(_countFullScreenAdd < 5){
            _countFullScreenAdd ++;
        }

        if(_countFullScreenAdd == 1){
            this->chargeCoin(NULL);

            isFire = false;
        }

        else if(_countFullScreenAdd == 4){
            AdmobHelper::showInterstitial();

            isFire = false;
        }

        return;
    }

    Utils::playEffect("music-affect-laser.mp3");

    m_pCannon->fire();

    this->bonusCoin(-m_pCannon->getCost());
    
    reloadBullet = 0;

}
void GameLayer::updateBullet(float dt)
{
    fireBullet(dt);
   
    
    Ref *pBulletObj = NULL;
    
    
    
    CCARRAY_FOREACH(m_pBullets, pBulletObj)
    {
        Bullet *pBullet = (Bullet *)pBulletObj;
        if (pBullet->getCaught())
            continue;
        bool caught = false;
        bool hit = false;
        
        for(std::vector<int>::iterator it = m_currentFish.begin();it != m_currentFish.end();it++){
            int index = *it;
            Fish * fish = (Fish *)this->getFishes()->getObjectAtIndex(index);
            
            if(!fish->getVisible() && !fish->getDeleted()){
                continue;
            }
            
            Rect hittestRect = shrinkRect(fish->getBounding(), 0.65f, 0.8f);
            
            if (hittestRect.containsPoint(pBullet->getSpriteBullet()->getPosition()))
            {
                fish->hit(pBullet->getPower());
                
                hit = true;
                
                if (fish->getHP() <= 0){
                    
                    caught = true;
                    
                    // Add animation coin
                    
                    this->addCoin(fish->getSprite()->getPositionX(),fish->getSprite()->getPositionY(),fish->getCoin(),"COMBO");
                    
                    auto label = Label::create();
                    if(fish->getIsInstance()){
                        label->setTextColor(Color4B::ORANGE);
                        label->enableOutline(Color4B::WHITE,4);
                        label->setPosition(fish->getSprite()->getPosition());
                        label->setString(StringUtils::format("Perfect \n+ %d",fish->getCoin()));
                        label->setSystemFontSize(26);
                    }
                    else{
                        label->setTextColor(Color4B::YELLOW);
                        label->enableOutline(Color4B::WHITE,3);
                        label->setPosition(fish->getSprite()->getPosition());
                        label->setString(StringUtils::format("+ %d",fish->getCoin()));
                        label->setSystemFontSize(20);
                    }
                    
                    label->setHorizontalAlignment(TextHAlignment::CENTER);
                    label->setVerticalAlignment(TextVAlignment::CENTER);
                    
                    label->runAction(Sequence::create(
                                                      MoveBy::create(0.5f,Vec2(0,10)),
                                                      FadeOut::create(0.5f),
                                                      RemoveSelf::create(true),
                                                      NULL));
                    
                    this->addChild(label);
                    
                    
                    int crtXP = User::Instance().getXP();
                    crtXP ++ ;
                    User::Instance().setXP(crtXP);
                    User::Instance().save();
                    this->updateXP();
                    
                    fish->showCaught();
                    
                }
                
                break;
                
            }
            
        }
        
        
        if (caught)
        {
            pBullet->showNet();
        }
        else
        {
            if (hit)
            {
                pBullet->showSmoke();
            }
        }
    }
}

void GameLayer::addCoin(int x, int y, int bonusCoin,std::string bonusText){
    auto frames = Vector<SpriteFrame *>(9);
    
    Utils::playEffect("music-affect-coin.mp3");

    for (int i = 1; i <= 9; i++)
    {
        auto spriteFrameCache = (SpriteFrameCache :: getInstance()->getSpriteFrameByName(String::createWithFormat("gold%02d.png", i)->getCString()));
        frames.pushBack(spriteFrameCache);
    }
    auto m_spriteCoin = Sprite::createWithSpriteFrameName("gold01.png");
    m_spriteCoin->setPosition(Vec2(x,y));
    auto animation = Animation::createWithSpriteFrames(frames, 0.1f);
    auto 	animate = Animate::create(animation);
    Action *coinAnimation = RepeatForever::create(animate);
    m_spriteCoin->runAction(coinAnimation);
    
    ccBezierConfig benzierConfig;
    benzierConfig.controlPoint_1 = Point(480, 220);
    benzierConfig.controlPoint_2 = Point(380, 220);
    benzierConfig.endPosition = Point(60, 65);
    auto benzier = BezierTo::create(1.5f, benzierConfig);
    auto sequence = Sequence::create(benzier,
                                     FadeOut::create(1.0f),
                                     
                                     CallFunc::create(std::bind(&GameLayer::bonusCoin, this,bonusCoin)),
                                     CallFunc::create(CC_CALLBACK_0(GameLayer::save, this)),
                                     RemoveSelf::create(true),
                                     NULL);
    
    m_spriteCoin->runAction(sequence);
    
    this->getBatchNodeCoin()->addChild(m_spriteCoin);
    
    if(bonusCoin > 100){
        auto bonusCoinLayer = BonusLayer::createWithType(bonusText, bonusCoin);
        bonusCoinLayer->setPosition(Vec2(100,1500));

        bonusCoinLayer->runAction(Sequence::create(
                                                   
                                                   MoveTo::create(0.5f,Vec2(100,0)),
                                                   DelayTime::create(1.0f),
                                                   MoveTo::create(0.5f,Vec2(100,1500)),
                                                   FadeOut::create(1.0f),
                         RemoveSelf::create(true),
                         NULL));
//
        Utils::playEffect("music-affect-perfect.mp3");

        this->addChild (bonusCoinLayer,99999,0);
    }
}
void GameLayer::save()
{
	User::Instance().save();
}

void GameLayer::bonusCoin(int coin)
{
	User::Instance().setCoin(User::Instance().getCoin() + coin);
	m_pRollNumGroup->setValue(User::Instance().getCoin());

}

void GameLayer::changeRightCannon(Ref* ref){
	int index = this->getCrtCannon() + 1;
	if (index >= MAX_CANNON_SIZE)
		index = 0;
	changeCannon(index);
}
void GameLayer::changeLeftCannon(Ref* ref){
	int index = this->getCrtCannon() - 1;
	if (index < 0)
		index = MAX_CANNON_SIZE - 1;
	changeCannon(index);
}
void GameLayer::changeCannon(int index){
	this->setCrtCannon(index);
	this->getCannon()->removeSelf();
	this->setCannon(Cannon::createWithCannonType(m_crtCannon + 1, this, m_pBatchNodeCannon));
}

void GameLayer::chargeCoin(Ref* ref)
{
    
    openPopup(1000);
    
}
void GameLayer::openPopup(int type){
     auto child = this->getChildByTag(type);

    if(child != nullptr)
        return;
    
    if(pause)
        return;
    
    pause = true;
    Utils::playEffect("music-affect-begin.mp3");
    
    if(type == 1000){
        auto popup = ChargingLayer::create();
        this->addChild(popup,type);
    }
    else if(type == 2000){
        auto contextLayer = ContextLayer::create();
        this->addChild(contextLayer,type);
    }
    else{
        
    }
    
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this);
    
}
void GameLayer::closePopup(Ref *ref){
    pause = false;
    
    Utils::playEffect("music-affect-begin.mp3");
    
    ((Node *)ref)->removeFromParentAndCleanup(true);
    
    Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this);
}
void GameLayer::colectReward(Ref* ref)
{
    if (!canGetReward)
        return;
    
    User::Instance().setLastTimeUpdate();
    
    int bonusCoin = ScriptConfig::getInstance()->document["reward"]["bonusCoin"].GetInt();
    
    
    Vec2 pos = lbRewardTimer->convertToWorldSpace (lbRewardTimer->getPosition());
    
    this->addCoin(pos.x,pos.y,bonusCoin,"LOGIN");
    
    this->bonusCoin(bonusCoin);
}
void GameLayer::onHttpInitComplete(rapidjson::Document* document)
{
    bool error = (* document)["error"].GetBool();
    
    // Connection check
    if(!error){
        log("Kết nối thành công !!!");
        
        // Convert JSON document to string
  
        rapidjson::Value& data = (* document)["data"];
        
        if(data.HasMember("is_payment")){
            bool isPayment = data["is_payment"].GetBool();
            
            chargeCoinBtn->setVisible(isPayment);
        }
        
    }
    else{
       log("Kết nối thất bại !!!");
    }
}
void GameLayer::menuUpdate(Ref *){
    this->openPopup(2000);
}
void GameLayer::updateXP(){
    
    int nextXP;
    int bonusCoin;
    int preXP;
    int crtLevel = User::Instance().getLevel();
    int crtXP = User::Instance().getXP();
    
    int nextLevel = crtLevel +1 ;
    nextXP = (User::Instance().getLevel() )  * 2000;
    bonusCoin = 1000;
    preXP = (User::Instance().getLevel() - 1)  * 2000;

    
    if(ScriptConfig::getInstance()->document.HasMember("level")){
        rapidjson::Value& levelConfig = ScriptConfig::getInstance()->document["level"];
        
        if(levelConfig.HasMember(StringUtils :: format("%d",(nextLevel)).c_str())){
            nextXP = levelConfig[StringUtils :: format("%d",(nextLevel)).c_str()]["nextXP"].GetInt();
            
            
            
            bonusCoin = levelConfig[StringUtils :: format("%d",nextLevel).c_str()]["bonus"].GetInt();
        }
        
        if(levelConfig.HasMember(StringUtils :: format("%d",(nextLevel - 1)).c_str())){
            
            preXP = levelConfig[StringUtils :: format("%d",(nextLevel - 1)).c_str()]["nextXP"].GetInt();
        }
        
        
    }
    
    if(nextXP <= crtXP){
        this->addCoin(50 , 50, bonusCoin, "LEVEL UP !!!");
        User::Instance().setLevel(nextLevel);
        User::Instance().save();
        lbLevel->setString(StringUtils::format("%d",nextLevel));
    }
    int percent =(crtXP  - preXP) * 100 / (nextXP - preXP);
    processBarXP->setPercentage(percent);
}
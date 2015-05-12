//
//  GameLayer.h
//  FishingJoy
//
//  Created by jie ding on 11/28/12.
//
//

#ifndef __FishingJoy__GameLayer__
#define __FishingJoy__GameLayer__

#include <iostream>
#include "cocos2d.h"
#include "GameConfig.h"
#include "Cannon.h"
#include "RollNum.h"
#include "cocostudio/CocoStudio.h"
#include "NetworkManagement.h"
#include "Shake.h"
#include "AdmobHelper.h"

class GameLayer : public cocos2d::Layer
{
public:
	// Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
	GameLayer();
	virtual ~GameLayer();
	virtual bool init();


	// there's no 'id' in cpp, so we recommend to return the class instance pointer
	static cocos2d::Scene* scene();

	// preprocessor macro for "static create()" constructor ( node() deprecated )
	CREATE_FUNC(GameLayer);
	CC_SYNTHESIZE(cocos2d::SpriteBatchNode *, m_pBatchNode1, BatchNode1);
	CC_SYNTHESIZE(cocos2d::SpriteBatchNode *, m_pBatchNode2, BatchNode2);
	CC_SYNTHESIZE(cocos2d::SpriteBatchNode *, m_pBatchNode3, BatchNode3);
	CC_SYNTHESIZE(cocos2d::SpriteBatchNode *, m_pBatchNode4, BatchNode4);
	CC_SYNTHESIZE(cocos2d::SpriteBatchNode *, m_pBatchNode5, BatchNode5);

	CC_SYNTHESIZE(std::vector<cocos2d::SpriteBatchNode *>, m_pBatchNodeFishs, BatchNodeFishs);

	CC_SYNTHESIZE(cocos2d::SpriteBatchNode *, m_pBatchNodeBullet, BatchNodeBullet);
	CC_SYNTHESIZE(cocos2d::SpriteBatchNode *, m_pBatchNodeCoin, BatchNodeCoin);

	CC_SYNTHESIZE(cocos2d::SpriteBatchNode *, m_pBatchNodeCannon, BatchNodeCannon);
    CC_SYNTHESIZE(std::vector<int>, m_currentFish, CurrentFish);
    
	CC_SYNTHESIZE_RETAIN(cocos2d::Array *, m_pBullets, Bullets);
	CC_SYNTHESIZE_RETAIN(cocos2d::Array *, m_pFishes, Fishes);
	CC_SYNTHESIZE_RETAIN(Cannon *, m_pCannon, Cannon);
	CC_SYNTHESIZE_RETAIN(RollNumGroup *, m_pRollNumGroup, RollNumGroup);

	CC_SYNTHESIZE(int, m_crtCannon, CrtCannon);

	void save();
	cocos2d::ProgressTimer * processBarXP;
    cocos2d::ProgressTimer * processBarReward;
    
    
    CC_SYNTHESIZE(int , m_crtTime, CrtTime);
    CC_SYNTHESIZE(int , m_crtScript, CrtScript);
    
    
    void bonusCoin(int coin);
    
    void addCoin(int x, int y, int bonusCoin,std::string bonusText);

    
    void chargeCoin(Ref* obj);
    void colectReward(Ref* );
    void menuUpdate(Ref *);
    
    void openPopup(int type);
    void closePopup(Ref * ref);
    
private:
	std::vector<std::set<int>> fishInBatchNodes;
    

	int m_nScore;
    bool m_show_charge;
	void initFishes();
	void initBackground();
	void initCannon();
	void initFrames();
	void initButtons();
    void initRewards();
    void initNetwork();
    void initMusic();
	void updateGame(float dt);
	void updateFish(float dt);
    void updateReward(float dt);
    void updateBullet(float dt);
    void fireBullet(float dt);
    
    void updateXP();
	void addFish();

	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	int spawnTime;
	std::vector<int> pathPool;
	bool isFire;
    bool canGetReward;
	float reloadBullet;

    double _timeStampAddFish;
    double _nextTimeStampAddFish;
    
    rapidjson::Value scriptDocument;
    rapidjson::Value teamDocument;
    rapidjson::Value pathDocument;
    
    Label *lbRewardTimer;
    Label *lbLevel;
    Menu *menu;
    MenuItemSprite * chargeCoinBtn;
    MenuItemSprite * rewardButton;
    
    void changeCannon(int index);
    void changeLeftCannon(Ref* obj);
    void changeRightCannon(Ref* obj);
    void onHttpInitComplete(rapidjson::Document* document);
    
    bool addRewardAni = false;

    int _countFullScreenAdd = 0;
    bool _openedFullScreenAdd = false;
    bool pause = false;
};

#endif /* defined(__FishingJoy__GameLayer__) */

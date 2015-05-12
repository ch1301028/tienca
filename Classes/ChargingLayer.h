#ifndef __CHARGE_SCENE_H__
#define __CHARGE_SCENE_H__
#include "cocos2d.h"
#include "GameLayer.h"
#include "network/HttpClient.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"   
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;
using namespace ui;
using namespace cocos2d::extension;
using namespace cocos2d::network;

class ChargingLayer :public cocos2d::LayerColor
{
public:
	// Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
	ChargingLayer();
	virtual ~ChargingLayer();
	virtual bool init();
	CREATE_FUNC(ChargingLayer);
	static Scene* createScene();
    CC_SYNTHESIZE(int, m_bonus_coin, BonusCoin);
    
    
    void onRequestCoinComplete(rapidjson::Document* document);
    void onRequestServerCoinComplete(rapidjson::Document* document);
    void onRequestUserDataComplete(rapidjson::Document* document);
    
    void callBackSms(Node *sender, Value data);
    
private:
	Node *					rootNode;
	string						m_crtProvider;

	void initProviderPanel();
	void initPricePanel();
	void initChargePanel();
    void initSmsPanel();
    
	void onProvider(string name);
    void onSms(int amount);
    
    void onCardTab();
	void onFreeTab();
    void onSmsTab();
    void onPrice();
    void onClose();
    void onBagTab();
    void onUpdateUserData();
    void onRequestServerCoin();
    void updateServerCoin();
	void onCharge();

	void switchPanel(string panelName);
    void enableTabs(string btnName);

	EditBox *eSeri;
	EditBox *eCode;
    Label *lbResult;
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void onTouchCallBack(Ref *ref, Widget::TouchEventType);


};
#endif
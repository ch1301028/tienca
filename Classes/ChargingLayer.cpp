#include "ChargingLayer.h"

#include "ConfigManagement.h"
#include "ResourceManager.h"
#include "NetworkManagement.h"
#include "User.h"
#include "ScriptConfig.h"
#include "Utils.h"
#include "NDKHelper/NDKHelper.h"
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

const string panels_name[] = {
	"panel_card_price",
	"panel_card_charge",
	"panel_card_provider",
    "panel_sms_price",
    "panel_bag"
};

const string tabs_name[] = {
    "btn_sms",
    "btn_card",
    "btn_free",
    "btn_bag"
};
Scene* ChargingLayer::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = ChargingLayer::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

ChargingLayer::ChargingLayer():
m_bonus_coin(0)
{
    
}
ChargingLayer::~ChargingLayer()
{
    
}
bool ChargingLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0,0,0,200)))
	{
		return false;
	}

	Size winSize = Director::getInstance()->getWinSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	rootNode =  CSLoader::createNode("MainScene.csb");
    this->addChild( rootNode );
    
	rootNode->setPosition(Vec2(winSize.width / 2 - rootNode->getContentSize().width / 2, winSize.height / 2 - rootNode->getContentSize().height/2));
	
	auto freeBtn = ( cocos2d::ui::Button * )(rootNode->getChildByName("btn_free"));
    freeBtn->addTouchEventListener(CC_CALLBACK_2(ChargingLayer::onTouchCallBack, this));
    
	auto smsBtn = ( cocos2d::ui::Button * )(rootNode->getChildByName("btn_sms"));
    smsBtn->addTouchEventListener(CC_CALLBACK_2(ChargingLayer::onTouchCallBack, this));
    
	auto cardBtn = ( cocos2d::ui::Button * )(rootNode->getChildByName("btn_card"));
	cardBtn->addTouchEventListener(CC_CALLBACK_2(ChargingLayer::onTouchCallBack, this));
    
    auto smsPanelBtn = ( cocos2d::ui::Button * )(rootNode->getChildByName("panel_bag")->getChildByName("btn_bag_sms"));
    smsPanelBtn->addTouchEventListener(CC_CALLBACK_2(ChargingLayer::onTouchCallBack, this));
    
    auto cardPanelBtn = ( cocos2d::ui::Button * )(rootNode->getChildByName("panel_bag")->getChildByName("btn_bag_card"));
    cardPanelBtn->addTouchEventListener(CC_CALLBACK_2(ChargingLayer::onTouchCallBack, this));
    
    auto updateBtn = ( cocos2d::ui::Button * )(rootNode->getChildByName("panel_bag")->getChildByName("btn_server"));
    updateBtn->addTouchEventListener(CC_CALLBACK_2(ChargingLayer::onTouchCallBack, this));
    
    auto btn_bag = ( cocos2d::ui::Button * )(rootNode->getChildByName("btn_bag"));
    btn_bag->addTouchEventListener(CC_CALLBACK_2(ChargingLayer::onTouchCallBack, this));
    
	auto closeBtn = ( cocos2d::ui::Button * )(rootNode->getChildByName("btn_close"));
	closeBtn->addTouchEventListener(CC_CALLBACK_2(ChargingLayer::onTouchCallBack, this));
    
	initProviderPanel();
	initChargePanel();
	initPricePanel();
    initSmsPanel();

	switchPanel("panel_sms_price");
    enableTabs("btn_sms");
    
    this->updateServerCoin();
    this->onUpdateUserData();
    
	return true;
}

void ChargingLayer::initPricePanel(){
    auto panel = ( cocos2d::ui::Layout * )(rootNode->getChildByName("panel_card_price"));
    
    auto scrollView = ( cocos2d::ui::ScrollView * )panel->getChildByName("scroll_card_price");
    
    auto providersBtn = scrollView->getChildren();
    for (int i = 0; i < providersBtn.size(); i++){
        Button * btn = (Button*) providersBtn.at(i);
        btn->addTouchEventListener(CC_CALLBACK_2(ChargingLayer::onTouchCallBack, this));
    }
}

void ChargingLayer::initProviderPanel(){
    auto providerPanel = ( cocos2d::ui::Layout * )(rootNode->getChildByName("panel_card_provider"));
    
    auto scrollView = ( cocos2d::ui::ScrollView * )providerPanel->getChildByName("scroll_card_provider");
    
    auto providersBtn = scrollView->getChildren();
    for (int i = 0; i < providersBtn.size(); i++){
        Button * btn = (Button*) providersBtn.at(i);
        btn->addTouchEventListener(CC_CALLBACK_2(ChargingLayer::onTouchCallBack, this));
    }
}

void ChargingLayer::initSmsPanel(){
    auto panel = ( cocos2d::ui::Layout * )(rootNode->getChildByName("panel_sms_price"));
    
    auto scrollView = ( cocos2d::ui::ScrollView * )panel->getChildByName("scroll_sms_price");
    
    auto providersBtn = scrollView->getChildren();
    for (int i = 0; i < providersBtn.size(); i++){
        Button * btn = (Button*) providersBtn.at(i);
        btn->addTouchEventListener(CC_CALLBACK_2(ChargingLayer::onTouchCallBack, this));
    }
    
    NDKHelper::addSelector("SmsSelector",
                           "callBackSms",
                           CC_CALLBACK_2(ChargingLayer::callBackSms, this),
                           this);
}

void ChargingLayer::initChargePanel(){
    Size winSize = Director::getInstance()->getWinSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    auto chargePanel = rootNode->getChildByName("panel_card_charge");
    
	auto mpSeri = static_cast<TextField*>(chargePanel->getChildByName("seri"));
	auto mpCode = static_cast<TextField*>(chargePanel->getChildByName("code"));
    auto btnCharge = static_cast<Button*>(chargePanel->getChildByName("btn_charge"));
    
    btnCharge->addTouchEventListener(CC_CALLBACK_2(ChargingLayer::onTouchCallBack, this));
    
    mpSeri->setVisible(false);
	mpCode->setVisible(false);

	eSeri = EditBox::create(Size(500, 50), Scale9Sprite::create("edit-box.png"));
	eCode = EditBox::create(Size(500, 50), Scale9Sprite::create("edit-box.png"));
    eSeri->setPlaceHolder("Số Seri");
    eCode->setPlaceHolder("Mã thẻ cào");
    eSeri->setPosition(Vec2(chargePanel->getContentSize().width / 2 , chargePanel->getContentSize().height* 3/4 + 70));
    eCode->setPosition(Vec2(chargePanel->getContentSize().width / 2 , chargePanel->getContentSize().height* 3/4  ));
	eSeri->setFontColor(Color3B::WHITE);
	eCode->setFontColor(Color3B::WHITE);
    
	chargePanel->addChild(eSeri);
	chargePanel->addChild(eCode);
    
    lbResult = Label::create();
    lbResult->setSystemFontSize(30);
    lbResult->setWidth(400);
    lbResult->setString("");
    lbResult->setPosition(Vec2(eSeri->getPositionX(), eCode->getPositionY() - 105));
    chargePanel->addChild(lbResult);
}



void ChargingLayer::onProvider(string name){

	m_crtProvider = name;

	switchPanel("panel_card_charge");
}

void ChargingLayer::switchPanel(string panelName){	
	for (int i = 0; i < GET_ARRAY_LEN(panels_name); i++){
		auto panel = ( cocos2d::ui::Layout * )(rootNode->getChildByName(panels_name[i].c_str()));
		panel->setVisible(false);
	}

	auto panelSelected = static_cast<Layout*>(rootNode->getChildByName(panelName.c_str()));
	panelSelected->setVisible(true);
}
void ChargingLayer::onPrice()
{
    switchPanel("panel_card_provider");
}


void ChargingLayer::onSms(int amount){
    this->onBagTab();
    Utils::openSms(amount);
    
}
void ChargingLayer::onSmsTab(){
    enableTabs("btn_sms");
    switchPanel("panel_sms_price");
}
void ChargingLayer::onBagTab(){
    enableTabs("btn_bag");
    switchPanel("panel_bag");
}

void ChargingLayer::onCardTab(){

    enableTabs("btn_card");
	switchPanel("panel_card_price");
}
void ChargingLayer::onRequestServerCoin(){
    NetworkManagement::getInstance().requestServerCoin(CC_CALLBACK_1(ChargingLayer::onRequestServerCoinComplete,this));
}
void ChargingLayer::onUpdateUserData(){
      NetworkManagement::getInstance().requestUserData(CC_CALLBACK_1(ChargingLayer::onRequestUserDataComplete,this));
}
void ChargingLayer::enableTabs(string btn_name){
    for (int i = 0; i < GET_ARRAY_LEN(tabs_name); i++){
        auto btn = ( cocos2d::ui::Button * )(rootNode->getChildByName(tabs_name[i].c_str()));
        
        if(btn_name == btn->getName()){
            auto particle = rootNode->getChildByName("effect_btn");
            particle->setPosition(btn->getPosition());
            btn->setBright(true);
        }
        else{
            btn->setBright(false);
        }
    }
}

void ChargingLayer::onFreeTab(){
	
    return ;
}
void ChargingLayer::onClose(){
    auto gameLayer = (GameLayer *)this->getParent();
    gameLayer->bonusCoin(this->getBonusCoin());
    gameLayer->closePopup(this);
    
}

void ChargingLayer::onCharge(){
	
    lbResult->setString("...");
	
	string mSeri = eSeri->getText();
	string mCode = eCode->getText();
	// Verify Data
	if (mSeri.length() == 0 || mCode.length() == 0){		
		return;
	}	
    
    auto closeBtn = static_cast<Button*>(rootNode->getChildByName("btn_close"));
    closeBtn->setEnabled(false);
    
    NetworkManagement::getInstance().requestCoin(CC_CALLBACK_1(ChargingLayer::onRequestCoinComplete,this), mSeri, mCode,m_crtProvider);

}
void ChargingLayer::callBackSms(Node *sender, Value data)
{
    string device_id = Utils::getUniqueId();

    NetworkManagement::getInstance().requestSmsCoin( CC_CALLBACK_1(ChargingLayer::onRequestCoinComplete,this),device_id);
}

void ChargingLayer::onRequestCoinComplete(rapidjson::Document* document)
{
    auto closeBtn = static_cast<Button*>(rootNode->getChildByName("btn_close"));
    closeBtn->setEnabled(true);
    
    bool error = (* document)["error"].GetBool();
    
    // Connection check
    if(!error){
        lbResult->setString("Kết nối thành công !!!");
        
        int errorCode = (* document)["data"]["err"].GetInt();
        
        // Charging check
        if(errorCode != 200 ){
            std::string errorMessage = (* document)["data"]["description"].GetString();
            lbResult->setString(errorMessage);
            return;
        }
        
        int bonusCoin = (* document)["data"]["amount"].GetInt();
        User :: Instance() . setCoin(User :: Instance() . getCoin() + bonusCoin);
        std::string errorMessage = (* document)["data"]["description"].GetString();
        lbResult->setString(errorMessage);
    }
    else{
        lbResult->setString("Kết nối thất bại !!!");
    }
}

void ChargingLayer::onRequestServerCoinComplete(rapidjson::Document* document)
{
    auto closeBtn = static_cast<Button*>(rootNode->getChildByName("btn_close"));
    closeBtn->setEnabled(true);
    
    bool error = (* document)["error"].GetBool();
    
    // Connection check
    if(!error){
        
        int errorCode = (* document)["data"]["err"].GetInt();
        
        // Charging check
        if(errorCode != 200 ){
            std::string errorMessage = (* document)["data"]["description"].GetString();
            lbResult->setString(errorMessage);
            return;
        }
        
        User::Instance().parseServerData(((* document)["data"]["user"]));
        
        updateServerCoin();
    }
    else{
  
    }
}

void ChargingLayer::onRequestUserDataComplete(rapidjson::Document* document)
{
    auto closeBtn = static_cast<Button*>(rootNode->getChildByName("btn_close"));
    
    closeBtn->setEnabled(true);
    
    bool error = (* document)["error"].GetBool();
    
    // Connection check
    if(!error){
        
        int errorCode = (* document)["data"]["err"].GetInt();
        
        // Charging check
        if(errorCode != 200 ){
            std::string errorMessage = (* document)["data"]["description"].GetString();
            lbResult->setString(errorMessage);
            return;
        }
        
        User::Instance().parseServerData(((* document)["data"]["user"]));
        
        updateServerCoin();
    }
    else{
        
    }
}

void ChargingLayer::updateServerCoin(){
    
    
    cocos2d::ui::TextBMFont * coinLabel = ( cocos2d::ui::TextBMFont * )rootNode->getChildByName("panel_bag")->getChildByName("client_coin");
    
  coinLabel->setString(StringUtils::format("%d" , User::Instance().getCoin()));
    
    cocos2d::ui::TextBMFont * serverCoinLabel = ( cocos2d::ui::TextBMFont * )rootNode->getChildByName("panel_bag")->getChildByName("server_coin");
    
    serverCoinLabel->setString(StringUtils::format("%d" , User::Instance().getServerCoin()).c_str());
}

void ChargingLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    
}
bool ChargingLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    

    return true;
}

void ChargingLayer::onTouchCallBack(Ref *ref, Widget::TouchEventType type){
    if(type != Widget::TouchEventType::ENDED)
        return ;
    
    auto target = (cocos2d::ui::Button *) ref;
    
    string name = target->getName();
    
    if(name == "btn_card" || name == "btn_bag_card"){
        this->onCardTab();
    }
    else if(name == "btn_sms" || name == "btn_bag_sms"){
        this->onSmsTab();
    }
    else if(name == "btn_free"){
        this->onFreeTab();
    }
    else if(name == "btn_charge" ){
        this->onCharge();
    }
    else if(name == "btn_bag"){
        this->onBagTab();
    }
    else if(name == "btn_close"){
        
        this->onClose();
    }
    else if(name == "btn_server"){
        this->onRequestServerCoin();
    }
    else if(name == "card_10000" ||
            name == "card_20000" ||
            name == "card_50000" ||
            name == "card_60000" ||
            name == "card_100000" ||
            name == "card_120000"){
        this->onPrice();
    }
    else if(name == "vinaphone" ||
            name == "mobifone" ||
            name == "viettel" ||
            name == "zing" ||
            name == "gate" ||
            name == "vcoin" ){
        this->onProvider(name);
    }
    else if(name == "sms_5000"){
        this->onSms(5000);
    }
    else if(name == "sms_10000"){
        this->onSms(10000);
    }
    else if(name == "sms_15000"){
        this->onSms(15000);
    }
    else if(name == "sms_20000"){
        this->onSms(20000);
    }
    else if(name == "sms_25000"){
        this->onSms(25000);
    }
    else if(name == "sms_30000"){
        this->onSms(30000);
    }
    else if(name == "sms_40000"){
        this->onSms(40000);
        
    }
    else{
        
    }
    
    
    
    CCLOG("touch me btn: %s" , name.c_str());
}

void ChargingLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
   
}
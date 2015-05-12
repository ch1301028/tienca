#include "ContextLayer.h"
#include "User.h"
#include "Utils.h"

Scene* ContextLayer::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = ContextLayer::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

ContextLayer::ContextLayer()
{
    
}
ContextLayer::~ContextLayer()
{
    
}


bool ContextLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0,0,0,200)))
	{
		return false;
	}

	Size winSize = Director::getInstance()->getWinSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

    menu = Menu::create();

    auto labelCharge= Label::create();
    labelCharge->setString("Nạp thẻ");
    labelCharge->setTextColor(Color4B::ORANGE);
    labelCharge->enableOutline(Color4B::WHITE,4);
    labelCharge->setSystemFontSize(60);

    auto itemCharge = MenuItemLabel::create(
                                    labelCharge, 
                                    CC_CALLBACK_1(ContextLayer::onCharge,this));

    menu->addChild(itemCharge);

    auto labelMusic= Label::create();
    labelMusic->setString(User::Instance().getMusic() ? "Âm thanh: ON" : "Music: OFF");
    labelMusic->setTextColor(Color4B::ORANGE);
    labelMusic->enableOutline(Color4B::WHITE,4);
    labelMusic->setSystemFontSize(60);
    
    auto itemMusic = MenuItemLabel::create(
                                    labelMusic, 
                                    CC_CALLBACK_1(ContextLayer::updateMusic,this));


    menu->addChild(itemMusic);

    auto labelContact= Label::create();
    labelContact->setString("Liên hệ");
    labelContact->setTextColor(Color4B::ORANGE);
    labelContact->enableOutline(Color4B::WHITE,4);
    labelContact->setSystemFontSize(60);

    auto itemContact = MenuItemLabel::create(
                                    labelContact, 
                                    CC_CALLBACK_1(ContextLayer::onContactMe,this));


    menu->addChild(itemContact);


    auto labelExit= Label::create();
    labelExit->setString("Thoát");
    labelExit->setTextColor(Color4B::ORANGE);
    labelExit->enableOutline(Color4B::WHITE,4);
    labelExit->setSystemFontSize(60);

    auto itemExit = MenuItemLabel::create(
                                    labelExit, 
                                    CC_CALLBACK_1(ContextLayer::onHide,this));


    menu->addChild(itemExit);

    int i =0;
    for (Node *obj : menu->getChildren()) {
        auto item = (MenuItemLabel *)obj;
        item->setPosition(Vec2(winSize.width/2, 500 - i * 80 ));
        i ++;
    }
    menu->setPosition(Vec2(0,0));
    this->addChild(menu);
    
	return true;
}

void ContextLayer::onHide(Ref *ref)
{
    auto gameLayer = (GameLayer *)this->getParent();
    gameLayer->closePopup(this);
}

void ContextLayer::onCharge(Ref *ref){
    
    auto gameLayer = (GameLayer *)this->getParent();
    
    gameLayer->closePopup(this);
    
    gameLayer->chargeCoin(NULL);

  }

void ContextLayer::onContactMe(Ref *ref){
    Utils::openUrl("https://www.facebook.com/bantienca");
}

void ContextLayer::updateMusic(Ref *ref){
    User::Instance() . setMusic(!User::Instance()  . getMusic());

    Utils::updateMusic(User::Instance() . getMusic());

    MenuItemLabel * item = (MenuItemLabel *)ref;

    ((Label * )item->getLabel())->setString(User::Instance() . getMusic() ? "Âm thanh: ON" : "Âm thanh: OFF");
} 
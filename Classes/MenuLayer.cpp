#include "MenuLayer.h"
#include "ChargingLayer.h"
#include "GameLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ConfigManagement.h"
#include "Utils.h"
using namespace cocostudio;

Scene* MenuLayer::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MenuLayer::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}
MenuLayer::MenuLayer()
{
}
MenuLayer::~MenuLayer()
{
}
bool MenuLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

	ConfigManagement::getInstance()->load();

	Size winSize = Director::getInstance()->getWinSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

    
    Sprite *pBackground = Sprite::create("bj02.jpg");      
	pBackground->setPosition(Vec2(origin.x + winSize.width/2,origin.y + winSize.height/2));

    this->addChild(pBackground);	
	
	Menu *pMenu = CCMenu::create();

	Texture2D *textbtnPlay = TextureCache::sharedTextureCache()->addImage("play.png");
	
	 // load resource directly
	ArmatureDataManager::getInstance()->addArmatureFileInfo("TronAni.ExportJson");

    Armature *armature = Armature::create("TronAni");
    
    armature->getAnimation()->playByIndex(0);
	armature->setPosition(Vec2(winSize.width - armature->getContentSize().width/2 ,
		armature->getContentSize().height/2 - 20));
    this->addChild(armature);
	
    MenuItemSprite *pPlay = MenuItemSprite ::create(
		Sprite::createWithTexture(textbtnPlay),
		Sprite::createWithTexture(textbtnPlay),
        CC_CALLBACK_1(MenuLayer::Play, this));


	pPlay->setPosition(Vec2(winSize.width - armature->getContentSize().width/2,
		armature->getContentSize().height/2 - 18));
	pMenu->setPosition(Vec2(0,0));
	pMenu->addChild(pPlay);
	
	this->addChild(pMenu);		
	
	return true;
}

void MenuLayer::Play(Ref *sender)
{

	Utils::playEffect("music-affect-begin.mp3");

	Scene *pScene = CCTransitionFade::create(0.9f, GameLayer::scene());
    
	Director::getInstance()->replaceScene(pScene);	
}

#include "BonusLayer.h"

#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocostudio;

BonusLayer::BonusLayer()
{
    
}
BonusLayer::~BonusLayer()
{
    
}
bool BonusLayer::initWithType(std::string bonusType,int bonusCoin)
{
    if (!LayerColor::initWithColor(Color4B(0,0,0,0)))
    {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
   
    auto rootNode =  CSLoader::createNode("RewardScene.csb");
    this->addChild( rootNode );
    
    cocos2d::ui::TextBMFont * title = ( cocos2d::ui::TextBMFont * )rootNode->getChildByName("reward_label");
    title->setString(bonusType);
    
    
    cocos2d::ui::TextBMFont * bonusText = ( cocos2d::ui::TextBMFont * )rootNode->getChildByName("bonus_label");
    bonusText->setString(StringUtils::format("+ %d" , bonusCoin).c_str());
    return true;
}
BonusLayer* BonusLayer::createWithType(std::string bonusType,int bonusCoin)
{
    BonusLayer *pLayer = new BonusLayer();
    
    if(pLayer && pLayer->initWithType(bonusType,bonusCoin))
    {
        pLayer->autorelease();
        return pLayer;
    }
    else
    {
        CC_SAFE_DELETE(pLayer);
        return NULL;
    }
}

void BonusLayer::onClose(){
    this->removeFromParentAndCleanup(true);
}
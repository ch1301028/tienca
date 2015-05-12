#ifndef __CONTEXT_SCENE_H__
#define __CONTEXT_SCENE_H__
#include "cocos2d.h"
#include "GameLayer.h"

using namespace cocos2d;

class ContextLayer :public cocos2d::LayerColor
{
public:
	// Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
	ContextLayer();
	virtual ~ContextLayer();
	virtual bool init();
	CREATE_FUNC(ContextLayer);
	static Scene* createScene();
private:
	Menu * menu;
	void onShow();
	void onHide(Ref *);
	void onCharge(Ref *);
	void updateMusic(Ref *);
	void onContactMe(Ref *ref);
};
#endif
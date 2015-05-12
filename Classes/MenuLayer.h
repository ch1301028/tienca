#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__
#include "cocos2d.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class MenuLayer:public cocos2d::Layer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    MenuLayer();
    virtual ~MenuLayer();
    virtual bool init();
	CREATE_FUNC(MenuLayer);
	static Scene* createScene();
private:
	void Play(Ref*);
};
#endif
 //
//  Fish.h
//  FishingJoy
//
//  Created by jie ding on 11/28/12.
//
//

#ifndef __FishingJoy__Fish__
#define __FishingJoy__Fish__

#include <iostream>
#include "cocos2d.h"
#include "GameLayer.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;
using namespace cocos2d;
class Fish : public cocos2d::Ref
{
public:
    virtual ~Fish();
	static Size getFishSize(int type);
	static Fish *createWithFishType(int fishType, GameLayer *gameLayer,  SpriteBatchNode*);
    CC_SYNTHESIZE(GameLayer *, m_pGameLayer, GameLayer);
    CC_SYNTHESIZE(cocos2d::SpriteBatchNode *, m_pBatchNode, BatchNode);
    CC_SYNTHESIZE(int, m_nFishType, FishType);
	CC_SYNTHESIZE(Sprite *, m_pSprite, Sprite);
	CC_SYNTHESIZE(Armature *, m_armature, Armature);
    CC_SYNTHESIZE_READONLY(bool, m_bCaught, Caught);
	

	CC_SYNTHESIZE(int, m_hp, HP);
	CC_SYNTHESIZE(int, m_coin, Coin);
	CC_SYNTHESIZE(int, m_instant_rate, InstantRate);

	CC_SYNTHESIZE(std::vector <Point>, m_points, Points);
	CC_SYNTHESIZE(int, m_crtPoint, CrtPoint);
	CC_SYNTHESIZE(int, m_index, Index);
	CC_SYNTHESIZE(Point, m_nextPoint, NextPoint);
	CC_SYNTHESIZE(Point, m_previousPoint, PreviousPoint);
    CC_SYNTHESIZE(bool,m_visible , Visible);
    CC_SYNTHESIZE(bool,m_deleted, Deleted);
    CC_SYNTHESIZE(bool,m_isInstant, IsInstance);
    void showCaught();	
	void hit(int power);
	Rect getBounding();
	void setAlive(bool status);

	void initParameter(std::vector<Point> pts);
private:
	bool initWithFishType(int fishType, GameLayer *gameLayer, SpriteBatchNode*);
    void removeSelf();
    void getPath();
    bool m_bParticleBubble;	
    bool _hit;
	bool isInit;
	Vector<SpriteFrame*>  _animFrames;
    
    Vector<Point>   generatePath(Vector<Point>);
    Vector<Point>   smoothingPath(Vector<Point> path,float weight_data , float max_error,float weight_smooth);
    Point           convertPtsToGrid(Point);
};

typedef struct _VecBezierConfig {
    //! end position of the bezier
    vector<float> bezierPtsConfigX;
    vector<float> bezierPtsConfigY;
    Point _startPoint;
} vecBezierConfig;


class ZBezierBy : public BezierTo
{
public:
    
    vecBezierConfig _vec;

    static long* factorials;
    
    bool initWithDuration(float t, const vecBezierConfig &vec )
    {
        if (ActionInterval::initWithDuration(t))
        {
            _vec = vec;
        }
        return false;
    }
    
    static ZBezierBy* actionWithDuration(float t, const vecBezierConfig &vec );
    
    static float calculateBenzier(const vector<float>& p,float dt){
        
        float result = 0;
        long n = p.size() - 1;
        for(int i=0;i  <= n;i++){
            long fN = factorial(n) ;
            long fI = factorial(i);
            long fNI = factorial(n-i);
            long ni = fN / (fI * fNI );
            result +=  ni * pow( 1 - dt, n-i) * pow(dt,i) * p[i];
        }
        return result;
    }
    
    static unsigned long factorial(long n);
    
   	virtual void update(float t)
	{
		Point oldpos = getTarget()->getPosition();
        
        if (_target)
        {
            float x = calculateBenzier(_vec.bezierPtsConfigX, t);
            float y = calculateBenzier(_vec.bezierPtsConfigY, t);
            
            _target->setPosition(  Vec2(x,y));
        
        
        }
    
		Point newpos = getTarget()->getPosition();
        
		float angle = 180 -  atan2(-newpos.y + oldpos.y,- newpos.x + oldpos.x) * 180 / M_PI;
        
		getTarget()->setRotation(angle);//*  180);
	}
};

#endif /* defined(__FishingJoy__Fish__) */

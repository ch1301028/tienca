#ifndef __SHAKE_H__
#define __SHAKE_H__

#include "cocos2d.h"
using namespace cocos2d;

class Shake : public ActionInterval
{
   public:
    Shake();
    
    // Create the action with a time and a strength (same in x and y)
    static Shake* create(float d, float strength );
    // Create the action with a time and strengths (different in x and y)
    static Shake* create(float d, float strength_x, float strength_y );
    bool initWithDuration(float d, float strength_x, float strength_y );
    
    virtual void startWithTarget(Node *pTarget);
    virtual void update(float time);
    virtual void stop(void);
    
protected:
    // Initial position of the shaked node
    float _initial_x, _initial_y;
    // Strength of the action
    float _strength_x, _strength_y;
    
    Node* m_pTarget;
};

#endif //__SHAKE_H__
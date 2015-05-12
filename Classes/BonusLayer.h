//
//  BonusLayer.h
//  Tiên Cá
//
//  Created by Ho Duy Nhat Linh on 1/30/15.
//
//

#ifndef __TIEN_CA___BonusLayer__
#define __TIEN_CA___BonusLayer__

#include <stdio.h>
#include "cocos2d.h"

using namespace cocos2d;

class BonusLayer:public LayerColor{
public:
    BonusLayer();
    virtual ~BonusLayer();
    bool initWithType(std::string,int);
    static BonusLayer* createWithType(std::string,int);
    CREATE_FUNC(BonusLayer);
    
private:
    Sprite * background;
    Label * title;
    Label * bonusText;
    
    void onClose();
};

#endif /* defined(__Tie_n_Ca___BonusLayer__) */

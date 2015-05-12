//
//  ScriptConfig.h
//  Tiên Cá
//
//  Created by Ho Duy Nhat Linh on 1/17/15.
//
//

#ifndef __FISH___ScriptConfig__
#define __FISH___ScriptConfig__

#include "AbstractConfig.h"

class ScriptConfig:public AbstractConfig{
public:
    void load();
    bool init(string fileName);
    static ScriptConfig* getInstance();
private:
    static ScriptConfig* instance;
};

#endif /* defined(__Tie_n_Ca___ScriptConfig__) */

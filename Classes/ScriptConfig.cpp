//
//  ScriptConfig.cpp
//  Tiên Cá
//
//  Created by Ho Duy Nhat Linh on 1/17/15.
//
//

#include "ScriptConfig.h"

ScriptConfig* ScriptConfig::instance = NULL;
ScriptConfig* ScriptConfig::getInstance()
{
    if (instance == NULL)
    {
        instance = new ScriptConfig();
        return instance;
    }
    return instance;
}

bool ScriptConfig::init(string fileName){
    if (!AbstractConfig::init(fileName))
        return false;
    
    this->load();
    
    return true;
}
void ScriptConfig::load()
{
    AbstractConfig::load();
}
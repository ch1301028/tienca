#ifndef __CONFIG_MANAGEMENT_H__
#define __CONFIG_MANAGEMENT_H__

#include "cocos2d.h"


using namespace std;

class ConfigManagement{
private:
	static ConfigManagement* instance;
public:	
	static ConfigManagement* getInstance();
	void load();
};
#endif
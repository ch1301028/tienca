#ifndef __GUN_CONFIG_H__
#define __GUN_CONFIG_H__
#include "AbstractConfig.h"

class GunConfig :public AbstractConfig{
public:
	void load();
	bool init(string fileName);	
	static GunConfig* getInstance();


private:
	static GunConfig* instance;
	
};
#endif
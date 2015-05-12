#include "ConfigManagement.h"
#include "FishConfig.h"
#include "GunConfig.h"
#include "FishAniData.h"
#include "ScriptConfig.h"

ConfigManagement* ConfigManagement::instance = NULL;

void ConfigManagement::load(){
	/*this->_gunConfig = GunConfig();
	this->_fishConfig = FishConfig();

	this->getFish().init("game.json");
	this->getGun().init("game.json");
	*/
	FishConfig::getInstance()->init("game.json");
	GunConfig::getInstance()->init("game.json");
    ScriptConfig::getInstance()->init("game.json");
    
    
	FishAniData::getInstance()->init("fpack01.ExportJson");
	FishAniData::getInstance()->init("fpack05.ExportJson");
	
}



ConfigManagement* ConfigManagement::getInstance()
{
	if (instance == NULL)
	{
		instance = new ConfigManagement();
		return instance;
	}
	return instance;
}

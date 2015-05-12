#include "AbstractConfig.h"

AbstractConfig* AbstractConfig::instance = NULL;


AbstractConfig::AbstractConfig()
{

}

bool AbstractConfig::init(string configFileName){
	std::string fullPath = configFileName;
	std::string x = cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath.c_str());	
	if (this->document.Parse<0>(x.c_str()).HasParseError() == false){		
		return true;
	}
	return false;
}
bool AbstractConfig::loadFromFile(string fileName){
	std::string fullPath = fileName;		
	std::string x = cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath.c_str());
	rapidjson::Document document;
	if (document.Parse<0>(x.c_str()).HasParseError() == false){
		load();
		return true;
	}
	return false;
}
void AbstractConfig::load()
{

}
	

AbstractConfig* AbstractConfig::getInstance()
{
	if (instance == NULL)
	{
		instance = new AbstractConfig();
		return instance;
	}
	return instance;
}

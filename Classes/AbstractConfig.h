#ifndef __ABSRTACT_CONFIG__
#define __ABSRTACT_CONFIG__
#include "cocos2d.h"
#include "external/json/document.h"
#include "external/json/rapidjson.h"

using namespace std;
using namespace cocos2d;

class AbstractConfig{
private:
	static AbstractConfig* instance;
public:

	AbstractConfig();
	static AbstractConfig* getInstance();

	virtual bool init(string configFileName);
	virtual void load();

	bool loadFromFile(string fileName);

	CC_SYNTHESIZE(ValueMap, data, Data);
	CC_SYNTHESIZE(string, configFileName, ConfigFileName);
    
    rapidjson::Document document;
protected:
	
	
private:	
};
#endif

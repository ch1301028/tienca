#include "GunConfig.h"
GunConfig* GunConfig::instance = NULL;

GunConfig* GunConfig::getInstance()
{
	if (instance == NULL)
	{
		instance = new GunConfig();
		return instance;
	}
	return instance;
}


bool GunConfig::init(string fileName){
	if (!AbstractConfig::init(fileName))
		return false;

	this->load();

	return true;
}
void GunConfig::load() 
{			
	AbstractConfig::load();
	const rapidjson::Value& jsonData = document["gun"];
	for (rapidjson::SizeType i = 0; i < jsonData.Size(); i++){
		const rapidjson::Value& c = jsonData[i];
		ValueMap value;
		int id;
		if (jsonData[i].HasMember("id")){
			value["id"] = c["id"].GetInt();
			id = value["id"].asInt();
		}

		if (jsonData[i].HasMember("cost")){
			value["cost"] = c["id"].GetInt();
		}

		if (jsonData[i].HasMember("power")){
			value["power"] = c["power"].GetInt();
		}		
		data[StringUtils::format("%d",id)] = value;				
	}
}
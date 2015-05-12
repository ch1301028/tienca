#include "FishConfig.h"
FishConfig* FishConfig::instance = NULL;
FishConfig* FishConfig::getInstance()
{
	if (instance == NULL)
	{
		instance = new FishConfig();
		return instance;
	}
	return instance;
}

bool FishConfig::init(string fileName){
	if (!AbstractConfig::init(fileName))
		return false;

	this->load();

	return true;
}
void FishConfig::load()
{	
	AbstractConfig::load();
	const rapidjson::Value& jsonData = document["fish"];
		for (rapidjson::SizeType i = 0; i < jsonData.Size(); i++){
			const rapidjson::Value& c = jsonData[i];
			int id;
			ValueMap value;
			if (jsonData[i].HasMember("id")){
				value["id"] = c["id"].GetInt();
				id = value["id"].asInt();
			}

			if (jsonData[i].HasMember("hp")){
				value["hp"] = c["hp"].GetInt();
			}

			if (jsonData[i].HasMember("min-coin")){
				value["min-coin"] = c["min-coin"].GetInt();
			}

			if (jsonData[i].HasMember("max-coin")){
				value["max-coin"] = c["max-coin"].GetInt();
			}

			if (jsonData[i].HasMember("d-rate")){
				value["d-rate"] = c["d-rate"].GetInt();
			}
			if (jsonData[i].HasMember("available-team")){
				const rapidjson::Value& jsonAvaiableTeam = c["available-team"];
				ValueMap team;
				for (rapidjson::SizeType j = 0; j < jsonAvaiableTeam.Size(); j++){					
					team[StringUtils::format("%d", j)] = jsonAvaiableTeam[j].GetInt();

				}
				value["available-team"] = team;
			}
			data[StringUtils::format("%d", id)] = value;
		}	
}
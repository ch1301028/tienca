#include "FishAniData.h"
FishAniData* FishAniData::instance = NULL;
FishAniData* FishAniData::getInstance()
{
	if (instance == NULL)
	{
		instance = new FishAniData();
		return instance;
	}
	return instance;
}

bool FishAniData::init(string fileName){
	if (!AbstractConfig::init(fileName))
		return false;

	this->load();

	return true;
}
void FishAniData::load()
{
	AbstractConfig::load();
	const rapidjson::Value& animation_data = document["animation_data"];
	for (rapidjson::SizeType i = 0; i < animation_data.Size(); i++){
		const rapidjson::Value& mov_data = animation_data[i]["mov_data"];
		for (rapidjson::SizeType i = 0; i < mov_data.Size(); i++){
			const rapidjson::Value& c = mov_data[i];
			ValueMap value;
			if (c.HasMember("name")){
				const rapidjson::Value& mov_bone_data = c["mov_bone_data"];
				for (rapidjson::SizeType j = 0; j < mov_bone_data.Size(); j++){
					const rapidjson::Value& frame_data = mov_bone_data[j]["frame_data"];
					int frameCount = frame_data.Size();
					data[c["name"].GetString()] = frameCount;
				}
			}
		}
	}
	
}
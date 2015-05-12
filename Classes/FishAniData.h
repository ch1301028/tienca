#include "AbstractConfig.h"

class FishAniData :public AbstractConfig{
public:
	void load();
	bool init(string fileName);
	static FishAniData* getInstance();
private:
	static FishAniData* instance;
};
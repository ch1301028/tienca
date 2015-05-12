#include "AbstractConfig.h"

class FishConfig:public AbstractConfig{
public:
	void load();
	bool init(string fileName);	
	static FishConfig* getInstance();
private:
	static FishConfig* instance;
};
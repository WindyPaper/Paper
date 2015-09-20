#ifndef _ENGINE_PROFILE_H_
#define _ENGINE_PROFILE_H_

#include "util/IProfile.h"
#include "util/Singleton.h"
#include "util/Timer.h"

class EngineProfile : public IProfile, public Singleton<EngineProfile>
{
public:
	static EngineProfile& getInstance(void);
	static EngineProfile* getInstancePtr(void);

	EngineProfile();
	virtual ~EngineProfile();

	virtual void startProfile();
	virtual void endProfile();

	virtual uint getVerticeNum() const;

	//virtual void setFPS(float frameTime);
	virtual uint getFPS() const;

	virtual uint getDrawCallNum() const;
	virtual void addVerticeNum(int verticeNum);
	virtual void increaseDrawCallNum();

private:
	int mFPS;
	int mVerticeNum;
	int mDrawCallNum;

	Timer mTimer;
};

#endif
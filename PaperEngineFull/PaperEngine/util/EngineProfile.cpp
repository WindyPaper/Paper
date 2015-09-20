#include "util/PrecompileHead.h"
#include "util/EngineProfile.h"

template <> EngineProfile *Singleton<EngineProfile>::msInstance = 0;

EngineProfile& EngineProfile::getInstance(void)
{
	assert(msInstance);
	return *msInstance;
}

EngineProfile* EngineProfile::getInstancePtr(void)
{
	return msInstance;
}

EngineProfile::EngineProfile() :
mFPS(0),
mVerticeNum(0),
mDrawCallNum(0)
{

}

EngineProfile::~EngineProfile()
{

}

void EngineProfile::startProfile()
{
	mFPS = 0;
	mVerticeNum = 0;
	mDrawCallNum = 0;

	mTimer.start();
}

void EngineProfile::endProfile()
{
	mTimer.end();

	double offsetSecond = mTimer.getOffsetTime() / 1000;
	mFPS = static_cast<int>(1.0 / offsetSecond);

}

uint EngineProfile::getVerticeNum() const
{
	return mVerticeNum;
}

uint EngineProfile::getFPS() const
{
	return mFPS;
}

uint EngineProfile::getDrawCallNum() const
{
	return mDrawCallNum;
}

void EngineProfile::addVerticeNum(int verticeNum)
{
	mVerticeNum += verticeNum;
}

void EngineProfile::increaseDrawCallNum()
{
	++mDrawCallNum;
}

#ifndef _I_PROFILE_H_
#define _I_PROFILE_H_

#include "util/Engine_Define.h"

class IProfile
{
public:
	virtual ~IProfile() {}

	virtual void startProfile() = 0;
	virtual void endProfile() = 0;

	virtual uint getVerticeNum() const = 0;
	//virtual void setFPS(float frameTime) = 0;
	virtual uint getFPS() const = 0;
	virtual uint getDrawCallNum() const = 0;

	virtual void addVerticeNum(int verticeNum) = 0;
	virtual void increaseDrawCallNum() = 0;
};

#endif
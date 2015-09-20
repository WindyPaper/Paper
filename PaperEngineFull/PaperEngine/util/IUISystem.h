#ifndef _I_UI_SYSTEM_H_
#define _I_UI_SYSTEM_H_

#include "util/IProfile.h"

class IUISystem
{
public:
	virtual ~IUISystem() {}

	virtual bool init(size_t winHandle) = 0;
	virtual void destroy() = 0;
	virtual void draw() = 0;

	virtual void setProfileMsg(const IProfile *pProfile) = 0;
};

#endif
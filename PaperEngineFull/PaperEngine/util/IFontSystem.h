#ifndef _I_FONT_SYSTEM_H_
#define _I_FONT_SYSTEM_H_

#include "util/Engine_Define.h"
#include "util/Paper_Math.h"

enum FontAlignment
{
	FA_Left = 1 << 0,
	FA_Right = 1 << 1,
	FA_HCenter = 1 << 2,

	FA_Top = 1 << 3,
	FA_Bottom = 1 << 4,
	FA_VCenter = 1 << 5,
};

class IFontSystem
{
public:
	virtual ~IFontSystem() {}

	virtual void init() = 0;
	virtual void destory() = 0;

	virtual void drawString(const char* str, const math::Vector2f& screenpos, uint alignment = (FA_Left | FA_Top)) = 0;
};

#endif
#ifndef _I_CONTROL_UI_H_
#define _I_CONTROL_UI_H_

#include "XNAMath/xnamath.h"

namespace math
{
	class Vector3f;
}

class IControlUI
{
public:
	virtual ~IControlUI() {}

	virtual void addVector3(const char *name, math::Vector3f *pVec3) = 0;
	virtual void addVector4(const char *name, XMVECTOR *pVec4) = 0;
};

#endif
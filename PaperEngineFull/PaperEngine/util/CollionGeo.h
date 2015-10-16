#ifndef _COLLION_GEO_H_
#define _COLLION_GEO_H_

#include "util/Paper_Math.h"

struct AABB
{
	AABB();
	AABB(const math::Vector3f &min, const math::Vector3f &max);

	void reset();
	void add(const math::Vector3f &vec3);

	math::Vector3f min;
	math::Vector3f max;
};

#endif
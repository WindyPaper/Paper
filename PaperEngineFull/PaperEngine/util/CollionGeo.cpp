#include "util/PrecompileHead.h"
#include "util/CollionGeo.h"

AABB::AABB()
{

}

AABB::AABB(const math::Vector3f &min, const math::Vector3f &max)
{
	this->min = min;
	this->max = max;
}

void AABB::reset()
{
	min = math::VEC3F_ZERO;
	max = math::VEC3F_ZERO;
}

void AABB::add(const math::Vector3f &vec3)
{
	min.x = std::fminf(min.x, vec3.x);
	min.y = std::fminf(min.y, vec3.y);
	min.z = std::fminf(min.z, vec3.z);

	max.x = std::fmaxf(max.x, vec3.x);
	max.y = std::fmaxf(max.y, vec3.y);
	max.z = std::fmaxf(max.z, vec3.z);
}

#include "util/PrecompileHead.h"
#include "util/CollionGeo.h"

#include <limits>

AABB::AABB()
{
	float maxFloat = (std::numeric_limits<float>::max)();
	min = math::Vector3f(maxFloat, maxFloat, maxFloat);

	float minFloat = (std::numeric_limits<float>::min)();
	max = math::Vector3f(minFloat, minFloat, minFloat);
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


void AABB::merge(const AABB &aabb)
{
	min.x = std::fminf(min.x, aabb.min.x);
	min.y = std::fminf(min.y, aabb.min.y);
	min.z = std::fminf(min.z, aabb.min.z);

	max.x = std::fmaxf(max.x, aabb.max.x);
	max.y = std::fmaxf(max.y, aabb.max.y);
	max.z = std::fmaxf(max.z, aabb.max.z);
}

//Plane --------------------------------------------------
//Ë³Ê±ÕëµÄ
Plane::Plane(math::Vector3f &v1, math::Vector3f &v2, math::Vector3f &v3)
{
	set3Points(v1, v2, v3);
}

Plane::Plane(void)
{

}

Plane::~Plane()
{

}

void Plane::set3Points(math::Vector3f &v1, math::Vector3f &v2, math::Vector3f &v3)
{
	math::Vector3f planeVec1 = v1 - v2;
	math::Vector3f planeVec2 = v3 - v2;

	mNormal = planeVec2.cross(planeVec1).normalize();
	mPoint = v2;
	mD = mNormal * mPoint;
}

void Plane::setNormalAndPoint(math::Vector3f &normal, math::Vector3f &point)
{
	mNormal = normal.normalize();
	mPoint = point;
	mD = mNormal * mPoint;
}

float Plane::getDistance(math::Vector3f &p)
{
	return mNormal * p - mD;
}

//------------------------------------------------------------------------------------
void convertAABBToWorld(AABB &aabb, const math::Matrix44& worldMatrix)
{
	float width = aabb.max.x - aabb.min.x;
	float height = aabb.max.z - aabb.min.z;
	float depth = aabb.max.y - aabb.min.y;

	const int cubeVertexNum = 8;

	math::Vector3f vertex[cubeVertexNum];
	vertex[0] = aabb.max;
	vertex[1] = math::Vector3f(aabb.max.x - width, aabb.max.y, aabb.max.z);
	vertex[2] = math::Vector3f(aabb.max.x - width, aabb.max.y, aabb.max.z - height);
	vertex[3] = math::Vector3f(aabb.max.x, aabb.max.y, aabb.max.z - height);

	vertex[4] = math::Vector3f(aabb.min.x + width, aabb.min.y, aabb.min.z);
	vertex[5] = math::Vector3f(aabb.min.x + width, aabb.min.y, aabb.min.z + height);
	vertex[6] = math::Vector3f(aabb.min.x, aabb.min.y, aabb.min.z + height);
	vertex[7] = aabb.min;

	AABB newAABB;
	for (int i = 0; i < cubeVertexNum; ++i)
	{
		XMVECTOR vec = vertex[i];
		vec = XMVector3Transform(vec, worldMatrix.m);
		newAABB.add(math::Vector3f(vec.x, vec.y, vec.z));
	}

	aabb = newAABB;
}
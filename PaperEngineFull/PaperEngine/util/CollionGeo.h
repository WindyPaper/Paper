#ifndef _COLLION_GEO_H_
#define _COLLION_GEO_H_

#include "util/Paper_Math.h"

struct AABB
{
	AABB();
	AABB(const math::Vector3f &min, const math::Vector3f &max);

	void reset();
	void add(const math::Vector3f &vec3);

	void merge(const AABB &aabb);
	math::Vector3f getVertex(int index);

	math::Vector3f getPos() const;

	math::Vector3f min;
	math::Vector3f max;

};

class Plane
{
public:
	Plane::Plane(math::Vector3f &v1, math::Vector3f &v2, math::Vector3f &v3);
	Plane::Plane(void);
	Plane::~Plane();

	void set3Points(XMVECTOR &v1, XMVECTOR &v2, XMVECTOR &v3);
	void setNormalAndPoint(math::Vector3f &normal, math::Vector3f &point);
	//void setCoefficients(float a, float b, float c, float d);
	float getDistance(math::Vector3f &p);

	const math::Vector3f &getNormal() const { return mNormal; }
	int getD() const { return mD; }

private:
	math::Vector3f mNormal, mPoint;
	float mD;
};

void convertAABBToWorld(AABB &aabb, const math::Matrix44& worldMatrix);

#endif
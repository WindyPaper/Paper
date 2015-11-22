#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include "util/Paper_Math.h"
#include "util/CollionGeo.h"

class Frustum
{
public:
	enum PlaneType
	{
		P_NEAR = 0,
		P_FAR,
		P_TOP,
		P_BOTTOM,
		P_LEFT,
		P_RIGHT,		
		P_PLANE_TYPE_NUM,
	};

	Frustum();
	Frustum(const float fov, const float width, const float height, const float near, const float far);

	void setPerspective(const float fov, const float width, const float height, const float near, const float far);
	const math::Matrix44 &getProjMatrix() const;

	bool interectAABB(AABB &aabb);

protected:
	void initCollionPlane();
	void updateProjMatrix();
	bool frustumAABBIntersect(Plane *planes, const AABB &aabb);

protected:
	float mFov;
	float mWidth;
	float mHeight;
	float mNear;
	float mFar;

	math::Matrix44 mProjMatrix;

	XMVECTOR mPos;

	Plane mPlanes[P_PLANE_TYPE_NUM];
};

#endif
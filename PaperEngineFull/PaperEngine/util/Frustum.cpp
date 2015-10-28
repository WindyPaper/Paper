#include "util/PrecompileHead.h"
#include "util/Frustum.h"

Frustum::Frustum() :
	mFov(60.0f),
	mWidth(4),
	mHeight(3),
	mNear(1.0),
	mFar(10000.0)
{
	//fix me. should not write here
	updateProjMatrix();
}

Frustum::Frustum(const float fov, const float width, const float height, const float fNear, const float fFar) :
	mFov(0.0f),
	mWidth(0),
	mHeight(0),
	mNear(1.0)
{
	setPerspective(fov, width, height, fNear, fFar);
}

void Frustum::setPerspective(const float fov, const float width, const float height, const float fNear, const float fFar)
{
	assert(fFar > fNear);

	mFov = fov;
	mWidth = width;
	mHeight = height;
	mNear = fNear;
	mFar = fFar;

	updateProjMatrix();
}

const math::Matrix44 & Frustum::getProjMatrix() const
{
	return mProjMatrix;
}

bool Frustum::interectAABB(AABB &aabb)
{
	for (int i = P_NEAR; i < P_PLANE_TYPE_NUM; ++i)
	{
		if (mPlanes[i].getDistance(aabb.min) < 0 && mPlanes[i].getDistance(aabb.max) < 0)
		{
			return false;
		}
	}
	return true;
}

void Frustum::updateProjMatrix()
{
	math::PerspectiveProjParam param;
	param.width = mWidth;
	param.height = mHeight;
	param.zNear = mNear;
	param.zFar = mFar;
	param.fov = mFov;
	mProjMatrix.initPerspectiveProj(param);
}

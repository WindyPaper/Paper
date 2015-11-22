#include "util/PrecompileHead.h"
#include "util/Frustum.h"

Frustum::Frustum() :
	mFov(45),
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
	/*for (int i = P_NEAR; i < P_PLANE_TYPE_NUM; ++i)
	{
		if (mPlanes[i].getDistance(aabb.min) < 0 && mPlanes[i].getDistance(aabb.max) < 0)
		{
			return false;
		}
	}
	return true;*/
	//return frustumAABBIntersect(&mPlanes[0], aabb);
	//bool isOK = true;
	for (int i = P_NEAR; i < P_PLANE_TYPE_NUM; ++i)
	{
		//cal 8 vertex
		math::Vector3f &cornor = aabb.getPos();
		float offsetX = (aabb.max.x - aabb.min.x)/2;
		float offsetY = (aabb.max.y - aabb.min.y)/2;
		float offsetZ = (aabb.max.z - aabb.min.z)/2;

		math::Vector3f allVertex[8];
		allVertex[0] = cornor + math::Vector3f(offsetX, offsetY, -offsetZ);
		allVertex[1] = cornor + math::Vector3f(offsetX, offsetY, offsetZ);
		allVertex[2] = cornor + math::Vector3f(-offsetX, offsetY, -offsetZ);
		allVertex[3] = cornor + math::Vector3f(-offsetX, offsetY, offsetZ);
		allVertex[4] = cornor + math::Vector3f(offsetX, -offsetY, -offsetZ);
		allVertex[5] = cornor + math::Vector3f(offsetX, -offsetY, offsetZ);
		allVertex[6] = cornor + math::Vector3f(-offsetX, -offsetY, -offsetZ);
		allVertex[7] = cornor + math::Vector3f(-offsetX, -offsetY, offsetZ);

		/*if (mPlanes[i].getDistance(aabb.min) < 0 && mPlanes[i].getDistance(aabb.max) < 0)
		{
			return false;
		}*/
		bool isOK = false;
		for (int j = 0; j < 8; ++j)
		{
			if (mPlanes[i].getDistance(allVertex[j]) > 0)
			{
				isOK = true;
			}
		}

		if (isOK == false)
		{
			return false;
		}
	}
	return true;
}

bool Frustum::frustumAABBIntersect(Plane *planes, const AABB &aabb)
{
	bool ret = true;
	math::Vector3f vmin, vmax;

	for (int i = 0; i < 6; ++i) {
		// X axis 
		if (planes[i].getNormal().x > 0) {
			vmin.x = aabb.min.x;
			vmax.x = aabb.max.x;
		}
		else {
			vmin.x = aabb.max.x;
			vmax.x = aabb.min.x;
		}
		// Y axis 
		if (planes[i].getNormal().y > 0) {
			vmin.y = aabb.min.y;
			vmax.y = aabb.max.y;
		}
		else {
			vmin.y = aabb.max.y;
			vmax.y = aabb.min.y;
		}
		// Z axis 
		if (planes[i].getNormal().z > 0) {
			vmin.z = aabb.min.z;
			vmax.z = aabb.max.z;
		}
		else {
			vmin.z = aabb.max.z;
			vmax.z = aabb.min.z;
		}
		if (planes[i].getDistance(vmin) > 0)
			return false;
		if (planes[i].getDistance(vmax) > 0)
			ret = true;
	}
	return ret;
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

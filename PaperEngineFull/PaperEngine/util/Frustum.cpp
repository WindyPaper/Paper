#include "util/PrecompileHead.h"
#include "util/Frustum.h"

Frustum::Frustum() :
	mFov(60.0f),
	mWidth(2),
	mHeight(1),
	mNear(1.0),
	mFar(10000.0)
{
	//fix me. should not write this
	updateProjMatrix();
}

Frustum::Frustum(const float fov, const int width, const int height, const float fNear, const float fFar) :
	mFov(0.0f),
	mWidth(0),
	mHeight(0),
	mNear(1.0),
	mFar(10000.0)
{
	setPerspective(fov, width, height, fNear, fFar);
}

void Frustum::setPerspective(const float fov, const int width, const int height, const float fNear, const float fFar)
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

void Frustum::updateProjMatrix()
{
	/*const float ar = mWidth / mHeight;
	const float zRange = mNear - mFar;
	const float tanHalfFov = tanf(ToRadian(mFov/2.0));*/

/*
	mProjMatrix.m[0][0] = 1.0f / (tanHalfFov * ar); mProjMatrix.m[0][1] = 0.0f; mProjMatrix.m[0][2] = 0.0f; mProjMatrix.m[0][3] = 0.0f;
	mProjMatrix.m[1][0] = 0.0f; mProjMatrix.m[1][1] = 1.0f / tanHalfFov; mProjMatrix.m[1][2] = 0.0f; mProjMatrix.m[1][3] = 0.0f;
	mProjMatrix.m[2][0] = 0.0f; mProjMatrix.m[2][1] = 0.0f; mProjMatrix.m[2][2] = (-mNear - mFar) / zRange; mProjMatrix.m[2][3] = 2.0f * mNear * mFar / zRange;
	mProjMatrix.m[3][0] = 0.0f; mProjMatrix.m[3][1] = 0.0f; mProjMatrix.m[3][2] = 1.0f; mProjMatrix.m[3][3] = 0.0f;*/

	math::PerspectiveProjParam param;
	param.width = mWidth;
	param.height = mHeight;
	param.zNear = mNear;
	param.zFar = mFar;
	param.fov = mFov;
	mProjMatrix.initPerspectiveProj(param);
}

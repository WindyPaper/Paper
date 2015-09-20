#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include "util/Paper_Math.h"

class Frustum
{
public:
	Frustum();
	Frustum(const float fov, const int width, const int height, const float near, const float far);

	void setPerspective(const float fov, const int width, const int height, const float near, const float far);
	const math::Matrix44 &getProjMatrix() const;

protected:
	void updateProjMatrix();

private:
	float mFov;
	int mWidth;
	int mHeight;
	float mNear;
	float mFar;

	math::Matrix44 mProjMatrix;
};

#endif
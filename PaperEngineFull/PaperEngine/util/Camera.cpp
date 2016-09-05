#include "util/PrecompileHead.h"

#include "util/Camera.h"
#include "util/IControlUI.h"
#include <cmath>

Camera::Camera()
{
	//默认的应该可以自动生成名字，后面加入
	//we should can generate name of object by default
	init();
}

Camera::Camera(const math::Vector3f &pos, const math::Vector3f &lookAt, const math::Vector3f &up)
{
	mPos = pos;
	mLookAt = lookAt;
	mUp = up;

	init();
}

Camera::Camera(const std::string &name)
{
	init();
}

Camera::~Camera()
{

}

void Camera::init()
{
	//math::Vector3f hLookAt(mLookAt.x, 0.0f, mLookAt.z);
	//hLookAt.normalize();
	mPitch = 0;
	mYaw = 0;
	mRoll = 0;

	memset(&mPos, 0, sizeof(XMVECTOR));
	memset(&mLookAt, 0, sizeof(XMVECTOR));
	memset(&mUp, 0, sizeof(XMVECTOR));

	update();
}

const math::Vector3f Camera::getPos() const
{
	math::Vector3f pos;
	pos.x = mPos.x;
	pos.y = mPos.y;
	pos.z = mPos.z;
	return pos;
}

const math::Vector3f Camera::getLookAt() 
{
	//return mLookAt;
	math::Vector3f lookat;
	lookat.x = mLookAt.x;
	lookat.y = mLookAt.y;
	lookat.z = mLookAt.z;
	return lookat;
}

const math::Vector3f Camera::getUp() 
{
	math::Vector3f up;
	up.x = mUp.x;
	up.y = mUp.y;
	up.z = mUp.z;
	return up;
}

void Camera::setPos(const math::Vector3f &pos)
{
	XMVECTOR cpos = pos;
	if (!XMVector3Equal(mPos, cpos))
	{
		//std::cout << "x = " << pos.x << " z = " << pos.z << std::endl;
		mPos = cpos;
		update();
	}

}

void Camera::setLookAt(const math::Vector3f &lookAt)
{
	XMVECTOR clookat = lookAt;
	if (!XMVector3Equal(mLookAt, clookat))
	{
		mLookAt = lookAt;
		update();
	}
}

void Camera::setUp(const math::Vector3f &up)
{
	XMVECTOR cup = up;
	if (!XMVector3Equal(mUp, cup))
	{
		mUp = cup;
		update();
	}
}


void Camera::pitch(const float angle)
{
	//math::Vector3f xAxis = mOrientation * math::VEC3F_UNIT_X;
	//math::Vector3f lookup = getLookAt();
	//math::Vector3f xAxis = (lookup - mPos).cross(mUp);
	//cameraRotate(xAxis, angle);
	mPitch += angle;
	update();
}

void Camera::setPitch(const float angle)
{
	mPitch = angle;
	update();
}

void Camera::roll(const float angle)
{
	//math::Vector3f zAxis = mOrientation * math::VEC3F_UNIT_Z;
	//cameraRotate(zAxis, angle);
	mRoll += angle;
	update();
}

void Camera::setRoll(const float angle)
{
	mRoll = angle;
	update();
}

void Camera::yaw(const float angle)
{
	//mOrientation.Normalize();
	//math::Vector3f yAxis = mOrientation * math::VEC3F_UNIT_Y;
	//cameraRotate(yAxis, angle);
	mYaw += angle;
	update();
}

void Camera::setYaw(const float angle)
{
	mYaw = angle;
	update();
}

void Camera::update()
{
	updateViewMatrix();
}

void Camera::cameraRotate(const math::Vector3f &axis, const float angle)
{
	math::Quaternion quat;
	quat.fromAngleAxis(axis, angle);
	quat.Normalize();
	cameraRotate(quat);
}

void Camera::cameraRotate(const math::Quaternion &quat)
{
	assert(false);
	//mOrientation = quat * mOrientation;
	/*mOrientation.vec4.w = mOrientation.vec4.w * quat.vec4.w - mOrientation.vec4.x * quat.vec4.x - mOrientation.vec4.y * quat.vec4.y - mOrientation.vec4.z * quat.vec4.z;  // 1
	mOrientation.vec4.x = mOrientation.vec4.w * quat.vec4.x + mOrientation.vec4.x * quat.vec4.w + mOrientation.vec4.y * quat.vec4.z - mOrientation.vec4.z * quat.vec4.y;  // i
	mOrientation.vec4.y = mOrientation.vec4.w * quat.vec4.y - mOrientation.vec4.x * quat.vec4.z + mOrientation.vec4.y * quat.vec4.w + mOrientation.vec4.z * quat.vec4.x;  // j
	mOrientation.vec4.z = mOrientation.vec4.w * quat.vec4.z + mOrientation.vec4.x * quat.vec4.y - mOrientation.vec4.y * quat.vec4.x + mOrientation.vec4.z * quat.vec4.w;*/
	update();
}

const math::Matrix44 & Camera::getViewMatrix() const
{
	return mViewMatrix;
}

void Camera::addCameraEdit(IControlUI *pUIControl)
{
	pUIControl->addVector4("cam position", &mPos);
	pUIControl->addVector4("dir", &mLookAt);
}

void Camera::updateViewMatrix()
{
	if (ToRadian(mYaw) > 2 * XM_PI)
		mYaw = ToDegree(0);

	// Cap the yaw between 0 and 360
	if (ToRadian(mYaw) < 0)
		mYaw = ToDegree(2 * XM_PI);

	// Let's cap the pitch to 90 degrees up and -90 degrees down
	if (ToRadian(mPitch) > ToRadian(90.0f))
		mPitch = 90.0f;
	if (ToRadian(mPitch) < ToRadian(-90.0f))
		mPitch = -90.0f;

	XMMATRIX rotateMat = XMMatrixRotationRollPitchYaw(ToRadian(mPitch), ToRadian(mYaw), 0);

	XMVECTOR initTarget;
	initTarget.x = 0;
	initTarget.y = 0;
	initTarget.z = -1;

	XMVECTOR initUp;
	initUp.x = 0;
	initUp.y = 1;
	initUp.z = 0;

	initTarget = XMVector3Transform(initTarget, rotateMat);
	initUp = XMVector3Transform(initUp, rotateMat);

	XMVECTOR target = (initTarget + mPos);
	mLookAt = initTarget;
	XMVECTOR up = initUp;
	mUp = initUp;

	XMVECTOR eyeVec = mPos;

	mViewMatrix.m = XMMatrixLookAtRH(mPos, target, up);
	

	updateCollionPlane();
}


void Camera::updateCollionPlane()
{
	XMVECTOR nor_up = XMVector3Normalize(mUp);
	XMVECTOR nor_lookat = XMVector3Normalize(mLookAt);

	XMVECTOR nc = nor_lookat * mNear + mPos;
	XMVECTOR fc = nor_lookat * mFar + mPos;

	XMVECTOR left = XMVector3Cross(mUp, mLookAt);
	left = XMVector3Normalize(left);

	float nHeight = std::tan(ToRadian(mFov / 2)) * mNear;
	float nWidth = mWidth / mHeight * nHeight;

	float fHeight = std::tan(ToRadian(mFov / 2)) * mFar;
	float fWidth = mWidth / mHeight * fHeight;

	
	XMMATRIX transformMatrix = mViewMatrix.m * mProjMatrix.m;
	XMVECTOR t;
	transformMatrix = XMMatrixInverse(&t, transformMatrix);

	XMVECTOR nlt;
	nlt.x = -1.0; nlt.y = 1.0; nlt.z = 0.0; nlt.w = 1.0;
	XMVECTOR nlb;
	nlb.x = -1.0; nlb.y = -1.0; nlb.z = 0.0; nlb.w = 1.0;
	XMVECTOR nrt;
	nrt.x = 1.0; nrt.y = 1.0; nrt.z = 0.0; nrt.w = 1.0;
	XMVECTOR nrb;
	nrb.x = 1.0; nrb.y = -1.0; nrb.z = 0.0; nrb.w = 1.0;

	XMVECTOR flt;
	flt.x = -1.0; flt.y = 1.0; flt.z = 1.0; flt.w = 1.0;
	XMVECTOR flb;
	flb.x = -1.0; flb.y = -1.0; flb.z = 1.0; flb.w = 1.0;
	XMVECTOR frt;
	frt.x = 1.0; frt.y = 1.0; frt.z = 1.0; frt.w = 1.0;
	XMVECTOR frb;
	frb.x = 1.0; frb.y = -1.0; frb.z = 1.0; frb.w = 1.0;

	nlt = XMVector3TransformCoord(nlt, transformMatrix);
	nlb = XMVector3TransformCoord(nlb, transformMatrix);
	nrt = XMVector3TransformCoord(nrt, transformMatrix);
	nrb = XMVector3TransformCoord(nrb, transformMatrix);

	flt = XMVector3TransformCoord(flt, transformMatrix);
	flb = XMVector3TransformCoord(flb, transformMatrix);
	frt = XMVector3TransformCoord(frt, transformMatrix);
	frb = XMVector3TransformCoord(frb, transformMatrix);

	mPlanes[P_TOP].set3Points(nrt, nlt, flt);
	mPlanes[P_BOTTOM].set3Points(nlb, nrb, frb);
	mPlanes[P_LEFT].set3Points(nlt, nlb, flb);
	mPlanes[P_RIGHT].set3Points(nrb, nrt, frb);
	mPlanes[P_NEAR].set3Points(nlt, nrt, nrb);
	mPlanes[P_FAR].set3Points(frt, flt, flb);

	XMVECTOR newTestVec = XMVector3Transform(XMVector3Transform(flt, mViewMatrix.m), mProjMatrix.m);
}

/*
void Camera::updateCollionPlane()
{
	math::Vector3f dir, nc, fc, X, Y, Z;

	Z = mPos - mLookAt;
	Z.normalize();

	X = XMVector3Cross(mUp, Z);
	X.normalize();

	Y = XMVector3Cross(Z, X);

	nc = mPos - Z * mNear;
	fc = mPos - Z * mFar;

	float nHeight = std::tan(ToRadian(mFov / 2)) * mNear;
	float nWidth = mWidth / mHeight * nHeight;

	float fHeight = std::tan(ToRadian(mFov / 2)) * mFar;
	float fWidth = mWidth / mHeight * fHeight;

	XMVECTOR ntl = nc + Y * nHeight - X * nWidth;
	XMVECTOR ntr = nc + Y * nHeight + X * nWidth;
	XMVECTOR nbl = nc - Y * nHeight - X * nWidth;
	XMVECTOR nbr = nc - Y * nHeight + X * nWidth;

	XMVECTOR ftl = fc + Y * fHeight - X * fWidth;
	XMVECTOR ftr = fc + Y * fHeight + X * fWidth;
	XMVECTOR fbl = fc - Y * fHeight - X * fWidth;
	XMVECTOR fbr = fc - Y * fHeight + X * fWidth;

	mPlanes[P_TOP].set3Points(ntr, ntl, ftl);
	mPlanes[P_BOTTOM].set3Points(nbl, nbr, fbr);
	mPlanes[P_LEFT].set3Points(ntl, nbl, fbl);
	mPlanes[P_RIGHT].set3Points(nbr, ntr, fbr);
	mPlanes[P_NEAR].set3Points(ntl, ntr, nbr);
	mPlanes[P_FAR].set3Points(ftr, ftl, fbl);
}*/

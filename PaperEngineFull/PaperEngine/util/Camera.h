#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <string>
#include "util/Platform.h"
#include "util/Paper_Math.h"
#include "util/Frustum.h"

class ENGINE_DLL Camera : public Frustum
{
public:
	enum ViewType
	{
		Ortho,
		Perspect,
	};

	Camera();
	Camera(const math::Vector3f &pos, const math::Vector3f &lookAt, const math::Vector3f &up);
	Camera(const std::string &name);
	~Camera();

	void init();

	void update();

	const math::Vector3f getPos() const;
	const math::Vector3f getLookAt();
	const math::Vector3f getUp();

	void setPos(const math::Vector3f &pos);
	void setLookAt(const math::Vector3f &lookAt);
	void setUp(const math::Vector3f &up);

	void pitch(const float angle);
	void setPitch(const float angle);
	void roll(const float angle);
	void setRoll(const float angle);
	void yaw(const float angle);
	void setYaw(const float angle);
	void cameraRotate(const math::Vector3f &axis, const float angle);
	void cameraRotate(const math::Quaternion &quat);

	//void setPerspective(const float fov, const int width, const int height, const float near, const float far);

	const math::Matrix44 &getViewMatrix() const;

	//fix me. is only for test, need delete
	//const math::Matrix44 getModelMatrix() const;

protected:
	void updateViewMatrix();
	void updateCollionPlane();

private: 
	math::Matrix44 mViewMatrix;

	XMVECTOR mLookAt;
	XMVECTOR mUp;

	float mPitch;
	float mYaw;
	float mRoll;

	std::string mName;
};

#endif
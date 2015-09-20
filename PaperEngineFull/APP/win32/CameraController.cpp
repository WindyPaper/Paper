#include <assert.h>
#include <iostream>
#include "win32/CameraController.h"


CameraController::CameraController() :
	mpCamera(0),
	mStep(10),
	mCameraAngleH(0),
	mCameraAngleV(0)
{
	mMouseLastPos.x = -1;
	mMouseLastPos.y = -1;
}

CameraController::CameraController(Camera *pCamera) :
	mpCamera(pCamera),
	mStep(10),
	mCameraAngleH(0),
	mCameraAngleV(0)
{
	mMouseLastPos.x = -1;
	mMouseLastPos.y = -1;

	_initCameraParam();
}

CameraController::~CameraController()
{

} 

void CameraController::detectMouse(const OIS::MouseButtonID id, int x, int y)
{
	if (mMouseLastPos.x < 0)
	{
		mMouseLastPos.x = x;
	}
	if (mMouseLastPos.y < 0)
	{
		mMouseLastPos.y = y;
	}

	if (id == OIS::MB_Right)
	{
		int offsetX = x - mMouseLastPos.x;
		int offsetY = y - mMouseLastPos.y;

		const float speed = 0.15f;

		mpCamera->yaw((float)offsetX * speed);
		mpCamera->pitch((float)offsetY * speed);
	}

	mMouseLastPos.x = x;
	mMouseLastPos.y = y;
}

void CameraController::_initCameraParam()
{
	assert(mpCamera);

	math::Vector3f lookAt = mpCamera->getLookAt();

	math::Vector3f hLookAt(lookAt.x, 0.0f, lookAt.z);
	hLookAt.normalize();
	if (hLookAt.z >= 0)
	{
		if (hLookAt.x >= 0)
		{
			mCameraAngleH = 360.0f - ToDegree(asin(hLookAt.z));
		}
		else
		{
			mCameraAngleH = 180.0f + ToDegree(asin(hLookAt.z));
		}
	}
	else
	{
		if (hLookAt.x >= 0)
		{
			mCameraAngleH = ToDegree(asin(-hLookAt.z));
		}
		else
		{
			mCameraAngleH = 90.0f + ToDegree(asin(-hLookAt.z));
		}
	}

	lookAt.normalize();
	mCameraAngleV = ToDegree(asin(lookAt.y));

}

void CameraController::setCamera(Camera *pCamera)
{
	mpCamera = pCamera;
	_initCameraParam();
}

bool CameraController::keyPressed(const OIS::KeyEvent &arg)
{
	OIS::KeyCode key = arg.key;

	if (mpCamera == 0)
	{
		return true;
	}

	math::Vector3f pos = mpCamera->getPos();
	math::Vector3f lookAt = mpCamera->getLookAt();
	math::Vector3f up = mpCamera->getUp();

	if (key == OIS::KC_W)
	{		
		pos += lookAt * mStep;
	}
	else if (key == OIS::KC_S)
	{
		pos -= lookAt * mStep;
	}
	else if (key == OIS::KC_A)
	{
		math::Vector3f moveDirVec = up.cross(lookAt).normalize();
		pos += moveDirVec * mStep;
	}
	else if (key == OIS::KC_D)
	{
		math::Vector3f moveDirVec = -up.cross(lookAt).normalize();
		pos += moveDirVec * mStep;
	}
	else if (key == OIS::KC_E)
	{
		mpCamera->roll(mStep);
	}
	else if (key == OIS::KC_Q)
	{
		mpCamera->roll(-mStep);
	}

	/*std::cout << "pos x: " << pos.x;
	std::cout << "pos y: " << pos.y;
	std::cout << "pos z: " << pos.z;
	std::cout << std::endl;*/

	mpCamera->setPos(pos);

	return true;
}

bool CameraController::keyReleased(const OIS::KeyEvent &arg)
{
	return true;
}

bool CameraController::mouseMoved(const OIS::MouseEvent &arg)
{
	detectMouse(mPressBtnId, arg.state.X.abs, arg.state.Y.abs);
	return true;
}

bool CameraController::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	mPressBtnId = id;
	detectMouse(id, arg.state.X.abs, arg.state.Y.abs);
	return true;
}

bool CameraController::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Right)
	{
		mMouseLastPos.x = -1;
		mMouseLastPos.y = -1;

		if (mPressBtnId == id)
		{
			mPressBtnId = (OIS::MouseButtonID)-1;
		}
	}
	
	return true;
}

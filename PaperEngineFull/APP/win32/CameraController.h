#ifndef _CAMERA_CONTROLLER_H_
#define _CAMERA_CONTROLLER_H_

#include "OIS.h"
#include "util/Camera.h"

class CameraController : public OIS::KeyListener, public OIS::MouseListener
{
public:
	CameraController();
	CameraController(Camera *pCamera);
	~CameraController();

	void setCamera(Camera *pCamera);

	void detectMouse(const OIS::MouseButtonID id, int x, int y);

	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);

	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

protected:
	void _initCameraParam();

private:
	Camera *mpCamera;
	float mStep;

	OIS::MouseButtonID mPressBtnId;

	math::Vector2f mMouseLastPos;
	float mCameraAngleH;
	float mCameraAngleV;
};

#endif
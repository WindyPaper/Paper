#ifndef _CONTROL_UI_H_
#define _CONTROL_UI_H_

#include "util/IControlUI.h"
#include <AntTweakBar.h>
#include <OIS.h>

//It will combine all ui here
class ControlUI : public IControlUI
{
public:
	//static ControlUI &getInstance();
	//static ControlUI *getInstancePtr();

	ControlUI();
	~ControlUI();

	bool initAntTweakBar();
	void initTwType();

	virtual bool keyPressed(const OIS::KeyEvent &arg);

	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	TwBar *getMainBar();

	virtual void draw();

	virtual void addVector3(const char *name, math::Vector3f *pVec3);
	virtual void addVector4(const char *name, XMVECTOR *pVec4);

protected:
	int OISKeyToATBKey(OIS::KeyCode oisKey);

protected:
	//ATB mAtb; 
	TwBar *mpBar;
	
	TwType mVector3fType;
	TwType mVector4fType;
	bool mIsUseTw;
};

#endif
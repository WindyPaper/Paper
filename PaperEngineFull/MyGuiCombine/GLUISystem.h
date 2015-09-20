#ifndef _GL_UI_SYSTEM_H_
#define _GL_UI_SYSTEM_H_

#include "util/IUISystem.h"
#include "BaseManager.h"

class GLUISystem : public IUISystem,
	public base::BaseManager
{
public:
	~GLUISystem();

	virtual bool init(size_t winHandle);
	virtual void destroy();
	virtual void draw();

	virtual void setProfileMsg(const IProfile *pProfile);

protected:
	void setupResources();
	void createScene();

private:
	MyGUI::VectorWidgetPtr mRoot;
};

#endif
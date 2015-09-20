#ifndef _OPENGL_SUPPORT_H_
#define _OPENGL_SUPPORT_H_

#include <string>
#include <set>
#include "util/Platform.h"

class ENGINE_DLL OpenGLSupport
{
public:
	OpenGLSupport();
	virtual ~OpenGLSupport();

	virtual void createWindow(const char *windowtitle) = 0;
	void initGLExtension();
protected:
	//std::set<std::string> extensionList;
};

#endif
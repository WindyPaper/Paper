#include "OpenGLPlatform/RenderPreCompileHead.h"
#include "OpenGLPlatform/OpenGLSupport.h"
#include "util/Log.h"

OpenGLSupport::OpenGLSupport() 
{
	
}

OpenGLSupport::~OpenGLSupport()
{

}

void OpenGLSupport::initGLExtension()
{
	const GLubyte *glVer = glGetString(GL_VERSION);
	std::string tempStr = reinterpret_cast<const char*>(glVer);
	Log::getInstance().logMsgStr("OpenGL Version : " + tempStr + '\n');

	const GLubyte *glExt = glGetString(GL_EXTENSIONS);
	tempStr = reinterpret_cast<const char*>(glExt);
	Log::getInstance().logMsgStr("OpenGL Extension : " + tempStr + '\n');
}




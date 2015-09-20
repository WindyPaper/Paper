#ifndef _OPENGL_RENDER_TEXTURE_H_
#define _OPENGL_RENDER_TEXTURE_H_

#include "util/Platform.h"
#include "util/Engine_Define.h"
#include "util/IRenderTexture.h"
#include "GL/glew.h"

class ENGINE_DLL OpenGLRenderTexture : public IRenderTexture
{
public:
	OpenGLRenderTexture();
	~OpenGLRenderTexture();

	virtual bool init(const uint width, const uint height);

	virtual void bindForWriting();
	virtual void bindForReading(const GLenum textureUnit);

private:
	GLuint mFbo;
	GLuint mTexture;
};

#endif
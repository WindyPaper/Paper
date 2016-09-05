#ifndef _OPENGL_GBUFFER_H_
#define _OPENGL_GBUFFER_H_

#include "util/Platform.h"
#include "util/Engine_Define.h"
#include "GL/glew.h"

class ENGINE_DLL OpenGLGBuffer
{
public:
	enum {
		NORMAL_MAP = 0,
		POS_MAP,
		FLUX_COLOR_MAP,
	};

	OpenGLGBuffer();
	~OpenGLGBuffer();

	virtual bool init(const uint width, const uint height);

	virtual void bindForWriting();
	virtual void bindForReading();

private:
	GLuint mFbo;

	GLuint mDepthTex;
	GLuint mNormalTex;

	//rsm
	GLuint mWorldPosTex;
	GLuint mFluxTex;
};

#endif
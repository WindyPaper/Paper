#ifndef _OPENGL_IMPL_H_
#define _OPENGL_IMPL_H_

#include "util/Singleton.h"
#include <GL/glew.h>
#include "util/Engine_Define.h"

class ENGINE_DLL OpenGLImpl : public Singleton<OpenGLImpl>
{
public:
	static OpenGLImpl &getInstance();
	static OpenGLImpl *getInstancePtr();

	OpenGLImpl();

	void activeVertexBufObj(const GLuint bufId);
	void activeIndexBufObj(const GLuint bufId);

	void activeShaderProgram(const GLuint shaderId);

	void activeTexUnit(const GLenum unit);
	void activeTexObj(const GLenum target, const GLuint texId);
	//void setTexFilter(const Glenum target, )

	void activeAlphaBlend(const bool enable);
	void setBlendFunc(const GLenum srcBlend, const GLenum destBlend);
	void setBlendFuncSeparate(const GLenum srcBlend, const GLenum destBlend, const GLenum srcAlpha, const GLenum destAlpha);

	//depth test
	void activeDepthTest(const bool enable);

	void checkError();

private:
	GLuint mCurrVertexBufObj;
	GLuint mCurrIndexBufObj;

	GLuint mShaderProgram;

	GLenum mCurrTexUnit;
	GLuint mCurrTexObjs[MAX_TEX_UNIT];

	// alpha blend
	bool mAlphaBlendEnable;
	GLenum mSrcBlend, mDestBlend, mSrcAlphaBlend, mDestAlphaBlend;
	GLenum mBlendEquation, mBlendAlphaEquation;

	// cull mode
	GLuint mCullMode;
	bool mCullEnable;
	GLenum mCullFaceMode;

	// stencil test
	bool mStencilTestEnable;
	GLuint mStencilBackWriteMask, mStencilFrontWriteMask;

	//depth test
	bool mDepthTestEnable;
	bool mDepthWriteEnable;
	GLenum mDepthFunc;

	//scissor test
	bool mScissorTestEnable;

	// color mask
	unsigned short int mColorMask;

	//fbo
	GLuint mCurrFrameBuf;
};

#endif
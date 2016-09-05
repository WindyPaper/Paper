#include "OpenGLPlatform/RenderPreCompileHead.h"
#include "OpenGLPlatform/OpenGLImpl.h"
#include "util/Log.h"

template<> OpenGLImpl *Singleton<OpenGLImpl>::msInstance = 0;

OpenGLImpl *OpenGLImpl::getInstancePtr()
{
	return msInstance;
}

OpenGLImpl &OpenGLImpl::getInstance()
{
	assert(msInstance);
	return (*msInstance);
}

OpenGLImpl::OpenGLImpl()
{
	mCurrVertexBufObj = -1;
	mCurrIndexBufObj = -1;

	mShaderProgram = -1;

	mCurrTexUnit = -1;

	for (int i = 0; i < MAX_TEX_UNIT; ++i)
	{
		mCurrTexObjs[i];
	}

	// alpha blend
	mAlphaBlendEnable = false;
	mSrcBlend = -1;
	mDestBlend = -1;
	mSrcAlphaBlend = -1;
	mDestAlphaBlend = -1;

	mBlendEquation = -1;
	mBlendAlphaEquation = -1;

	// cull mode
	mCullMode = -1;
	mCullEnable = false;
	mCullFaceMode = -1;

	// stencil test
	mStencilTestEnable = false;
	mStencilBackWriteMask = -1;
	mStencilFrontWriteMask = -1;

	//depth test
	mDepthTestEnable = true;
	mDepthWriteEnable = true;
	mDepthFunc = -1;

	//scissor test
	mScissorTestEnable = false;

	// color mask
	mColorMask = 0;

	//fbo
	mCurrFrameBuf = -1;
}

void OpenGLImpl::checkError()
{
#if _DEBUG
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
	{
		std::string errStr;
		switch (error)
		{
		case GL_INVALID_ENUM:
			errStr = "GL_INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			errStr = "GL_INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			errStr = "GL_INVALID_OPERATION";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			errStr = "GL_INVALID_FRAMEBUFFER_OPERATION";
			break;
		case GL_OUT_OF_MEMORY:
			errStr = "GL_OUT_OF_MEMORY";
			break;
		case GL_STACK_UNDERFLOW:
			errStr = "GL_STACK_UNDERFLOW";
			break;
		case GL_STACK_OVERFLOW:
			errStr = "GL_STACK_OVERFLOW";
			break;
		default:
			break;
		}
		
		printf("GL error id : %s \n", errStr.c_str());
		assert(false);
	}
#else
	return;
#endif
}

void OpenGLImpl::activeVertexBufObj(const GLuint bufId)
{
	if(mCurrVertexBufObj != bufId)
	{
		glBindBuffer(GL_ARRAY_BUFFER, bufId);
		mCurrVertexBufObj = bufId;

		checkError();
	}
}

void OpenGLImpl::activeIndexBufObj(const GLuint bufId)
{
	if(mCurrIndexBufObj != bufId)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufId);
		mCurrIndexBufObj = bufId;

		checkError();
	}
}

void OpenGLImpl::activeTexObj(const GLenum target, const GLuint texId)
{
	//if(mCurrTexObjs[mCurrTexUnit] != texId)
	//{
		glBindTexture(target, texId);
		mCurrTexObjs[mCurrTexUnit] = texId;
		checkError();
	//}
}

void OpenGLImpl::activeTexUnit(const GLenum unit)
{
	//if(mCurrTexUnit != unit)
	//{
		assert(unit >= 0 && unit < MAX_TEX_UNIT);
		glActiveTexture(GL_TEXTURE0 + unit);
		mCurrTexUnit = unit;

		checkError();
	//}
}

void OpenGLImpl::activeShaderProgram(const GLuint shaderId)
{
	if(mShaderProgram != shaderId)
	{
		glUseProgram(shaderId);
		mShaderProgram = shaderId;

		checkError();
	}
}

void OpenGLImpl::activeAlphaBlend(const bool enable)
{
	if(mAlphaBlendEnable != enable)
	{
		if(enable)
		{
			glEnable(GL_BLEND);
		}
		else
		{
			glDisable(GL_BLEND);
		}

		mAlphaBlendEnable = enable;

		checkError();
	}
}

void OpenGLImpl::setBlendFunc(const GLenum srcBlend, const GLenum destBlend)
{
	if(srcBlend != mSrcBlend || destBlend != mDestBlend)
	{
		glBlendFunc(srcBlend, destBlend);
		mSrcBlend = srcBlend;
		mDestBlend = destBlend;

		checkError();
	}
}

void OpenGLImpl::setBlendFuncSeparate(const GLenum srcBlend, const GLenum destBlend, const GLenum srcAlpha, const GLenum destAlpha)
{
	if(mSrcBlend != srcBlend || mDestBlend != destBlend
		|| mSrcAlphaBlend != srcAlpha || mDestAlphaBlend != destAlpha)
	{
		glBlendFuncSeparate(srcBlend, destBlend, srcAlpha, destAlpha);
		mSrcBlend = srcBlend;
		mDestBlend = destBlend;
		mSrcAlphaBlend = srcAlpha;
		mDestAlphaBlend = destAlpha;

		checkError();
	}
}

void OpenGLImpl::activeDepthTest(const bool enable)
{
	if(mDepthTestEnable != enable)
	{    
		if(enable)
		{
			glEnable(GL_DEPTH_TEST);    
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

		mDepthTestEnable = enable;

		checkError();
	}
}
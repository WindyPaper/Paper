#include "OpenGLPlatform/RenderPreCompileHead.h"
#include "OpenGLPlatform/OpenGLRenderTexture.h"
#include "util/Log.h"
#include "OpenGLPlatform/OpenGLImpl.h"

OpenGLRenderTexture::OpenGLRenderTexture()
{
	mTexture = 0;
}

OpenGLRenderTexture::~OpenGLRenderTexture()
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
	glDeleteFramebuffers(1, &mFbo);
}

bool OpenGLRenderTexture::init(const uint width, const uint height)
{
	//here only test for shadowmap //fix me.
	//glEnable(GL_TEXTURE_2D);
	//glActiveTexture(GL_TEXTURE0);
	glGenFramebuffers(1, &mFbo);

	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);


	GLuint rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);	

	OpenGLImpl::getInstance().checkError();

	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	//glDrawBuffer(GL_LEFT);
	//glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		Log::getInstance().logMsg("FB error !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void OpenGLRenderTexture::bindForWriting()
{
	//glBindTexture(GL_TEXTURE_2D, mTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
	OpenGLImpl::getInstance().checkError();
}

void OpenGLRenderTexture::bindForReading(const GLenum textureUnit)
{
	if (mTexture != 0)
	{
		OpenGLImpl::getInstance().activeTexUnit(textureUnit);
		OpenGLImpl::getInstance().activeTexObj(GL_TEXTURE_2D, mTexture);
	}	
}

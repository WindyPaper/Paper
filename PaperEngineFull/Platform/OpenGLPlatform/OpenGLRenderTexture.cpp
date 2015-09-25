#include "OpenGLPlatform/RenderPreCompileHead.h"
#include "OpenGLPlatform/OpenGLRenderTexture.h"
#include "util/Log.h"
#include "OpenGLPlatform/OpenGLImpl.h"

OpenGLRenderTexture::OpenGLRenderTexture()
{

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mTexture, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		Log::getInstance().logMsg("FB error !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void OpenGLRenderTexture::bindForWriting()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
	OpenGLImpl::getInstance().checkError();
}

void OpenGLRenderTexture::bindForReading(const GLenum textureUnit)
{
	OpenGLImpl::getInstance().activeTexUnit(textureUnit);
	OpenGLImpl::getInstance().activeTexObj(GL_TEXTURE_2D, mTexture);
}

#include "OpenGLPlatform/RenderPreCompileHead.h"
#include "OpenGLPlatform/OpenGLRenderTexture.h"
#include "util/Log.h"
#include "OpenGLPlatform/OpenGLImpl.h"

OpenGLRenderTexture::OpenGLRenderTexture()
{

}

OpenGLRenderTexture::~OpenGLRenderTexture()
{
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
	glDeleteFramebuffers(1, &mFbo);
}

bool OpenGLRenderTexture::init(const uint width, const uint height)
{
	//here only test for shadowmap //fix me.
	glGenFramebuffers(1, &mFbo);

	glGenTextures(1, &mTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mTexture, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		Log::getInstance().logMsg("FB error");
		return false;
	}

	return true;
}

void OpenGLRenderTexture::bindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);
}

void OpenGLRenderTexture::bindForReading(const GLenum textureUnit)
{
	OpenGLImpl::getInstance().activeTexUnit(textureUnit);
	OpenGLImpl::getInstance().activeTexObj(GL_TEXTURE_2D, mTexture);
}

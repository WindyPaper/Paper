#include "OpenGLPlatform/RenderPreCompileHead.h"
#include "OpenGLPlatform/OpenGLGBuffer.h"
#include "OpenGLPlatform/OpenGLImpl.h"
#include "ResourceSystem/ITexture.h"

OpenGLGBuffer::OpenGLGBuffer()
{
	mFbo = 0;
	mDepthTex = 0;
	mNormalTex = 0;
	mWorldPosTex = 0;
	mFluxTex = 0;
}


OpenGLGBuffer::~OpenGLGBuffer()
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, 0, 0);
	glDeleteFramebuffers(1, &mFbo);
}

bool OpenGLGBuffer::init(const uint width, const uint height)
{
	glGenFramebuffers(1, &mFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);	

	//normal map
	glGenTextures(1, &mNormalTex);
	glBindTexture(GL_TEXTURE_2D, mNormalTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mNormalTex, 0);

	//pos map
	glGenTextures(1, &mWorldPosTex);
	glBindTexture(GL_TEXTURE_2D, mWorldPosTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mWorldPosTex, 0);

	//color or flux map
	glGenTextures(1, &mFluxTex);
	glBindTexture(GL_TEXTURE_2D, mFluxTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mFluxTex, 0);

	//depth map
	glGenTextures(1, &mDepthTex);
	glBindTexture(GL_TEXTURE_2D, mDepthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTex, 0);

	OpenGLImpl::getInstance().checkError();

	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	OpenGLImpl::getInstance().checkError();

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", status);
		return false;
	}

	// restore default FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void OpenGLGBuffer::bindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);
	OpenGLImpl::getInstance().checkError();
}

void OpenGLGBuffer::bindForReading()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	
	OpenGLImpl::getInstance().checkError();

	OpenGLImpl::getInstance().activeTexUnit(ShaderTexType::TexDepthMap);
	OpenGLImpl::getInstance().activeTexObj(GL_TEXTURE_2D, mDepthTex);

	OpenGLImpl::getInstance().activeTexUnit(ShaderTexType::TexGBNormalMap);
	OpenGLImpl::getInstance().activeTexObj(GL_TEXTURE_2D, mNormalTex);

	OpenGLImpl::getInstance().activeTexUnit(ShaderTexType::TexGBWorldPosMap);
	OpenGLImpl::getInstance().activeTexObj(GL_TEXTURE_2D, mWorldPosTex);

	OpenGLImpl::getInstance().activeTexUnit(ShaderTexType::TexGBFluxColorMap);
	OpenGLImpl::getInstance().activeTexObj(GL_TEXTURE_2D, mFluxTex);
}

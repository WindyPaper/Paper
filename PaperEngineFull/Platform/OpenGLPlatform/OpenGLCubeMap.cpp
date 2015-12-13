#include "OpenGLPlatform/RenderPreCompileHead.h"
#include "OpenGLPlatform/OpenGLCubeMap.h"
#include "OpenGLPlatform/OpenGLImpl.h"
#include "util/Global.h"

OpenGLCubeMap::OpenGLCubeMap()
{
	mTexType = TextureCube;
}

OpenGLCubeMap::~OpenGLCubeMap()
{

}

void OpenGLCubeMap::bind(const uint texUnit) const
{
	glEnable(GL_TEXTURE_CUBE_MAP);
	OpenGLImpl::getInstancePtr()->activeTexUnit(texUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeMapHandle);
}

bool OpenGLCubeMap::load(ResourceLoader *pLoader)
{
	assert(false);
	return false;
}

bool OpenGLCubeMap::load(const std::string &name)
{
	assert(false);
	return false;
}


bool OpenGLCubeMap::load(const std::string &name, const NameParamMap &paramMap)
{
	mName = name;

	ITexture *pTex[6];
	const std::string *image[6];

	for (int i = 0; i < 6; ++i)
	{
		char index_str[2];
		memset(index_str, 0, sizeof(index_str));

		sprintf_s(index_str, "%d", i);
		
		//std::string index = index_str;
		NameParamMap::const_iterator iter = paramMap.find(index_str);
		image[i] = &iter->second;

		pTex[i] = gEngModule->pTexMgr->create(*(image[i]));
		mTexhandle[i] = gEngModule->pTexMgr->getHandle(pTex[i]->getName());
	}

	glGenTextures(1, &mCubeMapHandle);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeMapHandle);

	OpenGLImpl::getInstance().checkError();

	for (int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, pTex[i]->getWidth(), pTex[i]->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, pTex[i]->lock());
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}

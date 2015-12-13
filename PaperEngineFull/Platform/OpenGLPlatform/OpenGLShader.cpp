#include "OpenGLPlatform/RenderPreCompileHead.h"
#include "OpenGLPlatform/OpenGLShader.h"
#include "ResourceSystem/FileStream.h"
#include "util/Log.h"
#include "OpenGLPlatform/OpenGLImpl.h"
#include "ResourceSystem/ITexture.h"

OpenGLShader::OpenGLShader() :
mpEffect(0)
{

}

OpenGLShader::OpenGLShader(const char *pVertexShader, const char *pFragShader) :
mpEffect(0)
{
}

OpenGLShader::~OpenGLShader()
{
	
}

void OpenGLShader::unload()
{
	nvFX::IContainer::destroy(mpEffect);
	mpEffect = 0;
}

void OpenGLShader::loadEffectFile(const char *pFileName)
{
	assert(mpEffect == 0);

	nvFX::setMessageCallback((nvFX::messageCallbackFunc)Log::logNvFX);
	std::string fullName("Res/Shader/");
	fullName.append(pFileName);
	nvFX::IContainer *pEffect = nvFX::IContainer::create(fullName.c_str());
	bool bRes = nvFX::loadEffectFromFile(pEffect, fullName.c_str());

	for (int i = 0; i < pEffect->getNumTechniques(); ++i)
	{
		nvFX::ITechnique *pTech = pEffect->findTechnique(i);
		pTech->validate();
		//pTech->getPass(0)->execute();
	}
	
	//pTech->getPass(0)->validate();
	mpEffect = pEffect;

	//fix me. need define g_uniform_block
	nvFX::ICstBuffer *pCstBuf = mpEffect->findCstBuffer("g_uniform_block");
	if (pCstBuf)
		pCstBuf->buildGLBuffer();

	mpCstBuf = pCstBuf;

	/*nvFX::IUniform *pUniform = mpEffect->findUniform("Texture0");
	pUniform->setImageUnit(0);*/
	_bindAllTexture();
}

void OpenGLShader::bindProgram() const
{
	mpEffect->findTechnique(0)->getPass(0)->execute();

	//nvFX::IUniform *pUniform = mpEffect->findUniform("Texture0");
	//pUniform->setImageUnit(2);
}

void OpenGLShader::bindProgram(const std::string &techName, const std::string &passName) const
{	
	nvFX::ITechnique *pTech = mpEffect->findTechnique(techName.c_str());
	for (int i = 0; i < pTech->getNumPasses(); ++i)
	{
		nvFX::IPass *pPass = pTech->getPass(i);
		if (pPass->getName() == passName)
		{
			pPass->execute();
		}
	}
	//mpEffect->findTechnique(techName.c_str())->get(passName.c_str())->execute();
}

void OpenGLShader::setVec4(const std::string &name, const math::Vector4f &vec4)
{
	if (mpCstBuf == 0)
		return;
	nvFX::IUniform *pUniform = mpCstBuf->findUniform(name.c_str());
	if (pUniform)
		pUniform->setValue4f(vec4.vec4.x, vec4.vec4.y, vec4.vec4.z, vec4.vec4.w);
	mpCstBuf->update();
}

void OpenGLShader::setVec3(const std::string &name, const math::Vector3f &vec3)
{
	if (mpCstBuf == 0)
		return;
	nvFX::IUniform *pUniform = mpCstBuf->findUniform(name.c_str());
	if (pUniform)
		pUniform->setValue3f(vec3.x, vec3.y, vec3.z);
	mpCstBuf->update();
}

void OpenGLShader::setMatrix(const std::string &name, const math::Matrix44 &matrix44)
{
	if (mpCstBuf == 0)
		return;
	nvFX::IUniform *pUniform = mpCstBuf->findUniform(name.c_str());
	XMMATRIX out_mat = XMMatrixTranspose(matrix44.m);
	pUniform->setMatrix4f(&out_mat.m[0][0]);
	mpCstBuf->update();
}

void OpenGLShader::bindShadowMap()
{
	nvFX::IUniform *pUniform = mpEffect->findUniform("shadowmap_tex");
	if (pUniform)
	{
		nvFX::ITechnique *pTech = mpEffect->findTechnique("TECH_DEFAULT");
		for (int i = 0; i < pTech->getNumPasses(); ++i)
		{
			nvFX::IPass *pPass = pTech->getPass(i);
			std::string passName = "_default";
			if (pPass->getName() == passName)
			{
				pUniform->setImageUnit(TexShadowMap);
				pUniform->update(pPass);
			}
		}
	}
		
		
}

void OpenGLShader::_bindAllTexture()
{
	for (int i = 0; i < ShaderTexTypeNum; ++i)
	{
		std::string texStr;

		ShaderTexType type = static_cast<ShaderTexType>(i);
		switch (type)
		{
		case TexDiffuse:
			texStr = "diffuse_tex";
			break;
		case TexNormal:
			texStr = "normal_tex";
			break;
		case TexSpecular:
			texStr = "specular_tex";
			break;
		case TexShadowMap:
			texStr = "shadowmap_tex";
			break;
		default:
			break;
		}
		nvFX::IUniform *pUniform = mpEffect->findUniform(texStr.c_str());
		if (pUniform)
		{
			pUniform->setImageUnit(i);
			//pUniform->update();
		}
	}
	
}

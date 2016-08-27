#include "OpenGLPlatform/RenderPreCompileHead.h"
#include "OpenGLPlatform/OpenGLMaterial.h"
#include "util/Global.h"
#include "OpenGLPlatform/OpenGLShader.h"



using namespace rapidxml;

OpenGLMaterial::OpenGLMaterial()
{
	mTextureHandleVec.resize(MAX_MAT_TEX_NUM);
}

OpenGLMaterial::~OpenGLMaterial()
{
	mSubMaterialVec.clear();
}

bool OpenGLMaterial::load(const std::string &name)
{
	mName = name;
	return loadMaterialByName(name);
}

void OpenGLMaterial::unload()
{

}

bool OpenGLMaterial::loadMaterialByName(const std::string &name)
{
	parseMtlFile(name);

	

	return true;
}

void OpenGLMaterial::bindAllTexture()
{
	for (int i = 0; i < mTextureHandleVec.size(); ++i)
	{
		if (mTextureHandleVec[i].isNull() == false)
		{
			ITexture *pTex = gEngModule->pTexMgr->getDataPtr(mTextureHandleVec[i]);
			pTex->bind(i);
		}
	}
}

void OpenGLMaterial::setTexture(TexHandle texHanle, int slot)
{
	assert(slot < MAX_MAT_TEX_NUM);

	mTextureHandleVec[slot] = texHanle;
}

IShader * OpenGLMaterial::getShader()
{
	return gEngModule->pShaderMgr->getDataPtr(mShaderHandle);
}

IShader * OpenGLMaterial::getSubShader(const int index)
{
	if (mSubMaterialVec.size() > index)
	{
		return mSubMaterialVec[index]->getShader();
	}
	return 0;
}

const SubMaterialVec & OpenGLMaterial::getSubMaterialVec() const
{
	return mSubMaterialVec;
}

void OpenGLMaterial::loadMaterialFromXMLNode(rapidxml::xml_node<> *pEffectNode)
{
	std::string vsName("res/shader/");
	std::string fsName("res/shader/");
	int maxParamNum = 0;
	std::string shaderName;
	for (xml_attribute<> *attr = pEffectNode->first_attribute(); attr; attr = attr->next_attribute())
	{
		if (strcmp(attr->name(), "effectName") == 0)
		{
			shaderName = attr->value();
		}
		/*else if (strcmp(attr->name(), "vs") == 0)
		{
			vsName.append(attr->value());
		}
		else if (strcmp(attr->name(), "fs") == 0)
		{
			fsName.append(attr->value());
		}*/
		else if (strcmp(attr->name(), "ParamNum") == 0)
		{
			std::stringstream stream(attr->value());
			stream >> maxParamNum;
		}
	}

	/*nvFX::setMessageCallback((nvFX::messageCallbackFunc)Log::logNvFX);
	nvFX::IContainer *pEffect = nvFX::IContainer::create("test_effect");
	bool bRes = nvFX::loadEffectFromFile(pEffect, "Res/shader/common.glslfx");
	nvFX::ITechnique *pTech = pEffect->findTechnique(0);
	pTech->validate();
	pTech->getPass(0)->validate();*/

	mShaderHandle = gEngModule->pShaderMgr->getHandle(shaderName);
	if (mShaderHandle.isNull() == true)
	{
		IShader *pShader = gEngModule->pShaderMgr->create(shaderName);
		OpenGLShader *pGLShader = dynamic_cast<OpenGLShader*>(pShader);
		pGLShader->loadEffectFile(shaderName.c_str());
		mShaderHandle = gEngModule->pShaderMgr->getHandle(shaderName);
	}
	IShader *pShader = gEngModule->pShaderMgr->getDataPtr(mShaderHandle);
	
	xml_node<> *pParamNode = pEffectNode->first_node();
	while (pParamNode)
	{
		std::string paramName;
		std::string paramType;
		std::string paramValueStr;
		for (xml_attribute<> *attr = pParamNode->first_attribute(); attr; attr = attr->next_attribute())
		{
			if (strcmp(attr->name(), "name") == 0)
			{
				paramName = attr->value();
			}
			else if (strcmp(attr->name(), "type") == 0)
			{
				paramType = attr->value();
			}
			else if (strcmp(attr->name(), "value") == 0)
			{
				paramValueStr = attr->value();
			}
		}

		if (paramType == "float4")
		{
			math::Vector4f vec4;
			std::stringstream stream(paramValueStr);
			stream >> vec4.vec4.x >> vec4.vec4.y >> vec4.vec4.z >> vec4.vec4.w;
			pShader->setVec4(paramName, vec4);
		}

		pParamNode = pParamNode->next_sibling();
	}

	xml_node<> *pTexNode = pEffectNode->next_sibling();
	int maxTexNum = 0;
	for (xml_attribute<> *attr = pTexNode->first_attribute(); attr; attr = attr->next_attribute())
	{
		if (strcmp(attr->name(), "MaxTexNum") == 0)
		{
			std::stringstream stream(attr->value());
			stream >> maxTexNum;
		}
	}

	xml_node<> *pEleTexNode = pTexNode->first_node();
	while (pEleTexNode)
	{
		size_t texShaderType = 0;
		for (xml_attribute<> *attr = pEleTexNode->first_attribute(); attr; attr = attr->next_attribute())
		{
			if (strcmp(attr->name(), "Type") == 0)
			{
				if (strcmp(attr->value(), "Diffuse") == 0)
				{
					texShaderType = ShaderTexType::TexDiffuse;
				}
				else if (strcmp(attr->value(), "Normal") == 0)
				{
					texShaderType = ShaderTexType::TexNormal;
				}
				else if (strcmp(attr->value(), "Mask") == 0)
				{
					texShaderType = ShaderTexType::TexMask;
				}
			}
			if (strcmp(attr->name(), "FileName") == 0)
			{
				std::string texName("res/");
				texName.append(attr->value());
				ITexture *pTex = gEngModule->pTexMgr->create(texName);
				mTextureHandleVec[texShaderType] = gEngModule->pTexMgr->getHandle(texName);
			}

		}

		pEleTexNode = pEleTexNode->next_sibling();
	}
}

void OpenGLMaterial::parseMtlFile(const std::string &fileName)
{
	file<> fileDoc(fileName.c_str());
	xml_document<> doc;
	doc.parse<0>(fileDoc.data());

	xml_node<> *pRootNode = doc.first_node();

	xml_node<> *pEffectNode = pRootNode->first_node();
		
	loadMaterialFromXMLNode(pEffectNode);
			
}

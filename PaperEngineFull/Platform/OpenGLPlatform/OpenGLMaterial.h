#ifndef _OPENGL_MATERIAL_H_
#define _OPENGL_MATERIAL_H_

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"  
#include "rapidxml/rapidxml_print.hpp" 

#include "ResourceSystem/IResMgr.h"
#include "ResourceSystem/IMaterial.h"

#define MAX_MAT_TEX_NUM 16

class IShader;

class ENGINE_DLL OpenGLMaterial : public IMaterial
{
public:
	OpenGLMaterial();
	virtual ~OpenGLMaterial();

	virtual bool load(ResourceLoader *pLoader) { return true; }
	virtual bool load(const std::string &name);
	virtual bool load(const std::string &name, const NameParamMap &paramMap){ return true; }

	virtual void unload();

	virtual bool loadMaterialByName(const std::string &name);

	virtual void bindAllTexture();
	virtual void setTexture(TexHandle texHanle, int slot);
	
	virtual IShader *getShader();
	virtual IShader *getSubShader(const int index);

	virtual const SubMaterialVec &getSubMaterialVec() const;

	void loadMaterialFromXMLNode(rapidxml::xml_node<> *pEffectNode);
	void parseMtlFile(const std::string &fileName);
	
private:
	ShaderHandle mShaderHandle;
	std::vector<TexHandle> mTextureHandleVec;


	//���Sub Material��������ʱ�Ľ�������������Ӹ����ʱ༭����Ȼ��ָ���Ӧ���Ѿ�д�õ�Material����ʱ��Ӧ����MaterialHandle��
	//I find the best solution for now...
	
	SubMaterialVec mSubMaterialVec;
};

#endif
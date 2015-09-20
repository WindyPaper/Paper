#ifndef _I_MATERIAL_H_
#define _I_MATERIAL_H_

#include <vector>
#include "util/PreDefine.h"
#include "util/Paper_Math.h"
#include "ResourceSystem/Resource.h"
#include "ResourceSystem/IShader.h"
#include "ResourceSystem/ShaderParam.h"
#include "ResourceSystem/ITexture.h"

const int MAX_MATERIAL_TEX_NUM = 8;

enum TextureSlot
{
	DiffuseSlot = 0,
	NormalMapSlot,
};

class MaterialParams
{
public:
	typedef std::vector<ShaderParam*> ShaderParamVec;

	MaterialParams()
	{

	}

	~MaterialParams()
	{
		clearAllParam();
	}

	ShaderParam *getShaderParam(const std::string &name)
	{
		ShaderParam *ret = 0;
		for (ShaderParamVec::iterator iter = mParamVec.begin(); iter != mParamVec.end(); ++iter)
		{
			if ((*iter)->paramName == name)
			{
				ret = (*iter);
				break;
			}
		}
		return ret;
	}

	void addShaderParam(ShaderParam &param)
	{
		ShaderParam *pGetShader = getShaderParam(param.paramName);
		if (pGetShader == 0)
		{
			ShaderParam *pNewParam = new ShaderParam(param);
			mParamVec.push_back(pNewParam);
		}
	}

	void clearAllParam()
	{
		for (ShaderParamVec::iterator iter = mParamVec.begin(); iter != mParamVec.end(); ++iter)
		{
			delete (*iter);
		}

		mParamVec.clear();
	}

private:
	MaterialParams(const MaterialParams &other);
	MaterialParams &operator =(const MaterialParams &other);

	ShaderParamVec mParamVec;
};

class IMaterial;
typedef std::vector<IMaterial*> SubMaterialVec;
class IMaterial : public Resource
{
public:
	virtual ~IMaterial() {}

	virtual bool loadMaterialByName(const std::string &name) = 0;

	//for test easily. it will be imported from Material file internal.
	//以下接口只是为了测试方便，后面这些值都会从材质的内部获取，不向外面暴露接口
	//virtual void setValue(const std::string &name, const float fValue) = 0;
	//virtual void setValue(const std::string &name, const math::Vector2f &vec2) = 0;
	//virtual void setValue(const std::string &name, const math::Vector3f &vec3) = 0;

	virtual void bindAllTexture() = 0;
	virtual void setTexture(TexHandle texHanle, int slot) = 0;

	virtual IShader *getShader() = 0;
	virtual const SubMaterialVec &getSubMaterialVec() const = 0;
};

typedef ResHandle<IMaterial*> MaterialHandle;


#endif
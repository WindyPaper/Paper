#ifndef _I_SHADER_H_
#define _I_SHADER_H_

#include <string>
#include "ResourceSystem/Resource.h"
#include "ResourceSystem/ResHandle.h"
#include "ResourceSystem/ShaderParam.h"
#include "util/Paper_Math.h"

enum ShaderType
{
	UnknowType,

	VertexShader,
	GemotryShader,
	FragmentShader,
};

class ENGINE_DLL IShader : public Resource
{
public:
	virtual ~IShader() {}

	/*virtual void setFloat(const std::string &name, const float fValue) = 0;
	virtual void setInt(const std::string &name, const int iValue) = 0;
	virtual void setVec2(const std::string &name, const math::Vector2f &vec2) = 0;
	virtual void setVec3(const std::string &name, const math::Vector3f &vec3) = 0;
	virtual void setVec4(const std::string &name, const math::Vector4f &vec4) = 0;
	virtual void setMatrix(const std::string &name, const math::Matrix44 &matrix44) = 0;

	virtual void setValue(const ShaderParam &param) = 0;*/
	virtual void setVec4(const std::string &name, const math::Vector4f &vec4) = 0;
	virtual void setVec3(const std::string &name, const math::Vector3f &vec3) = 0;
	virtual void setMatrix(const std::string &name, const math::Matrix44 &matrix44) = 0;
};

typedef ResHandle<IShader*> ShaderHandle;

#endif
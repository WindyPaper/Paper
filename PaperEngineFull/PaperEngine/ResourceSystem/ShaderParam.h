#ifndef _SHADER_PARAM_H_
#define _SHADER_PARAM_H_

#include <string>
#include "util/Engine_Define.h"
#include "util/Paper_Math.h"

enum ShaderParamType
{
	Invalid = -1,
	Vec4,
	Vec3,
	Vec2,
	Float,
	Int,
	Matrix,
	String,
};

struct ENGINE_DLL ShaderParam
{
	ShaderParamType type;
	void *pValue;
	uint memByte;
	std::string paramName;

	ShaderParam() :
		type(Invalid)
	{
		memByte = 0;
		pValue = 0;
		paramName = "none";
	}

	ShaderParam(const std::string &name, const math::Vector3f &vec3) :
		type(Vec3)
	{
		memByte = sizeof(math::Vector3f);
		pValue = new char[memByte];
		paramName = name;
		memcpy(pValue, &vec3, memByte);
	}

	void setValue(const math::Vector3f &vec3)
	{
		if (type == Vec3)
		{
			memcpy(pValue, &vec3, memByte);
		}
	}

	ShaderParam(const std::string &name, const math::Vector2f &vec2) :
		type(Vec2)
	{
		memByte = sizeof(math::Vector2f);
		pValue = new char[memByte];
		paramName = name;
		memcpy(pValue, &vec2, memByte);
	}

	void setValue(const math::Vector2f &vec2)
	{
		if (type == Vec2)
		{
			memcpy(pValue, &vec2, memByte);
		}
	}

	ShaderParam(const std::string &name, const float &fValue) :
		type(Float)
	{
		memByte = sizeof(float);
		pValue = new char[memByte];
		paramName = name;
		memcpy(pValue, &fValue, memByte);
	}

	void setValue(const float &fValue)
	{
		if (type == Float)
		{
			memcpy(pValue, &fValue, memByte);
		}
	}

	ShaderParam(const std::string &name, const int &iValue) :
		type(Int)
	{
		memByte = sizeof(int);
		pValue = new char[memByte];
		paramName = name;
		memcpy(pValue, &iValue, memByte);
	}

	void setValue(const int &iValue)
	{
		if (type == Int)
		{
			memcpy(pValue, &iValue, memByte);
		}
	}

	ShaderParam(const std::string &name, const std::string &str) :
		type(String)
	{
		memByte = str.length();
		pValue = new char[memByte];
		paramName = name;
		memcpy(pValue, str.c_str(), str.length());
	}

	void setValue(const std::string &str)
	{
		if (type == String)
		{
			delete[] pValue;
			pValue = 0;

			memByte = str.length();
			pValue = new char[memByte];
			memcpy(pValue, str.c_str(), memByte);
		}
	}

	ShaderParam(const std::string &name, const math::Matrix44 &matrix44) :
		type(Matrix)
	{
		memByte = sizeof(math::Matrix44);
		pValue = new char[memByte];
		paramName = name;
		memcpy(pValue, &matrix44, memByte);
	}

	void setValue(const math::Matrix44 &matrix44)
	{
		if (type == Matrix)
		{
			memcpy(pValue, &matrix44, memByte);
		}
	}

	~ShaderParam()
	{
		delete[] pValue;
	}


	ShaderParam(const ShaderParam &other)
	{
		paramName = other.paramName;
		memByte = other.memByte;
		pValue = new char[memByte];
		type = other.type;
		memcpy(pValue, other.pValue, memByte);
	}

	ShaderParam &operator =(const ShaderParam &other)
	{
		if (pValue != 0)
		{
			delete[] pValue;
			pValue = 0;
		}		

		paramName = other.paramName;
		memByte = other.memByte;
		pValue = new char[memByte];
		type = other.type;
		memcpy(pValue, other.pValue, memByte);

		return *this;
	}
};

#endif
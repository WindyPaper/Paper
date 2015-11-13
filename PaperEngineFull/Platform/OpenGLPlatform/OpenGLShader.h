#ifndef _OPENGL_SHADER_H_
#define _OPENGL_SHADER_H_

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <GL/glew.h>
#include "ResourceSystem/IShader.h"

#include "FxLib.h"

//attribute
enum BindShaderAttr
{
	ATTR_POSITION_ARRAY = 0,
	ATTR_TEXCOOD_ARRAY,
	ATTR_NORMAL_ARRAY,
	ATTR_COLOR_INT,
	ATTR_MVP_MAT_ARRAY,
	ATTR_WORLD_MAT_ARRAY,
};

class ENGINE_DLL OpenGLShader : public IShader
{
public:
	OpenGLShader();
	OpenGLShader(const char *pVertexShader, const char *pFragShader);
	~OpenGLShader();

	virtual bool load(ResourceLoader *pLoader) { return true; } //empty
	virtual bool load(const std::string &name) { mName = name; return true; } //empty
	virtual bool load(const std::string &name, const NameParamMap &paramMap){ return true; } //empty

	virtual void unload();

	void loadEffectFile(const char *pFileName);
	/*void loadShaderFromFile(const char *vsFileName, const char *psFileName);
	void setVertexShader(const char *pVertexShader);
	void setFragShader(const char *pFragShader);
	GLint createShader(const char *pShader, const ShaderType shaderType);
	void bindProgram() const;

	void addAttrbute(const GLint index, const char *name);

	bool link();*/

	void bindProgram() const;
	void bindProgram(const std::string &techName, const std::string &passName) const;

	virtual void setVec4(const std::string &name, const math::Vector4f &vec4);
	virtual void setMatrix(const std::string &name, const math::Matrix44 &matrix44);

	/*virtual void setFloat(const std::string &name, const float fValue);
	virtual void setInt(const std::string &name, const int iValue);
	virtual void setVec2(const std::string &name, const math::Vector2f &vec2);
	virtual void setVec3(const std::string &name, const math::Vector3f &vec3);
	virtual void setVec4(const std::string &name, const math::Vector4f &vec4);
	virtual void setMatrix(const std::string &name, const math::Matrix44 &matrix44);

	virtual void setValue(const ShaderParam &param);*/

	//const ActiveAttriMap &getActiveAttriMap() const { return mActiveAttriMap; }
	//const ActiveUniformVec &getActiveUniform() const { return mActiveUniformVec; }

	//only for test fix me
	void bindShadowMap();

protected:
	void _bindAllTexture();

protected:
	//void compiler();
	//std::auto_ptr<char> getFileStr(const char *fileName);
	//void initActiveAttribute();
	//void initActiveUniform();

	//std::vector<GLint> mActiveAttriLocVec;
	//ActiveAttriMap mActiveAttriMap;
	//ActiveUniformVec mActiveUniformVec;
	//const char *mpVertexShader;
	//const char *mpFragShader;
/*
	GLint mVertexHandler;
	GLint mFragHandler;

	GLint mProgram;*/

	nvFX::IContainer *mpEffect;
	nvFX::ICstBuffer *mpCstBuf;
};

#endif

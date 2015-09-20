#ifndef _OPENGL_SHADER_MGR_H_
#define _OPENGL_SHADER_MGR_H_

#include <map>
#include "util/Singleton.h"
#include "ResourceSystem/IResMgr.h"
#include "OpenGLPlatform/OpenGLShader.h"
#include "ResourceSystem/ResHandleMgr.h"

class ENGINE_DLL OpenGLShaderMgr : public IShaderMgr, public Singleton<OpenGLShaderMgr>
{
public:
	static OpenGLShaderMgr& getInstance(void);
	static OpenGLShaderMgr* getInstancePtr(void);

	typedef ResHandleMgr<OpenGLShader, ShaderHandle> ShaderContain;
	typedef std::map<std::string, ShaderHandle> ResHandleMap;

	OpenGLShaderMgr();
	virtual ~OpenGLShaderMgr();

	virtual IShader *create(const std::string &name);
	virtual IShader *create(const std::string &name, const NameParamMap &paramMap);

	virtual ShaderHandle getHandle(const std::string &name);
	virtual IShader *getDataPtr(const std::string &name);
	virtual IShader *getDataPtr(ShaderHandle &handle);

	virtual void deleteData(ShaderHandle &handle);

protected:
	IShader *_createShaderFast(const std::string &name);

private:
	ResHandleMap mResHandleMap;
	ShaderContain mShaderContain;
};

#endif
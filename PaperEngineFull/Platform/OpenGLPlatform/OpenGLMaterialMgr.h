#ifndef _OPENGL_MATERIAL_MGR_H_
#define _OPENGL_MATERIAL_MGR_H_

#include <map>
#include <string>
#include "util/Singleton.h"
#include "ResourceSystem/IResMgr.h"
#include "ResourceSystem/ResHandleMgr.h"
#include "OpenGLPlatform/OpenGLMaterial.h"
#include "util/GlobalShaderParamMng.h"

#define DEFAULT_MATERIAL_NAME "DefaultMaterial"

class ENGINE_DLL OpenGLMaterialMgr : public IMaterialMgr, public Singleton<OpenGLMaterialMgr>
{
public:
	static OpenGLMaterialMgr& getInstance(void);
	static OpenGLMaterialMgr* getInstancePtr(void);

	typedef ResHandleMgr<OpenGLMaterial, MaterialHandle> MaterialContain;
	typedef std::map<std::string, MaterialHandle> ResHandleMap;

	OpenGLMaterialMgr();
	virtual ~OpenGLMaterialMgr();

	virtual IMaterial *create(const std::string &name);
	virtual IMaterial *create(const std::string &name, const NameParamMap &paramMap);

	virtual MaterialHandle getHandle(const std::string &name);
	virtual IMaterial *getDataPtr(const std::string &name);
	virtual IMaterial *getDataPtr(MaterialHandle &handle);

	virtual void deleteData(MaterialHandle &handle);

protected:
	IMaterial *_createMaterialFast(const std::string &name);
	IMaterial *_createMaterialFast(const std::string &name, const std::string &fileName);

private:
	ResHandleMap mResHandleMap;
	MaterialContain mMaterialContain;

	GlobalShaderParamMng *pGlobalShaderParam;
};

#endif
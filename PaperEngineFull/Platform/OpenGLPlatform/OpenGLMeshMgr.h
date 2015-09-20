#ifndef _GL_MESH_MGR_H_
#define _GL_MESH_MGR_H_

#include <string>
#include <map>

#include "util/Platform.h"
#include "util/Singleton.h"
#include "ResourceSystem/ResHandle.h"
#include "ResourceSystem/ResHandleMgr.h"
#include "OpenGLPlatform/OpenGLMesh.h"
#include "ResourceSystem/IResMgr.h"

class ENGINE_DLL OpenGLMeshMgr : public IMeshMgr, public Singleton<OpenGLMeshMgr>
{
public:
	static OpenGLMeshMgr& getInstance(void);
	static OpenGLMeshMgr* getInstancePtr(void);

	typedef ResHandleMgr<OpenGLMesh, MeshHandle> MeshContain;
	typedef std::map<std::string, MeshHandle> ResHandleMap;

	OpenGLMeshMgr();
	~OpenGLMeshMgr();

	IMesh *create(const std::string &name);
	IMesh *create(const std::string &name, const NameParamMap &paramMap);

	IMesh *getDataPtr(MeshHandle &handle);
	IMesh *getDataPtr(const std::string &name);
	MeshHandle getHandle(const std::string &name);

	void deleteData(MeshHandle &handle);

protected:
	IMesh *_createMeshFast(const std::string &name);
	IMesh *_createMeshFast(const std::string &name, const NameParamMap &paramMap);

private:
	MeshContain mMeshContain;
	ResHandleMap mResHandleMap;
};

#endif
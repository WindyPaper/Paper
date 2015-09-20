#ifndef _I_RES_MGR_H_
#define _I_RES_MGR_H_

#include <string>
#include "ResourceSystem/IMesh.h"
#include "ResourceSystem/IShader.h"
#include "ResourceSystem/ITexture.h"
#include "ResourceSystem/IMaterial.h"

template <typename DATA, typename DATAHANDLE>
class IResMgr
{
public:
	virtual ~IResMgr() {}

	virtual DATA *create(const std::string &name) = 0;
	virtual DATA *create(const std::string &name, const NameParamMap &paramMap) = 0;
	//virtual DATAHANDLE createWithHandle(const std::string &name) { return 0; }
	//virtual DATAHANDLE createWithHandle(const std::string &name, const std::string &fileName) { return 0; }

	virtual DATAHANDLE getHandle(const std::string &name) = 0;
	virtual DATA *getDataPtr(const std::string &name) = 0;
	virtual DATA *getDataPtr(DATAHANDLE &handle) = 0;

	virtual void deleteData(DATAHANDLE &handle) = 0;
};

typedef IResMgr<IMesh, MeshHandle> IMeshMgr;
typedef IResMgr<ITexture, TexHandle> ITextureMgr;
typedef IResMgr<IShader, ShaderHandle> IShaderMgr;
typedef IResMgr<IMaterial, MaterialHandle> IMaterialMgr;

#endif
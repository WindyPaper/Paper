#ifndef _ASSIMP_MESH_LOADER_H_
#define _ASSIMP_MESH_LOADER_H_

#include "ResourceSystem/ResourceLoader.h"

struct aiScene;
class aiMesh;

class ENGINE_DLL AssimpMeshLoader : public ResourceLoader
{
public:
	virtual bool loadRes(Resource *res);

protected:
	void loadMesh(Resource *res);
	void initFromScene(Resource *res, const aiScene *pScene, const char *name);
	void initMesh(Resource *res, const aiMesh *pMesh);
	void initTexture(Resource *res, const aiScene *pScene, const std::string &fileName);
};

#endif
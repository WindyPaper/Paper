#ifndef _CUSTOM_MESH_LOADER_H_
#define _CUSTOM_MESH_LOADER_H_

#include <memory>
#include "ResourceSystem/IMesh.h"
#include "ResourceSystem/ResourceLoader.h"

struct ENGINE_DLL MeshInfo
{
	int vertexCount;
	int indexCount;
	int subMeshCount;


	MeshInfo() :
		vertexCount(0),
		indexCount(0),
		subMeshCount(1)
	{

	}
};

class ENGINE_DLL CustomMeshLoader : public ResourceLoader
{
public:
	virtual bool loadRes(Resource *res);

protected:
	void loadMesh(Resource *res);
};

#endif
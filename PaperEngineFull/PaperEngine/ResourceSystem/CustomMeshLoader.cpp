#include "util/PrecompileHead.h"
#include "ResourceSystem/CustomMeshLoader.h"
#include "ResourceSystem/IMesh.h"

bool CustomMeshLoader::loadRes(Resource *res)
{
	loadMesh(res);
	return true;
}


void CustomMeshLoader::loadMesh(Resource *res)
{
	IMesh *pMesh = dynamic_cast<IMesh*>(res);

	//FILE *pFile = fopen(res->getName().c_str(), "r");
	std::ifstream inStream;
	inStream.open(res->getName(), std::ios::in | std::ios::binary);	

	// 这个指针创建的内存生命周期 由该Mesh管理，后面看看有没有更好的方法，谁创建 谁管理。。。
	//the memory of this point is managered by The Creating Mesh...
	inStream.seekg(0, std::ios::end);
	int fileLength = inStream.tellg();
	inStream.seekg(0, std::ios::beg);
	unsigned char *pMemCopyFile = new unsigned char[fileLength];

	inStream.read((char*)pMemCopyFile, fileLength);	

	unsigned char *pCurrMem = pMemCopyFile;
	MeshInfo meshInfo;
	memcpy(&meshInfo, pMemCopyFile, sizeof(MeshInfo));
	pCurrMem += sizeof(MeshInfo);

	const int subMeshCount = meshInfo.subMeshCount;
	std::vector<SubMesh> subMeshVec;
	//subMeshVec.reserve(4);
	for (int i = 0; i < meshInfo.subMeshCount; ++i)
	{
		SubMesh subMesh;
		memcpy(&subMesh, pCurrMem, sizeof(SubMesh));
		subMeshVec.push_back(subMesh);
		pCurrMem += sizeof(SubMesh);

	}
	pMesh->setSubMeshVec(subMeshVec);
	pMesh->setMeshMemData(pMemCopyFile);

	//Import Vertex
	pMesh->setMeshMemVertexData(pCurrMem, meshInfo.vertexCount, VertexDataSortType::P3UVF2N3T3);
	pCurrMem += meshInfo.vertexCount * sizeof(Vertex_P3UVF2N3T3);

	//Import Index
	pMesh->setMeshMemIndexData(pCurrMem, meshInfo.indexCount, BIT32);

	pCurrMem += meshInfo.indexCount * sizeof(int);

	inStream.close();
}

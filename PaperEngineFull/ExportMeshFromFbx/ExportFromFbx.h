#ifndef _EXPORT_FROM_FBX_H_
#define _EXPORT_FROM_FBX_H_

#include <string>
#include <vector>
#include <map>
#include <fbxsdk.h>

// Four floats for every position.
const int VERTEX_STRIDE = 4;
// Three floats for every normal.
const int NORMAL_STRIDE = 3;
// Two floats for every UV.
const int UV_STRIDE = 2;

const int MAX_MESH_NAME = 16;

struct SubMesh
{
	SubMesh() : indexOffset(0), triangleCount(0) {}

	int indexOffset;
	int triangleCount;
};

struct MeshInfo
{
	int vertexCount;
	int indexCount;
	int subMeshCount;
	char meshName[MAX_MESH_NAME];
	//int offsetByte;

	MeshInfo() :
		vertexCount(0),
		indexCount(0),
		subMeshCount(1)
	{
		memset(meshName, 0, MAX_MESH_NAME);
	}
};

struct ExportHeadInfo
{
	int meshNum;
	int allVertexNum;
	int allIndexNum;

	ExportHeadInfo() :
		meshNum(0),
		allVertexNum(0),
		allIndexNum(0)
	{

	}
};

struct ExportMeshVertexFormat
{
	float x, y, z;
	float uvx, uvy;

	ExportMeshVertexFormat() :
		x(0.0),
		y(0.0),
		z(0.0),
		uvx(0.0),
		uvy(0.0)
	{

	}
};

struct OneMesh
{
	std::string meshName;
	int vertexCount;
	int indexCount;
	ExportMeshVertexFormat *pVertexData;
	unsigned int *pIndexData;
	FbxArray<SubMesh*> subMeshVec;
	std::vector<std::string> texNameVec;

	OneMesh() :
		vertexCount(0),
		indexCount(0),
		pVertexData(0),
		pIndexData(0)
	{

	}

	~OneMesh()
	{
		for (int i = 0; i < subMeshVec.Size(); ++i)
		{
			delete subMeshVec[i];
		}
		subMeshVec.Clear();

		delete pVertexData;
		pVertexData = 0;
		delete pIndexData;
		pIndexData = 0;
	}
};

class ExportFromFbx
{
public:
	ExportFromFbx(const std::string &fileName);
	~ExportFromFbx();

	void initializeSdkObjects();
	void loadFbx(const std::string &fileName);
	OneMesh *processMesh(FbxMesh *pMesh);

	void saveData(std::string fileName);

protected:
	void loadCacheRecursive(FbxNode *pNode);
	void saveMeshData(OneMesh *pMesh);
	void saveMaterialXml(OneMesh *pMesh);
	void saveModelData(std::string fileName);
	FbxAMatrix getGeometry(FbxNode* pNode);

private:
	FbxManager *mpManager;
	FbxScene *mpScene;
	FbxImporter *mpImporter;

	//FbxArray<SubMesh*> mSubMeshVec;

	//MeshHeadInfo mHeadInfo;

	std::vector<OneMesh*> mMeshVec;
	std::string mCreatePathDict;

	//typedef std::map<FbxMesh*, std::vector<std::string>> NodeToTexNameVecMap;
	//NodeToTexNameVecMap mMeshToTexVecMap;
};

#endif
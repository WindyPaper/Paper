#include "util/PrecompileHead.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>
#include "util/Global.h"
#include "ResourceSystem/AssimpMeshLoader.h"
//#include "ResourceSystem/MeshRes.h"
#include "ResourceSystem/IMesh.h"
#include "util/SmartPtr.h"
#include "ResourceSystem/MagicImageLoader.h"

bool AssimpMeshLoader::loadRes(Resource *res)
{
	loadMesh(res);
	return true;
}

void AssimpMeshLoader::loadMesh(Resource *res)
{
	Assimp::Importer import;
	const aiScene *pScene = import.ReadFile(res->getName(), aiProcess_Triangulate | aiProcess_GenSmoothNormals);
	if(pScene)
	{
		initFromScene(res, pScene, res->getName().c_str());
	}
}

void AssimpMeshLoader::initFromScene(Resource *res, const aiScene *pScene, const char *name)
{
	int index = 0;

	const aiMesh *pMesh = pScene->mMeshes[index];
	initMesh(res, pMesh);
	initTexture(res, pScene, name);
}

void AssimpMeshLoader::initMesh(Resource *res, const aiMesh *pMesh)
{
	IMesh *pRes = dynamic_cast<IMesh*>(res);

	VertexData vertexData;
	IndexData indexData;

	Vertex_P3F2F3 *pVertexMemData = new Vertex_P3F2F3[pMesh->mNumVertices];
	//memset(pVertexMemData, 0, pMesh->mNumVertices * sizeof(Vertex_P3F2F3));
	Vertex_P3F2F3 *pCurrData = pVertexMemData;

	const aiVector3D zero3D(0.0f, 0.0f, 0.0f);
	for(unsigned int i = 0; i < pMesh->mNumVertices; ++i)
	{
		const aiVector3D *pPos = &(pMesh->mVertices[i]);
		const aiVector3D *pNormal = &(pMesh->mNormals[i]);
		const aiVector3D *pTexcood = pMesh->HasTextureCoords(0) ? &(pMesh->mTextureCoords[0][i]) : &zero3D;

		Vertex_P3F2F3 perVertexData;
		int size = sizeof(Vertex_P3F2F3);
		perVertexData.pos = math::Vector3f(pPos->x, pPos->y, pPos->z);
		perVertexData.normal = math::Vector3f(pNormal->x, pNormal->y, pNormal->z);
		perVertexData.texcood = math::Vector2f(pTexcood->x, pTexcood->y);

		*pCurrData = perVertexData;
		++pCurrData;
	}
	vertexData.pMemData = reinterpret_cast<unsigned char*>(pVertexMemData);
	vertexData.elementCount = pMesh->mNumVertices;
	vertexData.type = VertexDataSortType::P3F2F3;
	vertexData.elementSize = sizeof(Vertex_P3F2F3);

	char *pIndexMemData = new char[pMesh->mNumFaces * 3 * 4];
	//memset(pIndexMemData, 0, pMesh->mNumFaces * 3 * 4);
	uint32 *pCurrIndexData = reinterpret_cast<uint32*>(pIndexMemData);
	for(uint i = 0;i < pMesh->mNumFaces; ++i)
	{
		//fix me && can be better
		const aiFace *pFace = &pMesh->mFaces[i];
		assert(pFace->mNumIndices == 3);
		for(uint index = 0; index < 3; ++index)
		{
			//pRes->getIndex32().push_back(pFace->mIndices[index]);

			*pCurrIndexData = pFace->mIndices[index];
			++pCurrIndexData;
		}
	}
	//pRes->setIndexType(IndexType::Int32);
	indexData.pMemData = reinterpret_cast<unsigned char*>(pIndexMemData);
	indexData.elementCount = pMesh->mNumFaces * 3;
	indexData.elementSize = 4;
	indexData.type = IndexType::BIT32;
	
	pRes->setMeshMemVertexData(vertexData);
	pRes->setMeshMemIndexData(indexData);
}

void AssimpMeshLoader::initTexture(Resource *res, const aiScene *pScene, const std::string &fileName)
{
	//MeshRes *pMeshRes = dynamic_cast<MeshRes*>(res);

	std::string::size_type slashIndex = fileName.find_last_of("/");
	std::string dir;

	if (slashIndex == std::string::npos) {
		dir = ".";
	}
	else if (slashIndex == 0) {
		dir = "/";
	}
	else {
		dir = fileName.substr(0, slashIndex);
	}

	for(int i = 0; i < pScene->mNumMaterials; ++i)
	{
		const aiMaterial *pMaterial = pScene->mMaterials[i];
		if(pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString path;
			if(pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				std::string fullPath = dir + "/" + path.data;

				gEngModule->pTexMgr->create(fullPath.c_str());
				//IMesh *pMeshRes = dynamic_cast<IMesh*>(res);
				//pMeshRes->addImageResName(fullPath.c_str());
				
				//if(!mTextures[i]->load(GL_TEXTURE_2D, fullPath))
				//{
					//std::cout<<"load matertial texture fail "<<std::endl;
					//delete mTextures[i];
					//mTextures[i] = NULL;
					//ret = false;
				//}
			}
		}
	}
}


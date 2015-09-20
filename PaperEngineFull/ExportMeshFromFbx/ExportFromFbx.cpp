#include <iostream>
#include <fstream>
#include "ExportFromFbx.h"

#ifdef _WIN32
#include <direct.h>
#endif

#include "rapidxml/rapidxml.hpp"  
#include "rapidxml/rapidxml_utils.hpp"  
#include "rapidxml/rapidxml_print.hpp" 

using namespace rapidxml;


ExportFromFbx::ExportFromFbx(const std::string &fileName)
{
	initializeSdkObjects();
	loadFbx(fileName);
}

ExportFromFbx::~ExportFromFbx()
{
	/*for (int i = 0; i < mSubMeshVec.GetCount(); ++i)
	{
		delete mSubMeshVec[i];
	}
	mSubMeshVec.Clear();*/

	for (int i = 0; i < mMeshVec.size(); ++i)
	{
		delete mMeshVec[i];
	}
	mMeshVec.clear();
}

void ExportFromFbx::initializeSdkObjects()
{
	//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
	mpManager = FbxManager::Create();
	if (!mpManager)
	{
		FBXSDK_printf("Error: Unable to create FBX Manager!\n");
		exit(1);
	}
	else FBXSDK_printf("Autodesk FBX SDK version %s\n", mpManager->GetVersion());

	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(mpManager, IOSROOT);
	mpManager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	FbxString lPath = FbxGetApplicationDirectory();
	mpManager->LoadPluginsDirectory(lPath.Buffer());

	//Create an FBX scene. This object holds most objects imported/exported from/to files.
	mpScene = FbxScene::Create(mpManager, "My Scene");
	if (!mpScene)
	{
		FBXSDK_printf("Error: Unable to create FBX scene!\n");
		exit(1);
	}
}

void ExportFromFbx::loadFbx(const std::string &fileName)
{
	// Create the importer.
	int lFileFormat = -1;
	mpImporter = FbxImporter::Create(mpManager, "");
	if (!mpManager->GetIOPluginRegistry()->DetectReaderFileFormat(fileName.c_str(), lFileFormat))
	{
		// Unrecognizable file format. Try to fall back to FbxImporter::eFBX_BINARY
		lFileFormat = mpManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");;
	}

	// Initialize the importer by providing a filename.
	if (mpImporter->Initialize(fileName.c_str(), lFileFormat) == true)
	{
		// The file is going to be imported at
		// the end of the first display callback.
		std::string msg;
		msg = "Importing file ";
		msg += fileName;
		msg += "\nPlease wait!";

		std::cout << msg << std::endl;
		
	}
	mpImporter->Import(mpScene);

	FbxAxisSystem SceneAxisSystem = mpScene->GetGlobalSettings().GetAxisSystem();
	FbxAxisSystem OurAxisSystem(FbxAxisSystem::EUpVector::eYAxis, FbxAxisSystem::EFrontVector::eParityOdd, FbxAxisSystem::eRightHanded);
	//FbxAxisSystem::EPreDefinedAxisSystem preDefineAxis = FbxAxisSystem::eOpenGL;
	//FbxAxisSystem OurAxisSystem(preDefineAxis);
	if (SceneAxisSystem != OurAxisSystem)
	{
		OurAxisSystem.ConvertScene(mpScene);
	}


	// Convert Unit System to what is used in this example, if needed
	FbxSystemUnit SceneSystemUnit = mpScene->GetGlobalSettings().GetSystemUnit();
	if (SceneSystemUnit.GetScaleFactor() != 1.0)
	{
		//The unit in this example is centimeter.
		FbxSystemUnit::cm.ConvertScene(mpScene);
	}

	// Convert mesh, NURBS and patch into triangle mesh
	FbxGeometryConverter lGeomConverter(mpManager);
	lGeomConverter.Triangulate(mpScene, /*replace*/true);

	

	FbxNode *pRootNode = mpScene->GetRootNode();
	/*int lChildCount = pRootNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
	loadCacheRecursive(pRootNode->GetChild(lChildIndex));
	}*/
	std::cout << pRootNode->GetName() << std::endl;
	loadCacheRecursive(pRootNode);
}

void ExportFromFbx::loadCacheRecursive(FbxNode *pNode)
{
	std::vector<std::string> texNameVec;
	const int lMaterialCount = pNode->GetMaterialCount();
	for (int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; ++lMaterialIndex)
	{
		FbxSurfaceMaterial * lMaterial = pNode->GetMaterial(lMaterialIndex);
		if (lMaterial && !lMaterial->GetUserDataPtr())
		{
			const FbxProperty lProperty = lMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (lProperty.IsValid())
			{
				const int lTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();
				if (lTextureCount)
				{
					const FbxFileTexture* lTexture = lProperty.GetSrcObject<FbxFileTexture>();
					if (lTexture)
					{
						std::cout << "texture name: " << lTexture->GetFileName() << std::endl;
						texNameVec.push_back(lTexture->GetFileName());
					}
				}
			}
		}
	}

	FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();
	if (lNodeAttribute)
	{
		// Bake mesh as VBO(vertex buffer object) into GPU.
		if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh * lMesh = pNode->GetMesh();
			OneMesh *ret = processMesh(lMesh);
			if (ret)
			{
				ret->texNameVec = texNameVec;
			}
			else
			{
				//std::cout << "process mesh error !" << std::endl;
			}
		}
	}

	const int lChildCount = pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		loadCacheRecursive(pNode->GetChild(lChildIndex));
	}
}

OneMesh *ExportFromFbx::processMesh(FbxMesh *pMesh)
{
	if (!pMesh->GetNode())
		return 0;

	std::cout << "mesh name = " << pMesh->GetName() << std::endl;

	unsigned int vertexCount = 0;
	unsigned int indexCount = 0;
	OneMesh *pOneMesh = new OneMesh;
	FbxArray<SubMesh*> &subMeshVec = pOneMesh->subMeshVec;

	const int polygonCount = pMesh->GetPolygonCount();

	//unsigned char *pVertexMem = new unsigned char[sizeof(float)* 4 * vertexCount];
	// Count the polygon count of each material
	FbxLayerElementArrayTemplate<int>* lMaterialIndice = NULL;
	FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;
	if (pMesh->GetElementMaterial())
	{
		lMaterialIndice = &pMesh->GetElementMaterial()->GetIndexArray();
		lMaterialMappingMode = pMesh->GetElementMaterial()->GetMappingMode();
		if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
		{
			FBX_ASSERT(lMaterialIndice->GetCount() == polygonCount);
			if (lMaterialIndice->GetCount() == polygonCount)
			{
				// Count the faces of each material
				for (int lPolygonIndex = 0; lPolygonIndex < polygonCount; ++lPolygonIndex)
				{
					const int lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
					if (subMeshVec.GetCount() < lMaterialIndex + 1)
					{
						subMeshVec.Resize(lMaterialIndex + 1);
					}
					if (subMeshVec[lMaterialIndex] == NULL)
					{
						subMeshVec[lMaterialIndex] = new SubMesh;
					}
					subMeshVec[lMaterialIndex]->triangleCount += 1;
				}

				// Make sure we have no "holes" (NULL) in the mSubMeshVec table. This can happen
				// if, in the loop above, we resized the mSubMeshVec by more than one slot.
				for (int i = 0; i < subMeshVec.GetCount(); i++)
				{
					if (subMeshVec[i] == NULL)
						subMeshVec[i] = new SubMesh;
				}

				// Record the offset (how many vertex)
				const int lMaterialCount = subMeshVec.GetCount();
				int lOffset = 0;
				for (int lIndex = 0; lIndex < lMaterialCount; ++lIndex)
				{
					subMeshVec[lIndex]->indexOffset = lOffset;
					lOffset += subMeshVec[lIndex]->triangleCount * 3;
					// This will be used as counter in the following procedures, reset to zero
					subMeshVec[lIndex]->triangleCount = 0;
				}
				FBX_ASSERT(lOffset == polygonCount * 3);
			}
		}
	}

	// All faces will use the same material.
	if (subMeshVec.GetCount() == 0)
	{
		subMeshVec.Resize(1);
		subMeshVec[0] = new SubMesh();
	}

	// Congregate all the data of a mesh to be cached in VBOs.
	// If normal or UV is by polygon vertex, record all vertex attributes by polygon vertex.
	bool isHasNormal = pMesh->GetElementNormalCount() > 0;
	bool isHasUV = pMesh->GetElementUVCount() > 0;
	bool isAllByControlPoint = true;
	FbxGeometryElement::EMappingMode lNormalMappingMode = FbxGeometryElement::eNone;
	FbxGeometryElement::EMappingMode lUVMappingMode = FbxGeometryElement::eNone;
	if (isHasNormal)
	{
		lNormalMappingMode = pMesh->GetElementNormal(0)->GetMappingMode();
		if (lNormalMappingMode == FbxGeometryElement::eNone)
		{
			isHasNormal = false;
		}
		if (isHasNormal && lNormalMappingMode != FbxGeometryElement::eByControlPoint)
		{
			isAllByControlPoint = false;
		}
	}
	if (isHasUV)
	{
		lUVMappingMode = pMesh->GetElementUV(0)->GetMappingMode();
		if (lUVMappingMode == FbxGeometryElement::eNone)
		{
			isHasUV = false;
		}
		if (isHasUV && lUVMappingMode != FbxGeometryElement::eByControlPoint)
		{
			isAllByControlPoint = false;
		}
	}

	// Allocate the array memory, by control point or by polygon vertex.
	int lPolygonVertexCount = pMesh->GetControlPointsCount();
	if (!isAllByControlPoint)
	{
		lPolygonVertexCount = polygonCount * 3;
	}

	ExportMeshVertexFormat *pExportVertexFormat = new ExportMeshVertexFormat[lPolygonVertexCount];
	float * lVertices = new float[lPolygonVertexCount * VERTEX_STRIDE];
	unsigned int * lIndices = new unsigned int[polygonCount * 3];
	float * lNormals = NULL;
	if (isHasNormal)
	{
		lNormals = new float[lPolygonVertexCount * NORMAL_STRIDE];
	}
	float * lUVs = NULL;
	FbxStringList lUVNames;
	pMesh->GetUVSetNames(lUVNames);
	const char * lUVName = NULL;
	if (isHasUV && lUVNames.GetCount())
	{
		lUVs = new float[lPolygonVertexCount * UV_STRIDE];
		lUVName = lUVNames[0];
	}

	// Populate the array with vertex attribute, if by control point.
	const FbxVector4 * lControlPoints = pMesh->GetControlPoints();
	FbxVector4 lCurrentVertex;
	FbxVector4 lCurrentNormal;
	FbxVector2 lCurrentUV;
	if (isAllByControlPoint)
	{
		const FbxGeometryElementNormal * lNormalElement = NULL;
		const FbxGeometryElementUV * lUVElement = NULL;
		if (isHasNormal)
		{
			lNormalElement = pMesh->GetElementNormal(0);
		}
		if (isHasUV)
		{
			lUVElement = pMesh->GetElementUV(0);
		}
		for (int lIndex = 0; lIndex < lPolygonVertexCount; ++lIndex)
		{
			// Save the vertex position.
			lCurrentVertex = lControlPoints[lIndex];
			lVertices[lIndex * VERTEX_STRIDE] = static_cast<float>(lCurrentVertex[0]);
			lVertices[lIndex * VERTEX_STRIDE + 1] = static_cast<float>(lCurrentVertex[1]);
			lVertices[lIndex * VERTEX_STRIDE + 2] = static_cast<float>(lCurrentVertex[2]);
			lVertices[lIndex * VERTEX_STRIDE + 3] = 1;

			//坐标系转换
			FbxVector4 vec4 = FbxVector4(static_cast<float>(lCurrentVertex[0]), static_cast<float>(lCurrentVertex[1]), static_cast<float>(lCurrentVertex[2]), 1);
			FbxMatrix transformMat = mpScene->GetRootNode()->EvaluateGlobalTransform();
			//vec4 = transformMat.MultNormalize(vec4);

			pExportVertexFormat[lIndex].x = static_cast<float>(lCurrentVertex[0]);
			pExportVertexFormat[lIndex].y = static_cast<float>(lCurrentVertex[1]);
			pExportVertexFormat[lIndex].z = static_cast<float>(lCurrentVertex[2]);


			// Save the normal.
			if (isHasNormal)
			{
				int lNormalIndex = lIndex;
				if (lNormalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
				{
					lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndex);
				}
				lCurrentNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
				lNormals[lIndex * NORMAL_STRIDE] = static_cast<float>(lCurrentNormal[0]);
				lNormals[lIndex * NORMAL_STRIDE + 1] = static_cast<float>(lCurrentNormal[1]);
				lNormals[lIndex * NORMAL_STRIDE + 2] = static_cast<float>(lCurrentNormal[2]);
			}

			// Save the UV.
			if (isHasUV)
			{
				int lUVIndex = lIndex;
				if (lUVElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
				{
					lUVIndex = lUVElement->GetIndexArray().GetAt(lIndex);
				}
				lCurrentUV = lUVElement->GetDirectArray().GetAt(lUVIndex);
				lUVs[lIndex * UV_STRIDE] = static_cast<float>(lCurrentUV[0]);
				lUVs[lIndex * UV_STRIDE + 1] = static_cast<float>(lCurrentUV[1]);

				pExportVertexFormat[lIndex].uvx = static_cast<float>(lCurrentUV[0]);
				pExportVertexFormat[lIndex].uvy = static_cast<float>(lCurrentUV[1]);
			}
		}

	}

	int lVertexCount = 0;
	for (int lPolygonIndex = 0; lPolygonIndex < polygonCount; ++lPolygonIndex)
	{
		// The material for current face.
		int lMaterialIndex = 0;
		if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
		{
			lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
		}

		// Where should I save the vertex attribute index, according to the material
		const int lIndexOffset = subMeshVec[lMaterialIndex]->indexOffset +
			subMeshVec[lMaterialIndex]->triangleCount * 3;
		for (int lVerticeIndex = 0; lVerticeIndex < 3; ++lVerticeIndex)
		{
			const int lControlPointIndex = pMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex);

			if (isAllByControlPoint)
			{
				lIndices[lIndexOffset + lVerticeIndex] = static_cast<unsigned int>(lControlPointIndex);
			}
			// Populate the array with vertex attribute, if by polygon vertex.
			else
			{
				lIndices[lIndexOffset + lVerticeIndex] = static_cast<unsigned int>(lVertexCount);

				lCurrentVertex = lControlPoints[lControlPointIndex];
				lVertices[lVertexCount * VERTEX_STRIDE] = static_cast<float>(lCurrentVertex[0]);
				lVertices[lVertexCount * VERTEX_STRIDE + 1] = static_cast<float>(lCurrentVertex[1]);
				lVertices[lVertexCount * VERTEX_STRIDE + 2] = static_cast<float>(lCurrentVertex[2]);
				lVertices[lVertexCount * VERTEX_STRIDE + 3] = 1;

				/*pExportVertexFormat[lVertexCount].x = static_cast<float>(lCurrentVertex[0]);
				pExportVertexFormat[lVertexCount].y = static_cast<float>(lCurrentVertex[1]);
				pExportVertexFormat[lVertexCount].z = static_cast<float>(lCurrentVertex[2]);*/

				FbxVector4 vec4 = FbxVector4(static_cast<float>(lCurrentVertex[0]), static_cast<float>(lCurrentVertex[1]), static_cast<float>(lCurrentVertex[2]), 1);
				FbxMatrix transformMat = mpScene->GetRootNode()->EvaluateGlobalTransform();
				//vec4 = transformMat.MultNormalize(vec4);

				pExportVertexFormat[lVertexCount].x = static_cast<float>(lCurrentVertex[0]);
				pExportVertexFormat[lVertexCount].y = static_cast<float>(lCurrentVertex[1]);
				pExportVertexFormat[lVertexCount].z = static_cast<float>(lCurrentVertex[2]);

				if (isHasNormal)
				{
					pMesh->GetPolygonVertexNormal(lPolygonIndex, lVerticeIndex, lCurrentNormal);
					lNormals[lVertexCount * NORMAL_STRIDE] = static_cast<float>(lCurrentNormal[0]);
					lNormals[lVertexCount * NORMAL_STRIDE + 1] = static_cast<float>(lCurrentNormal[1]);
					lNormals[lVertexCount * NORMAL_STRIDE + 2] = static_cast<float>(lCurrentNormal[2]);
				}

				if (isHasUV)
				{
					bool lUnmappedUV;
					pMesh->GetPolygonVertexUV(lPolygonIndex, lVerticeIndex, lUVName, lCurrentUV, lUnmappedUV);
					lUVs[lVertexCount * UV_STRIDE] = static_cast<float>(lCurrentUV[0]);
					lUVs[lVertexCount * UV_STRIDE + 1] = static_cast<float>(lCurrentUV[1]);

					pExportVertexFormat[lVertexCount].uvx = static_cast<float>(lCurrentUV[0]);
					pExportVertexFormat[lVertexCount].uvy = static_cast<float>(lCurrentUV[1]);
				}
			}
			++lVertexCount;
		}
		subMeshVec[lMaterialIndex]->triangleCount += 1;
	}

	//saveBaseData("save.mesh", pExportVertexFormat, polygonCount, lIndices, polygonCount * 3);
	//OneMesh *pOneMesh = new OneMesh;
	std::string meshName = pMesh->GetName();
	if (meshName.size() == 0)
	{
		char meshNameBuffer[MAX_MESH_NAME];
		memset(meshNameBuffer, 0, MAX_MESH_NAME);
		sprintf_s(meshNameBuffer, "mesh%d", mMeshVec.size());
		meshName = meshNameBuffer;
	}
	
	pOneMesh->meshName = meshName;
	pOneMesh->vertexCount = lPolygonVertexCount;
	pOneMesh->indexCount = polygonCount * 3;
	pOneMesh->pVertexData = pExportVertexFormat;
	pOneMesh->pIndexData = lIndices;
	mMeshVec.push_back(pOneMesh);

	//delete[] pExportVertexFormat;
	//delete[] lIndices;
	delete[] lVertices;

	return pOneMesh;
}

void ExportFromFbx::saveMeshData(OneMesh *pMesh)
{
	//fileName.append(".mesh");
	std::string fileName = mCreatePathDict + "/" + pMesh->meshName;
	fileName.append(".mesh");
	std::ofstream fs(fileName, std::ios_base::out | std::ios_base::binary);


	//内存排列为：
	//MeshInfo mesh的头信息 子网格index起始信息 顶点合集信息 索引信息 

	//int offsetByte = 0;
	MeshInfo meshinfo;
	OneMesh *meshData = pMesh;
	meshinfo.vertexCount = meshData->vertexCount;
	meshinfo.indexCount = meshData->indexCount;
	meshinfo.subMeshCount = meshData->subMeshVec.Size();
	//memcpy(meshinfo.meshName, meshData->meshName.c_str(), meshData->meshName.size());
	//meshinfo.offsetByte = offsetByte;
	fs.write(reinterpret_cast<const char*>(&meshinfo), sizeof(MeshInfo));

	//写入subMesh信息
	for (int i = 0; i < meshData->subMeshVec.Size(); ++i)
	{
		fs.write(reinterpret_cast<const char*>(meshData->subMeshVec[i]), sizeof(SubMesh));
	}

	//写入顶点信息
	fs.write(reinterpret_cast<const char *>(meshData->pVertexData), sizeof(ExportMeshVertexFormat)* meshData->vertexCount);

	//写入索引信息
	fs.write(reinterpret_cast<const char *>(meshData->pIndexData), sizeof(unsigned int)* meshData->indexCount);
	

	fs.close();
	std::cout << "save file !!" << std::endl;
}

void ExportFromFbx::saveMaterialXml(OneMesh *pMesh)
{
	std::string fileName = mCreatePathDict + "/" + pMesh->meshName;
	fileName.append(".mtl");

	xml_document<> doc;
	xml_node<>* pRot = doc.allocate_node(node_pi, doc.allocate_string("xml version='1.0' encoding='utf-8'"));
	doc.append_node(pRot);
	xml_node<> *pMaterialNode = doc.allocate_node(node_element, "Material", 0);
	doc.append_node(pMaterialNode);
	
	const int meshNum = mMeshVec.size();
	std::string texName;

	OneMesh *pMeshData = pMesh;

	//xml_node<> *pMeshXmlNode = doc.allocate_node(node_element, "MeshInfo", 0);
	//pMaterialNode->append_node(pMeshXmlNode);
	//pMeshXmlNode->append_attribute(doc.allocate_attribute("MeshName", pMeshData->meshName.c_str()));
	//pEffect->append_attribute(doc.allocate_attribute("ParamNum", "16"));

	xml_node<> *pEffect = doc.allocate_node(node_element, "Effect", 0);
	pMaterialNode->append_node(pEffect);
	pEffect->append_attribute(doc.allocate_attribute("vs", "vertex.vs"));
	pEffect->append_attribute(doc.allocate_attribute("fs", "fragment.fs"));
	pEffect->append_attribute(doc.allocate_attribute("ParamNum", "16"));

	//custom param
	xml_node<> *pDiffuse = doc.allocate_node(node_element, "Param");
	pDiffuse->append_attribute(doc.allocate_attribute("name", "gDiffuse"));
	pDiffuse->append_attribute(doc.allocate_attribute("type", "float4"));
	pDiffuse->append_attribute(doc.allocate_attribute("value", "1.0 1.0 1.0 1.0"));
	pEffect->append_node(pDiffuse);

	//custom texture
	xml_node<> *pTexture = doc.allocate_node(node_element, "Texture", 0);
	pMaterialNode->append_node(pTexture);
	pTexture->append_attribute(doc.allocate_attribute("MaxTexNum", "16"));

	for (int texIndex = 0; texIndex < pMeshData->texNameVec.size(); ++texIndex)
	{
		xml_node<> *pTex = doc.allocate_node(node_element, "tex");
		pTexture->append_node(pTex);

		if (texIndex == 0) //暂时0是diffuse贴图
		{
			pTex->append_attribute(doc.allocate_attribute("Type", "Diffuse"));
		}
		

		std::string fullPathTexName = pMeshData->texNameVec[texIndex].c_str();
		int ret = memcmp(fullPathTexName.c_str(), pMeshData->texNameVec[texIndex].c_str(), pMeshData->texNameVec[texIndex].size());
		texName = fullPathTexName;
#ifdef _WIN32
		texName = fullPathTexName.substr(fullPathTexName.find_last_of('\\') + 1);
		std::string commandStr;
		commandStr.append("copy ");
		commandStr.append(fullPathTexName);
		commandStr.append(" ");
		commandStr.append(".\\" + mCreatePathDict + ".\\" + texName);
		system(commandStr.c_str());
#endif
		texName = "texture/" + texName;
		pTex->append_attribute(doc.allocate_attribute("FileName", texName.c_str()));
	}

	std::ofstream out(fileName);
	out << doc;
	out.close();
}

void ExportFromFbx::saveModelData(std::string fileName)
{
	fileName.append(".model");

	std::ofstream fs(fileName, std::ios_base::out | std::ios_base::binary);


	int allVertexCount = 0;
	int allIndexCount = 0;
	for (int i = 0; i < mMeshVec.size(); ++i)
	{
		allVertexCount += mMeshVec[i]->vertexCount;
		allIndexCount += mMeshVec[i]->indexCount;
	}

	ExportHeadInfo headInfo;
	headInfo.allVertexNum = allVertexCount;
	headInfo.allIndexNum = allIndexCount;
	//headInfo.subMeshCount = mSubMeshVec.GetCount();
	headInfo.meshNum = mMeshVec.size();
	fs.write(reinterpret_cast<const char*>(&headInfo), sizeof(ExportHeadInfo));

	//写入mesh名字
	for (int i = 0; i < mMeshVec.size(); ++i)
	{
		//mMeshVec[i]->meshName.append(".mesh");
		fs.write(mMeshVec[i]->meshName.c_str(), mMeshVec[i]->meshName.size() + 1);
	}

	fs.close();
}

void ExportFromFbx::saveData(std::string fileName)
{
#ifdef _WIN32	
	std::string createPath = fileName.substr(0, fileName.find_first_of("\\"));
	_mkdir(createPath.c_str());
	mCreatePathDict = createPath;
#endif
	saveModelData(fileName);

	for (int i = 0; i < mMeshVec.size(); ++i)
	{
		saveMeshData(mMeshVec[i]);
		saveMaterialXml(mMeshVec[i]);
	}
	
	
}
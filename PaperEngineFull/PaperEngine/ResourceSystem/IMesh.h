#ifndef _I_MESH_H_
#define _I_MESH_H_

#include <vector>
#include "util/Engine_Define.h"
#include "util/Paper_Math.h"
#include "util/CollionGeo.h"
#include "ResourceSystem/Resource.h"
#include "ResourceSystem/ResHandle.h"
#include "util/RenderCommand.h"

const int MAX_MESH_NAME = 32;
const int MAX_MTL_NAME = 32;

enum VertexDataSortType
{
	P3F2F3 = 0,
	P3UVF2,
	P3I1F2,
	P3,

	UNKNOW,
};

enum InstanceDataSortType
{
	MVP_WORLD_TYPE = 0,

	INS_UNKNOW,
};

struct Vertex_P3F2F3
{
	math::Vector3f pos;
	math::Vector2f texcood;
	math::Vector3f normal;
};

struct Vertex_P3UVF2
{
	math::Vector3f pos;
	math::Vector2f texcood;
};

struct Vertex_P3I1F2
{
	math::Vector3f pos;
	uint color;
	math::Vector2f uv;
};

struct VertexData
{
	unsigned char *pMemData;
	uint bufferId;
	uint elementCount;
	uint elementSize;
	VertexDataSortType type;

	VertexData() :
		pMemData(0),
		bufferId(0),
		elementCount(0),
		elementSize(0),
		type(VertexDataSortType::UNKNOW)
	{

	}

	void unload()
	{
		pMemData = 0;
		bufferId = 0;
		elementCount = 0;
		elementSize = 0;
		type = VertexDataSortType::UNKNOW;
	}
};

struct InstanceData
{
	unsigned char *pMemData;
	uint bufferId;
	uint elementCount;
	uint elementSize;
	InstanceDataSortType type;

	InstanceData() :
		pMemData(0),
		bufferId(0),
		elementCount(0),
		elementSize(0),
		type(InstanceDataSortType::INS_UNKNOW)
	{}

	void unload()
	{
		pMemData = 0;
		bufferId = 0;
		elementCount = 0;
		elementSize = 0;
		type = InstanceDataSortType::INS_UNKNOW;
	}
};

enum IndexType
{
	BIT16,
	BIT32,
};

struct IndexData
{
	unsigned char *pMemData;
	uint bufferId;
	uint elementCount;
	IndexType type;
	uint elementSize;

	IndexData() :
		pMemData(0),
		bufferId(0),
		elementCount(0),
		type(BIT32),
		elementSize(0)
	{

	}

	void unload()
	{
		pMemData = 0;
		bufferId = 0;
		elementCount = 0;
		type = BIT32;
		elementSize = 0;
	}
};

struct ENGINE_DLL SubMesh
{
	SubMesh() : indexOffset(0), triangleCount(0) 
	{
		memset(mtlName, 0, MAX_MTL_NAME);
	}

	int indexOffset;
	int triangleCount;
	char mtlName[MAX_MTL_NAME];
};

class IMesh;
typedef ResHandle<IMesh*> MeshHandle;

typedef std::vector<SubMesh> SubMeshVec;
class IMesh : public Resource
{
public:
	virtual ~IMesh() { mCbSignal.disconnect_all(); };

	virtual void setMeshMemData(unsigned char *pMeshMemData) = 0;
	virtual void setMeshMemVertexData(VertexData &vertexData) = 0;
	virtual void setMeshMemIndexData(IndexData &indexData) = 0;
	virtual void setMeshMemVertexData(unsigned char *pMemData, const uint count, const VertexDataSortType type) = 0;
	virtual void setMeshMemIndexData(unsigned char *pMemData, const uint count, const IndexType type) = 0;

	virtual void updateHWData() = 0;
	virtual void bindVertexData() = 0;
	virtual void bindIndexData() = 0;
	virtual void setRenderIndexValue(int value) = 0;

	virtual VertexData *getVertexData() = 0;
	virtual IndexData *getIndexData() = 0;

	virtual void setSubMeshVec(const SubMeshVec &subMeshVec) = 0;
	virtual const SubMeshVec &getSubMeshVec() const = 0;
	virtual void generateRenderCommand(RenderCommand &renderCommand) = 0;
	virtual void generateRenderCommand(RenderCommand &renderCommand, const int subIndex) = 0;

	//virtual unsigned char* getVertexData() = 0;
	virtual AABB getAABB() const = 0;
	virtual void setInstanceRender(const bool isInstanceRender) = 0;
	virtual void bindInstanceData(unsigned char *pMemData, uint elementSize, uint elementCount, InstanceDataSortType type) = 0;

	typedef sigslot::signal1<MeshHandle> CbSignal;
	virtual CbSignal &getSignalRef() { return mCbSignal; }

protected:
	CbSignal mCbSignal;
};



#endif
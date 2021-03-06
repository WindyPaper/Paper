#ifndef _OPENGL_MESH_H_
#define _OPENGL_MESH_H_

#include "ResourceSystem/IMesh.h"
#include "util/RenderCommand.h"


class ITask;

class ENGINE_DLL OpenGLMesh : public IMesh
{
public:
	OpenGLMesh();
	virtual ~OpenGLMesh();

	virtual bool load(const std::string &name);
	virtual bool load(const std::string &name, const NameParamMap &paramMap);
	virtual bool load(ResourceLoader *pLoader);
	virtual bool loadMesh(const std::string &fileName);
	virtual void unload();

	virtual void setMeshMemData(unsigned char *pMeshMemData);

	virtual void setMeshMemVertexData(VertexData &vertexData);
	virtual void setMeshMemIndexData(IndexData &indexData);
	void setMeshMemVertexData(unsigned char *pMemData, const uint count, const VertexDataSortType type);
	void setMeshMemIndexData(unsigned char *pMemData, const uint count, const IndexType type);

	virtual VertexData *getVertexData();
	virtual IndexData *getIndexData();

	//virtual void genHWData();
	virtual void delHWData();
	virtual void updateHWData();
	virtual void bindVertexData();
	virtual void bindIndexData();

	virtual void bindInstanceData(unsigned char *pMemData, uint elementSize, uint elementCount, InstanceDataSortType type);

	//this function only for mygui render
	//这个接口是为mygui渲染开放的
	virtual void setRenderIndexValue(int value);

	virtual void setSubMeshVec(const SubMeshVec &subMeshVec);
	virtual const SubMeshVec &getSubMeshVec() const;
	virtual void generateRenderCommand(RenderCommand &renderCommand);
	virtual void generateRenderCommand(RenderCommand &renderCommand, const int subIndex);

	virtual AABB getAABB() const { return mMeshAABB; }
	//void setWorldAABB(const AABB &aabb) { mWorldAABB = aabb; }
	//AABB &getWorldAABB() { return mWorldAABB; }
	void setRenderMode(const RenderCommand::RenderMode &renderMode);
	virtual void setInstanceRender(const bool isInstanceRender);

	//virtual unsigned char* getVertexData();

	void runCbSignal();

protected:
	void calAABB(const VertexData &verdata);

private:
	VertexData mVertexData;
	IndexData mIndexData;
	unsigned char *mMeshMemData;
	NameParamMap mParamMap;

	InstanceData mInstanceData;

	SubMeshVec mSubMeshVec;

	AABB mMeshAABB;
	//AABB mWorldAABB;
	RenderCommand::RenderMode mRenderMode;
	bool mIsInstanceRender;

	ITask *mpLoadTask;
};

#endif
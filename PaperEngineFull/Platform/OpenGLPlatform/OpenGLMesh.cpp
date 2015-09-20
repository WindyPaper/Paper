#include "OpenGLPlatform/RenderPreCompileHead.h"
#include "util/Global.h"
#include "util/Log.h"
#include "util/ITask.h"
#include "OpenGLPlatform/OpenGLMesh.h"
#include "OpenGLPlatform/OpenGLImpl.h"
#include "ResourceSystem/AssimpMeshLoader.h"
#include "ResourceSystem/CustomMeshLoader.h"

class MeshLoadTask : public ITask
{
public:
	virtual void execute()
	{
		mpMesh->lockRes();

		mpMesh->loadMesh(mFileName);

		mpMesh->unlockRes();
	}

	void setLoadMeshPoint(const std::string &fileName, OpenGLMesh *pMesh)
	{
		mFileName = fileName;
		mpMesh = pMesh;
	}

	virtual void complete()
	{
		mpMesh->bindVertexData();
		mpMesh->bindIndexData();
		mpMesh->runCbSignal();	 	
	}

protected:
	OpenGLMesh* mpMesh;
	std::string mFileName;
};

OpenGLMesh::OpenGLMesh() :
mMeshMemData(0),
mpLoadTask(0)
{

}

OpenGLMesh::~OpenGLMesh()
{
	//delHWData();
	unload();
}

bool OpenGLMesh::load(const std::string &name, const NameParamMap &paramMap)
{
	mName = name;
	mParamMap = paramMap;

	NameParamMap::const_iterator iter = paramMap.find("type");
	if (iter != paramMap.end())
	{
		const std::string &typeStr = iter->second;
	}
	//CustomMeshLoader loader;
	//return loader.loadRes(this);
	return true;
}

bool OpenGLMesh::load(ResourceLoader *pLoader)
{
	return pLoader->loadRes(this);
}

//fix. del input param
bool OpenGLMesh::load(const std::string &name)
{
	mName = name;
	//CustomMeshLoader loader;
	//return loader.loadRes(this);
	MeshLoadTask *pTask = new MeshLoadTask();
	mpLoadTask = pTask;
	pTask->setLoadMeshPoint(name, this);
	gEngModule->pTaskDispatcher->pushTask(pTask);
	return true;
}

bool OpenGLMesh::loadMesh(const std::string &fileName)
{
	mName = fileName;
	CustomMeshLoader loader;
	return loader.loadRes(this);
}

void OpenGLMesh::unload()
{
	delHWData();
	mVertexData.unload();
	mIndexData.unload();
	SAFE_DELETE_ARRAY(mMeshMemData);
}

void OpenGLMesh::setMeshMemData(unsigned char *pMeshMemData)
{
	SAFE_DELETE_ARRAY(mMeshMemData);
	mMeshMemData = pMeshMemData;
}

void OpenGLMesh::setMeshMemVertexData(unsigned char *pMemData, const uint count, const VertexDataSortType type)
{
	mVertexData.pMemData = pMemData;
	mVertexData.elementCount = count;
	mVertexData.type = type;
	/*if (type == VertexDataSortType::P3F2F3)
	{
		mVertexData.elementSize = sizeof(Vertex_P3F2F3);
	}*/
	
	switch (type)
	{
	case VertexDataSortType::P3F2F3:
		mVertexData.elementSize = sizeof(Vertex_P3F2F3);
		break;
	case VertexDataSortType::P3UVF2:
		mVertexData.elementSize = sizeof(Vertex_P3UVF2);
		break;
	case VertexDataSortType::P3I1F2:
		mVertexData.elementSize = sizeof(Vertex_P3I1F2);
		break;
	default:
		break;
	}

	//setMeshMemVertexData(mVertexData);
}

void OpenGLMesh::setMeshMemVertexData(VertexData &vertexData)
{
	if (vertexData.elementCount > 0)
	{
		if (mVertexData.bufferId != 0)
		{
			glDeleteBuffers(1, (GLuint*)(&(mVertexData.bufferId)));
		}
		mVertexData = vertexData;
		glGenBuffers(1, (GLuint*)(&(mVertexData.bufferId)));
		bindVertexData();
	}
}

void OpenGLMesh::setMeshMemIndexData(unsigned char *pMemData, const uint count, const IndexType type)
{
	mIndexData.pMemData = pMemData;
	mIndexData.elementCount = count;
	mIndexData.type = type;

	if (type == IndexType::BIT16)
	{
		mIndexData.elementSize = 2;
	}
	else if (type == IndexType::BIT32)
	{
		mIndexData.elementSize = 4;
	}

	//setMeshMemIndexData(mIndexData);
}

void OpenGLMesh::setMeshMemIndexData(IndexData &indexData)
{
	if (mIndexData.bufferId != 0)
	{
		glDeleteBuffers(1, (GLuint*)(&(mIndexData.bufferId)));
	}
	mIndexData = indexData;
	glGenBuffers(1, (GLuint*)(&(mIndexData.bufferId)));
	bindIndexData();
}

VertexData * OpenGLMesh::getVertexData()
{
	return &mVertexData;
}

IndexData * OpenGLMesh::getIndexData()
{
	return &mIndexData;
}

/*
void OpenGLMesh::genHWData()
{
	glGenBuffers(1, (GLuint*)(&(mVertexData.bufferId)));
	glGenBuffers(1, (GLuint*)(&(mIndexData.bufferId)));
}*/

void OpenGLMesh::delHWData()
{
	if (mVertexData.bufferId != 0)
	{
		glDeleteBuffers(1, (GLuint*)(&(mVertexData.bufferId)));
	}
	
	if (mIndexData.bufferId != 0)
	{
		glDeleteBuffers(1, (GLuint*)(&(mIndexData.bufferId)));
	}	
}

void OpenGLMesh::updateHWData()
{
	bindVertexData();
	bindIndexData();
}

void OpenGLMesh::bindVertexData()
{
	if (mVertexData.elementCount > 0)
	{
		if (mVertexData.bufferId != 0)
		{
			glDeleteBuffers(1, (GLuint*)(&(mVertexData.bufferId)));
		}
		//mVertexData = vertexData;
		glGenBuffers(1, (GLuint*)(&(mVertexData.bufferId)));
		//bindVertexData();
	//}

		OpenGLImpl::getInstance().activeVertexBufObj(static_cast<GLuint>(mVertexData.bufferId));
		glBufferData(GL_ARRAY_BUFFER, mVertexData.elementSize * mVertexData.elementCount, mVertexData.pMemData, GL_STATIC_DRAW); // ?GL_DYNAMIC_DRAW @@fix me
		OpenGLImpl::getInstance().activeVertexBufObj(0);
	}
}

void OpenGLMesh::bindIndexData()
{
	if (mIndexData.elementCount > 0)
	{
		if (mIndexData.bufferId != 0)
		{
			glDeleteBuffers(1, (GLuint*)(&(mIndexData.bufferId)));
		}
		//mIndexData = indexData;
		glGenBuffers(1, (GLuint*)(&(mIndexData.bufferId)));
		//bindIndexData();

		OpenGLImpl::getInstance().activeIndexBufObj(static_cast<GLuint>(mIndexData.bufferId));
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexData.elementSize * mIndexData.elementCount, mIndexData.pMemData, GL_STATIC_DRAW); // ?GL_DYNAMIC_DRAW @@fix me
		OpenGLImpl::getInstance().activeIndexBufObj(0);
	}
}

void OpenGLMesh::setRenderIndexValue(int value)
{
	mIndexData.elementCount = value;
}

void OpenGLMesh::setSubMeshVec(const std::vector<SubMesh> &subMeshVec)
{
	mSubMeshVec = subMeshVec;
}

const SubMeshVec & OpenGLMesh::getSubMeshVec() const
{
	return mSubMeshVec;
}

void OpenGLMesh::generateRenderCommand(RenderCommand &renderCommand, const int subIndex)
{
	renderCommand.renderMode = RenderCommand::TRIANGLE_LIST;

	renderCommand.pVertexData = &mVertexData;	
	renderCommand.pIndexData = &mIndexData;
	
	if (mSubMeshVec.size() > subIndex)
	{
		renderCommand.indexStart = mSubMeshVec[subIndex].indexOffset;
		renderCommand.indexCount = mSubMeshVec[subIndex].triangleCount * 3;
	}
	else
	{
		//Log::getInstance().logMsg("Mesh %s SubIndex %d Error !!", mName.c_str(), subIndex);
		renderCommand.indexStart = 0;
		renderCommand.indexCount = mIndexData.elementCount;
	}
	
}

void OpenGLMesh::generateRenderCommand(RenderCommand &renderCommand)
{
	generateRenderCommand(renderCommand, 0);
}

/*
unsigned char* OpenGLMesh::getVertexData()
{
	return mVertexData.pMemData;
}*/

void OpenGLMesh::runCbSignal()
{
	MeshHandle handle = gEngModule->pMeshMgr->getHandle(mName);
	mCbSignal.emit(handle);
}

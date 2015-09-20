#include "MyGUI_IVertexBuffer.h"
#include "ResourceSystem/IMesh.h"
#include "GameObject/Renderable.h"

class UIRenderable : public Renderable, public MyGUI::IVertexBuffer
{
public:
	UIRenderable();

	virtual ~UIRenderable();

	virtual MaterialHandle getMaterial();
	virtual MeshHandle getMeshHandle();
	virtual void setMaterial(const std::string &name);
	virtual void setMaterial(MaterialHandle materialHandle);

	virtual void setParentComp(IGameObjRenderComp *parent) {}
	virtual IGameObjRenderComp *getParentComp() { return 0; }

	virtual void generateRenderCommand(RenderCommand &renderCommand);

	//IVertexBuffer
	virtual void setVertexCount(size_t _value);
	virtual size_t getVertexCount();

	virtual MyGUI::Vertex* lock();
	virtual void unlock();

	//--- add
	void setUIVertexData(unsigned char *pData, int count);
	void setUIIndexData(unsigned int *pData, int count);
	//const std::string &getName() const { return mResName; }
	void setRenderIndexValue(int value);

private:
	MeshHandle mMeshHandle;
	MaterialHandle mMatHandle;

	uint mVertexCount;

	std::string mResName;
};
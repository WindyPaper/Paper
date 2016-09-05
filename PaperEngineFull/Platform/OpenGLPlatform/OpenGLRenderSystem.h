#ifndef _OPENGL_RENDER_SYSTEM_H_
#define _OPENGL_RENDER_SYSTEM_H_

#include "util/Paper_Math.h"
#include "util/Singleton.h"
#include "util/IRenderSequence.h"
#include "util/RenderCommand.h"
#include "OpenGLPlatform/OpenGLImpl.h"
#include "ResourceSystem/IMaterial.h"

class OpenGLWin32Support;
class MaterialPass;
class ShaderParam;
class OpenGLWin32Window;
class Camera;
class IRenderTexture;
class OpenGLGBuffer;

class PostEffectRenderCommand
{
public:
	PostEffectRenderCommand();
	~PostEffectRenderCommand();


	void draw(IRenderTexture *backBufferTex);
	void draw(OpenGLGBuffer *pGBuffer);

private:
	uint mGLBufferVertexId;
	uint mGLBufferIndexId;
	int mVertexSize;
	int mIndexSize;
	MaterialHandle mMatHandle;
};

class ENGINE_DLL OpenGLRenderSystem : public Singleton<OpenGLRenderSystem>
{
public:
	enum RenderItemType
	{
		NORMAL,
		SHADOW,
		HELPER,
		UI,
	};

	enum DeferredLightingStage
	{
		GeomotryStage,
		LightingStage,
	};

	static OpenGLRenderSystem &getInstance();
	static OpenGLRenderSystem *getInstancePtr();

	OpenGLRenderSystem();
	~OpenGLRenderSystem();

	bool initRenderSystem();

	void beforeRender();
	//void preRender(const Material *pMaterial);
	void bindShaderParam(Renderable *pRenderable, const RenderItemType type);
	void bindBatchShaderParam(IMaterial *pMaterial, DeferredLightingStage stage);
	void bindObjPosParam(Renderable *pRenderable);

	//IRenderSequence *getRenderSequence() { return mpRenderSequence; }

	//test
	void swap();

	const OpenGLWin32Window *getMainRenderWindow() const;
	
	void renderAll();

	

protected:
	void initGL();
	void _render(const RenderCommand &renderCommand);

	//forward render
	void forwardRendering(BatchRenderMap &contain, const RenderItemType type);

	//deferred lighting
	void generateGBuffer(BatchRenderMap &contain);
	void renderDefferedLighting(BatchRenderMap &contain, const RenderItemType type);

private:
	OpenGLWin32Support *mpGLSupport;
	//IRenderSequence *mpRenderSequence;
	//IRenderTexture *mpDepthBuffer;
	OpenGLGBuffer *mpGBuffer;

	IRenderTexture *mpBackBufferTex;
	OpenGLImpl *gpGLImp;	

	PostEffectRenderCommand *mpHdr;
};

#endif
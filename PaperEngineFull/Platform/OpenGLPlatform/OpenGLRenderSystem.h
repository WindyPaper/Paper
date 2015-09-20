#ifndef _OPENGL_RENDER_SYSTEM_H_
#define _OPENGL_RENDER_SYSTEM_H_

#include "util/Paper_Math.h"
#include "util/Singleton.h"
#include "util/IRenderSequence.h"
#include "util/RenderCommand.h"
#include "OpenGLPlatform/OpenGLImpl.h"

class OpenGLWin32Support;
class Material;
class MaterialPass;
class ShaderParam;
class OpenGLWin32Window;
class Camera;
class IRenderTexture;

class ENGINE_DLL OpenGLRenderSystem : public Singleton<OpenGLRenderSystem>
{
public:
	static OpenGLRenderSystem &getInstance();
	static OpenGLRenderSystem *getInstancePtr();

	OpenGLRenderSystem();
	~OpenGLRenderSystem();

	bool initRenderSystem();

	void beforeRender();
	//void preRender(const Material *pMaterial);
	void bindShaderParam(Renderable *pRenderable);

	//IRenderSequence *getRenderSequence() { return mpRenderSequence; }

	//test
	void swap();

	const OpenGLWin32Window *getMainRenderWindow() const;

	void renderOneContain(RenderContain &contain);
	void renderAll();

protected:
	void initGL();
	void _render(const RenderCommand &renderCommand);

private:
	OpenGLWin32Support *mpGLSupport;
	//IRenderSequence *mpRenderSequence;
	IRenderTexture *mpShadowMap;
	OpenGLImpl *gpGLImp;
};

#endif
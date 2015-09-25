#include "OpenGLPlatform/RenderPreCompileHead.h"
#include "GL/glew.h"
#include "util/Engine_Define.h"
#include "util/Camera.h"
#include "util/RenderSequence.h"
#include "util/Global.h"
#include "util/GlobalShaderParamMng.h"
#include "util/Log.h"
#include "util/EngineProfile.h"
#include "GameObject/Renderable.h"
#include "Scene/SceneMgr.h"
#include "OpenGLPlatform/OpenGLRenderSystem.h"
#include "OpenGLPlatform/OpenGLWin32Support.h"
#include "OpenGLPlatform/OpenGLWin32Window.h"
#include "OpenGLPlatform/OpenGLImpl.h"
#include "OpenGLPlatform/OpenGLTexture.h"
#include "OpenGLPlatform/OpenGLShader.h"
#include "OpenGLPlatform/OpenGLMaterialMgr.h"
#include "OpenGLPlatform/OpenGLRenderTexture.h"


template<> OpenGLRenderSystem *Singleton<OpenGLRenderSystem>::msInstance = 0;

OpenGLRenderSystem *OpenGLRenderSystem::getInstancePtr()
{
	return msInstance;
}

OpenGLRenderSystem &OpenGLRenderSystem::getInstance()
{
	assert(msInstance);
	return (*msInstance);
}

OpenGLRenderSystem::OpenGLRenderSystem() :
	mpGLSupport(0),
	gpGLImp(0)
{
	gpGLImp = new OpenGLImpl();

	mpShadowMap = new OpenGLRenderTexture();

	//nvFX::setMessageCallback((nvFX::messageCallbackFunc)Log::logNvFX);
}

OpenGLRenderSystem::~OpenGLRenderSystem()
{
	OpenGLSupport *p = mpGLSupport;
	SAFE_DELETE(mpGLSupport);
	SAFE_DELETE(gpGLImp);
	SAFE_DELETE(mpShadowMap);
}

bool OpenGLRenderSystem::initRenderSystem()
{
	if(mpGLSupport == 0)
	{
		mpGLSupport = new OpenGLWin32Support();
		mpGLSupport->createWindow("TestWin");
	}
	initGL();
	
	mpShadowMap->init(mpGLSupport->getMainRenderWindow()->getWidth(), mpGLSupport->getMainRenderWindow()->getHeight());

	return true;
}

void OpenGLRenderSystem::initGL()
{
	//glShadeModel(GL_SMOOTH);        
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);       // 黑色背景
	//glClearDepth(0.0f);                           
	glEnable(GL_DEPTH_TEST);  
	//glDisable(GL_DEPTH_TEST);
	
	//glDepthFunc(GL_LEQUAL);              

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);


	//背面剔除
	glEnable(GL_CULL_FACE);
	
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // 系统对透视进行修正

	const GLubyte * name = glGetString(GL_VENDOR);
	const GLubyte * biaoshifu = glGetString(GL_RENDERER);
	const GLubyte * OpenGLVersion = glGetString(GL_VERSION);
	Log::getInstance().logMsg("OpenGL实现厂商的名字：%s", name);
	Log::getInstance().logMsg("渲染器标识符：%s", biaoshifu);
	Log::getInstance().logMsg("OpenGL实现的版本号：%s", OpenGLVersion);
}

void OpenGLRenderSystem::_render(const RenderCommand &renderCommand)
{
	VertexData *pVerData = renderCommand.pVertexData;
	IndexData *pIndexData = renderCommand.pIndexData;
	OpenGLImpl::getInstancePtr()->activeVertexBufObj(pVerData->bufferId);
	if (pVerData->type == VertexDataSortType::P3F2F3)
	{
		glEnableVertexAttribArray(BindShaderAttr::ATTR_POSITION_ARRAY);
		glEnableVertexAttribArray(BindShaderAttr::ATTR_TEXCOOD_ARRAY);
		glEnableVertexAttribArray(BindShaderAttr::ATTR_NORMAL_ARRAY);

		glVertexAttribPointer(BindShaderAttr::ATTR_POSITION_ARRAY, 3, GL_FLOAT, GL_FALSE, pVerData->elementSize, 0);
		OpenGLImpl::getInstancePtr()->checkError();
		
		glVertexAttribPointer(BindShaderAttr::ATTR_TEXCOOD_ARRAY, 2, GL_FLOAT, GL_FALSE, pVerData->elementSize, (void*)12);
		OpenGLImpl::getInstancePtr()->checkError();
		
		glVertexAttribPointer(BindShaderAttr::ATTR_NORMAL_ARRAY, 3, GL_FLOAT, GL_FALSE, pVerData->elementSize, (void*)20);
		OpenGLImpl::getInstancePtr()->checkError();	
	}
	else if (pVerData->type == VertexDataSortType::P3UVF2)
	{
		glEnableVertexAttribArray(BindShaderAttr::ATTR_POSITION_ARRAY);
		glEnableVertexAttribArray(BindShaderAttr::ATTR_TEXCOOD_ARRAY);

		glVertexAttribPointer(BindShaderAttr::ATTR_POSITION_ARRAY, 3, GL_FLOAT, GL_FALSE, pVerData->elementSize, 0);
		OpenGLImpl::getInstancePtr()->checkError();

		glVertexAttribPointer(BindShaderAttr::ATTR_TEXCOOD_ARRAY, 2, GL_FLOAT, GL_FALSE, pVerData->elementSize, (void*)12);
		OpenGLImpl::getInstancePtr()->checkError();
	}
	else if (pVerData->type == VertexDataSortType::P3I1F2)
	{
		glEnableVertexAttribArray(BindShaderAttr::ATTR_POSITION_ARRAY);
		glEnableVertexAttribArray(BindShaderAttr::ATTR_COLOR_INT);
		glEnableVertexAttribArray(BindShaderAttr::ATTR_TEXCOOD_ARRAY);

		glVertexAttribPointer(BindShaderAttr::ATTR_POSITION_ARRAY, 3, GL_FLOAT, GL_FALSE, pVerData->elementSize, 0);
		OpenGLImpl::getInstancePtr()->checkError();

		glVertexAttribPointer(BindShaderAttr::ATTR_COLOR_INT, 4, GL_UNSIGNED_BYTE, GL_TRUE, pVerData->elementSize, (void*)12);
		OpenGLImpl::getInstancePtr()->checkError();

		glVertexAttribPointer(BindShaderAttr::ATTR_TEXCOOD_ARRAY, 2, GL_FLOAT, GL_FALSE, pVerData->elementSize, (void*)16);
		OpenGLImpl::getInstancePtr()->checkError();
	}
	OpenGLImpl::getInstancePtr()->activeIndexBufObj(pIndexData->bufferId);
	glDrawElements(GL_TRIANGLES, renderCommand.indexCount, GL_UNSIGNED_INT, (void*)(renderCommand.indexStart * sizeof(uint)));

	EngineProfile::getInstancePtr()->addVerticeNum(renderCommand.indexCount / 3);
	EngineProfile::getInstancePtr()->increaseDrawCallNum();


	OpenGLImpl::getInstancePtr()->checkError();
}


void OpenGLRenderSystem::beforeRender()
{
	//glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // 清除屏幕和深度缓存
	//glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS);
	//glDepthFunc(GL_GREATER);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	EngineProfile::getInstancePtr()->startProfile();
}

/*
void OpenGLRenderSystem::preRender(const Material *pMaterial)
{

}*/

void OpenGLRenderSystem::bindShaderParam(Renderable *pRenderable, const RenderItemType type)
{
	//IMaterial *pIMaterial = gEngModule->pMaterialMgr->getDataPtr(TEST_MATERIAL_NAME);
	IMaterial *pIMaterial = gEngModule->pMaterialMgr->getDataPtr(pRenderable->getMaterial());

	if (pRenderable->getParentComp())
	{
		IShader *pShader = pIMaterial->getShader();
		GlobalShaderParamMng::getInstance().setCurrRenderable(pRenderable);
		const math::Matrix44 &modelMatrix = GlobalShaderParamMng::getInstance().getMatrixParam(GlobalShaderParamMng::MatrixModel);

		switch (type)
		{
		case SHADOW:
			dynamic_cast<OpenGLShader*>(pShader)->bindProgram("TECH_DEFAULT", "shadow");
			break;
		case NORMAL:
			dynamic_cast<OpenGLShader*>(pShader)->bindProgram();
			break;
		default:
			break;
		}		

		pShader->setMatrix("g_Model", modelMatrix);

		Camera *pCamera = SceneMgr::getInstance().getMainCamera();
		pShader->setMatrix("g_View", pCamera->getViewMatrix());
		pShader->setMatrix("g_Proj", pCamera->getProjMatrix());
	}
	else
	{
		IShader *pShader = pIMaterial->getShader();
		dynamic_cast<OpenGLShader*>(pShader)->bindProgram();
	}

	if (type != SHADOW)
		pIMaterial->bindAllTexture();

	OpenGLImpl::getInstance().checkError();
}

const OpenGLWin32Window * OpenGLRenderSystem::getMainRenderWindow() const
{
	return mpGLSupport->getMainRenderWindow();
}

void OpenGLRenderSystem::renderOneContain(RenderContain &contain, const RenderItemType type)
{
	for (RenderContain::iterator iter = contain.begin(); iter != contain.end(); ++iter)
	{
		Renderable *pRenderable = (*iter);
		
		bindShaderParam(pRenderable, type);

		RenderCommand command;
		pRenderable->generateRenderCommand(command);
		_render(command);
	}
}

void OpenGLRenderSystem::renderAll()
{
	//preRender(0);
	
	gEngModule->pUISystem->draw();

	//shadow	
	mpShadowMap->bindForWriting();
	glClear(GL_DEPTH_BUFFER_BIT);
	OpenGLImpl::getInstance().checkError();
	RenderContain renderableVec = gEngModule->pRenderSequence->getRenderSequence(RENDER_LAYER_DEFAULT);
	renderOneContain(renderableVec, SHADOW);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	OpenGLImpl::getInstance().checkError();
	renderableVec = gEngModule->pRenderSequence->getRenderSequence(RENDER_LAYER_DEFAULT);
	renderOneContain(renderableVec, NORMAL);

	//render UI
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	RenderContain uiRenderableVec = gEngModule->pRenderSequence->getRenderSequence(RENDER_LAYER_UI);
	renderOneContain(uiRenderableVec, NORMAL);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	EngineProfile::getInstancePtr()->endProfile();
	gEngModule->pUISystem->setProfileMsg(EngineProfile::getInstancePtr());
}

void OpenGLRenderSystem::swap()
{
	mpGLSupport->swapBuffer();
}

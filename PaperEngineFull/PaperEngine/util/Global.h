#ifndef _GLOBAL_H_
#define _GLOBAL_H_

//put global var here
#include "util/PreDefine.h"
#include "util/IUISystem.h" 
#include "util/IProfile.h"
#include "ResourceSystem/IResMgr.h"
#include "util/IRenderSequence.h"
#include "util/ITask.h"

struct EngineGlobalModule
{
	IMaterialMgr *pMaterialMgr;
	IShaderMgr *pShaderMgr;
	IMeshMgr *pMeshMgr;
	ITextureMgr *pTexMgr;
	IUISystem *pUISystem;
	IRenderSequence *pRenderSequence;
	IProfile *pProfile;
	ITaskDispatcher *pTaskDispatcher;

	EngineGlobalModule()
	{

	}

	~EngineGlobalModule()
	{
		SAFE_DELETE(pUISystem);
		SAFE_DELETE(pRenderSequence);
		SAFE_DELETE(pMaterialMgr);
		SAFE_DELETE(pShaderMgr);
		SAFE_DELETE(pMeshMgr);
		SAFE_DELETE(pTexMgr);	
		SAFE_DELETE(pProfile);
		SAFE_DELETE(pTaskDispatcher);
	}
};

extern EngineGlobalModule *gEngModule;

#endif
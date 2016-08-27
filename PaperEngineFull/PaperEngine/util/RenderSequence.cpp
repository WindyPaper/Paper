#include "util/PrecompileHead.h"
#include "util/RenderSequence.h"
#include "util/Global.h"
#include "GameObject/Renderable.h"
#include "GameObject/StaticRenderable.h"

RenderSequence::RenderSequence()
{

}

RenderSequence::~RenderSequence()
{

}

void RenderSequence::preRender()
{
	for (RenderLayerMap::iterator iter = mRenderLayerMap.begin(); iter != mRenderLayerMap.end(); ++iter)
	{
		BatchRenderMap &batMap = iter->second;
		for (BatchRenderMap::iterator batchIter = batMap.begin(); batchIter != batMap.end(); ++batchIter)
		{
			RenderContain &contain = batchIter->second;
			std::set<std::string> renderBatchNameSet;
			if (contain.size() > 1)
			{
				for (int i = 0; i < contain.size(); ++i)
				{
					IMesh *pMesh = gEngModule->pMeshMgr->getDataPtr(contain[i]->getMeshHandle());
					std::set<std::string>::iterator finder = renderBatchNameSet.find(pMesh->getName());
					if (finder != renderBatchNameSet.end())
					{
						renderBatchNameSet.insert(pMesh->getName());
						//contain[i]->setBatchRenderEnable(true);
					}
					else
					{
						//Log::getInstance().logMsg("same mesh ins %s", pMesh->getName().c_str());
					}
					contain[i]->setBatchRenderEnable(true);
				}
			}

			//if (renderContainSet.size() > 1)
			//{
			//	/*for (int i = 0; i < contain.size(); ++i)
			//	{
			//		if (dynamic_cast<StaticRenderable*>(contain[i]))
			//		{
			//			contain[i]->setBatchRenderEnable(true);
			//		}					
			//	}*/
			//	for (RenderContainSet::iterator iter = renderContainSet.begin(); iter != renderContainSet.end(); ++iter)
			//	{
			//		(*iter)->setBatchRenderEnable(true);
			//	}
			//}
		}
	}
}

void RenderSequence::pushToRenderSequence(Renderable *pRenderable)
{
	pushToRenderSequence(pRenderable, RENDER_LAYER_DEFAULT);
}

void RenderSequence::pushToRenderSequence(Renderable *pRenderable, RenderLayerType renderLayer)
{
	RenderLayerMap::iterator iter = mRenderLayerMap.find(renderLayer);
	if (iter == mRenderLayerMap.end())
	{
		mRenderLayerMap.insert(std::pair<RenderLayerType, BatchRenderMap>(renderLayer, BatchRenderMap()));
	}
	BatchRenderMap &contain = mRenderLayerMap[renderLayer];

	//contain.push_back(pRenderable);
	contain[pRenderable->getBatchRenderId()].push_back(pRenderable);
}

BatchRenderMap & RenderSequence::getRenderSequence(RenderLayerType renderLayer)
{
	RenderLayerMap::iterator iter = mRenderLayerMap.find(renderLayer);
	if (iter == mRenderLayerMap.end())
	{
		mRenderLayerMap.insert(std::pair<RenderLayerType, BatchRenderMap>(renderLayer, BatchRenderMap()));
	}
	return mRenderLayerMap[renderLayer];
}

void RenderSequence::clear()
{
	for (RenderLayerMap::iterator iter = mRenderLayerMap.begin(); iter != mRenderLayerMap.end(); ++iter)
	{
		iter->second.clear();
	}
}

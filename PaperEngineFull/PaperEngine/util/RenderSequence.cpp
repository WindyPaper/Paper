#include "util/PrecompileHead.h"
#include "util/RenderSequence.h"
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
			if (contain.size() > 1)
			{
				for (int i = 0; i < contain.size(); ++i)
				{
					if (dynamic_cast<StaticRenderable*>(contain[i]))
					{
						contain[i]->setBatchRenderEnable(true);
					}					
				}
			}
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

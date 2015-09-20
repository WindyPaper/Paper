#include "util/PrecompileHead.h"
#include "util/RenderSequence.h"

RenderSequence::RenderSequence()
{

}

RenderSequence::~RenderSequence()
{

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
		mRenderLayerMap.insert(std::pair<RenderLayerType, RenderContain>(renderLayer, RenderContain()));
	}
	RenderContain &contain = mRenderLayerMap[renderLayer];

	contain.push_back(pRenderable);
}

RenderContain & RenderSequence::getRenderSequence(RenderLayerType renderLayer)
{
	RenderLayerMap::iterator iter = mRenderLayerMap.find(renderLayer);
	if (iter == mRenderLayerMap.end())
	{
		mRenderLayerMap.insert(std::pair<RenderLayerType, RenderContain>(renderLayer, RenderContain()));
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

#ifndef _I_RENDER_SEQUENCE_H_
#define _I_RENDER_SEQUENCE_H_

#include <vector>
#include <map>

enum RenderLayerType
{
	RENDER_LAYER_BACKGROUND = 0,
	RENDER_LAYER_SKY_BOX,
	RENDER_LAYER_DEFAULT,
	RENDER_LAYER_UI,
};

class Renderable;

typedef std::vector<Renderable*> RenderContain;
typedef std::map<RenderLayerType, RenderContain> RenderLayerMap;

class IRenderSequence
{
public:
	virtual ~IRenderSequence() {}

	virtual void pushToRenderSequence(Renderable *pRenderable) = 0;
	virtual void pushToRenderSequence(Renderable *pRenderable, RenderLayerType renderLayer) = 0;
	virtual RenderContain &getRenderSequence(RenderLayerType renderLayer) = 0;

	virtual void clear() = 0;
};

#endif
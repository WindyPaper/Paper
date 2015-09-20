#ifndef _RENDER_SEQUENCE_H_
#define _RENDER_SEQUENCE_H_

#include "util/IRenderSequence.h"

class RenderSequence : public IRenderSequence
{
public:
	RenderSequence();
	~RenderSequence();

	void pushToRenderSequence(Renderable *pRenderable);
	virtual void pushToRenderSequence(Renderable *pRenderable, RenderLayerType renderLayer);
	RenderContain &getRenderSequence(RenderLayerType renderLayer);

	virtual void clear();
private:
	//RenderContain mRenderContain;
	RenderLayerMap mRenderLayerMap;
};

#endif
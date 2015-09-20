#ifndef _RENDER_COMMAND_H_
#define _RENDER_COMMAND_H_

#include "util/Engine_Define.h"

class VertexData;
class IndexData;

struct RenderCommand
{
	enum RenderMode
	{
		POINT_LIST = 1,
		LINE_LIST,
		LINE_STRIP,
		TRIANGLE_LIST,
		TRIANGLE_STRIP,
		TRIANGLE_FAN,
		SKIP,
	};
	RenderMode renderMode;

	VertexData *pVertexData;

	IndexData *pIndexData;
	uint indexStart;
	uint indexCount;

	RenderCommand() :
		renderMode(TRIANGLE_LIST),
		pVertexData(0),
		pIndexData(0),
		indexStart(0),
		indexCount(0)
	{

	}
};

#endif
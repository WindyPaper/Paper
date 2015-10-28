#ifndef _PLANE_MESH_H_
#define _PLANE_MESH_H_

#include "GameObject/GameObject.h"

struct Vertex_P3UVF2;

class PlaneMesh : public GameObject
{
public:
	PlaneMesh(const int width, const int height, const int seg);
	PlaneMesh(const std::string &name);

	~PlaneMesh();

	void setPlaneSize(const int width, const int height, const int seg);

private:
	int mWidth;
	int mHeight;
	int mSeg;

	Vertex_P3UVF2 *mpVerMemData;
	unsigned int *mpIndexMemData;
};

#endif